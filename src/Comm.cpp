/*!
 * @file
 * @brief
 */

#include <string.h>
#include "tiny/Comm.h"
#include "tiny/Crc16.h"

constexpr auto crc_seed = static_cast<uint16_t>(0xFFFF);

enum {
  stx = 0x02,
  etx = 0x03,
  dle = 0x10
};

enum {
  send_state_send_crc_msb,
  send_state_send_crc_lsb,
  send_state_send_payload,
  send_state_sent_etx
};

enum {
  receive_state_idle,
  receive_state_receive_crc_msb,
  receive_state_receive_crc_lsb,
  receive_state_receive_payload
};

using namespace tiny;

auto Comm::send_control_character(uint8_t byte) -> void
{
  _uart.send(byte);
}

auto Comm::control_character(uint8_t byte) -> bool
{
  return byte == stx || byte == etx || byte == dle;
}

//! Returns true if the character was sent or false if it was escaped
auto Comm::send_byte(uint8_t byte) -> bool
{
  if(!send_escaped && control_character(byte)) {
    send_escaped = true;
    _uart.send(dle);
  }
  else {
    send_escaped = false;
    _uart.send(byte);
  }

  return !send_escaped;
}

auto Comm::send_complete_trampoline(Comm* _this) -> void
{
  _this->send_complete();
}

auto Comm::byte_received_trampoline(Comm* _this, uint8_t byte) -> void
{
  _this->byte_received(byte);
}

// May be running in an interrupt context
auto Comm::send_complete() -> void
{
  // We need to respect this flag because this might be
  // executed from an ISR while a send is being set up
  if(!send_in_progress) {
    return;
  }

  switch(send_state) {
    case send_state_send_crc_msb:
      if(send_byte(send_crc >> 8)) {
        send_state = send_state_send_crc_lsb;
      }
      break;

    case send_state_send_crc_lsb:
      if(send_byte(static_cast<uint8_t>(send_crc))) {
        send_state = send_state_send_payload;
      }
      break;

    case send_state_send_payload:
      if(send_buffer_offset < send_buffer_count) {
        if(send_byte(_send_buffer[send_buffer_offset])) {
          send_buffer_offset++;
        }
      }
      else {
        send_state = send_state_sent_etx;
        send_control_character(etx);
      }
      break;

    case send_state_sent_etx:
      send_in_progress = false;
      break;
  }
}

// May be running in an interrupt context
auto Comm::byte_received(uint8_t byte) -> void
{
  if(received_packet_ready) {
    return;
  }

  if(!receive_escaped && byte == stx) {
    receive_count = 0;
    receive_state = receive_state_receive_crc_msb;
    receive_calculated_crc = crc_seed;
  }
  else if(!receive_escaped && byte == etx) {
    if(receive_state == receive_state_receive_payload) {
      if(receive_calculated_crc == receive_crc) {
        received_packet_ready = true;
      }

      receive_state = receive_state_idle;
    }
  }
  else if(!receive_escaped && byte == dle) {
    receive_escaped = true;
  }
  else {
    receive_escaped = false;

    switch(receive_state) {
      case receive_state_receive_crc_msb:
        receive_crc = byte << 8;
        receive_state = receive_state_receive_crc_lsb;
        break;

      case receive_state_receive_crc_lsb:
        receive_crc |= byte;
        receive_state = receive_state_receive_payload;
        break;

      case receive_state_receive_payload:
        if(receive_count < _receive_buffer_size) {
          _receive_buffer[receive_count++] = byte;
          receive_calculated_crc = Crc16::byte(receive_calculated_crc, byte);
        }
        else {
          receive_state = receive_state_idle;
        }
        break;
    }
  }
}

auto Comm::send(const void* payload, uint8_t length) -> void
{
  if(length > _send_buffer_size) {
    return;
  }

  // Send must be stopped while preparing to send so that send complete
  // ISRs don't wreak havok while we're in an inconsistent state
  send_in_progress = false;
  {
    memcpy(_send_buffer, payload, length);
    send_buffer_count = length;
    send_buffer_offset = 0;
    send_state = send_state_send_crc_msb;
    send_crc = Crc16::block(crc_seed, static_cast<const uint8_t*>(payload), length);
  }
  send_in_progress = true;

  send_control_character(stx);
}

Comm::Comm(
  IUart& uart,
  uint8_t* send_buffer,
  uint8_t send_buffer_size,
  uint8_t* receive_buffer,
  uint8_t receive_buffer_size)
  : _uart(uart),
    receive(),
    _send_complete(this, send_complete_trampoline),
    _byte_received(this, byte_received_trampoline),
    _send_buffer(send_buffer),
    send_crc(),
    _send_buffer_size(send_buffer_size),
    send_buffer_count(),
    send_buffer_offset(),
    send_state(),
    send_escaped(),
    send_in_progress(),
    _receive_buffer(receive_buffer),
    receive_crc(),
    receive_calculated_crc(),
    _receive_buffer_size(receive_buffer_size),
    receive_count(),
    receive_state(),
    receive_escaped(),
    received_packet_ready()
{
  uart.on_send_complete().subscribe(_send_complete);
  uart.on_receive().subscribe(_byte_received);
}

auto Comm::run() -> void
{
  if(received_packet_ready) {
    receive.publish(_receive_buffer, receive_count);
    received_packet_ready = false;
  }
}

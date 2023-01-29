/*!
 * @file
 * @brief Sends and receives payloads via UART.
 *
 * Packet structure:
 * [STX] [CRC MSB] [CRC LSB] [Payload (0-255 bytes)] [ETX]
 *
 * DLE is used to escape control characters sent as data/CRC.
 *
 * Payload integrity is protected by a 16-bit CRC (CRC-CCITT with a seed
 * of 0xFFFF).
 *
 * Buffers a single packet for send and a single packet for receive.
 */

#ifndef tiny_Comm_hpp
#define tiny_Comm_hpp

#include "tiny/IComm.hpp"
#include "tiny/hal/IUart.hpp"
#include "tiny/Event.hpp"

namespace tiny {
  class Comm final : public IComm {
   public:
    Comm(
      IUart& uart,
      uint8_t* send_buffer,
      uint8_t send_buffer_size,
      uint8_t* receive_buffer,
      uint8_t receive_buffer_size);

    Comm(const Comm& other) = delete;

    void operator=(const Comm& other) = delete;

    /*!
     * Runs the tiny comm instance. If any received packets
     * are pending, they will be published.
     */
    void run();

    virtual void send(const void* payload, uint8_t length) override;

    virtual bool sending() const override
    {
      return send_in_progress;
    }

    virtual IEvent<const void*, uint8_t>& on_receive() override
    {
      return receive;
    }

   private:
    void send_complete();
    void byte_received(uint8_t byte);

    void send_control_character(uint8_t byte);
    bool control_character(uint8_t byte);
    bool send_byte(uint8_t byte);

   private:
    IUart& _uart;

    Event<const void*, uint8_t> receive{};

    EventSubscription<> _send_complete;
    EventSubscription<uint8_t> _byte_received;

    uint8_t* _send_buffer;
    uint16_t send_crc{};
    uint8_t _send_buffer_size;
    uint8_t send_buffer_count{};
    uint8_t send_buffer_offset{};
    uint8_t send_state{};
    bool send_escaped{};

    // Makes sure we don't handle send complete
    // while we're setting up another send
    volatile bool send_in_progress{};

    uint8_t* _receive_buffer;
    uint16_t receive_crc{};
    uint16_t receive_calculated_crc{};
    uint8_t _receive_buffer_size;
    uint8_t receive_count{};
    uint8_t receive_state{};
    bool receive_escaped{};

    // Makes sure we don't handle received bytes
    // while waiting for packet to be processed
    volatile bool received_packet_ready{};
  };
}

#endif

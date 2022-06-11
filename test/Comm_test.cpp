/*!
 * @file
 * @brief
 */

#include "tiny/Comm.h"
#include "tiny/Crc16.h"
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "tiny/test_double/Uart.h"

#define and_

using namespace tiny;

TEST_GROUP(Comm)
{
  enum {
    stx = 0x02,
    etx = 0x03,
    dle = 0x10
  };

  enum {
    send_buffer_size = 5,
    receive_buffer_size = 4
  };

  test_double::Uart uart{};

  uint8_t send_buffer[send_buffer_size];
  uint8_t receive_buffer[receive_buffer_size];

  Comm comm{uart, send_buffer, send_buffer_size, receive_buffer, receive_buffer_size};

  EventSubscription<const void*, uint8_t> receive_subscription{static_cast<void*>(nullptr), payload_received};

  void setup()
  {
    comm.on_receive().subscribe(receive_subscription);
  }

  static void payload_received(void* context, const void* payload, uint8_t length)
  {
    static_cast<void>(context);

    mock()
      .actualCall("payload_received")
      .withMemoryBufferParameter("payload", static_cast<const unsigned char*>(payload), length);
  }

  void after_send_completes()
  {
    uart.trigger_send_complete();
  }

  void byte_should_be_sent(uint8_t byte)
  {
    mock().expectOneCall("send").onObject(&uart).withParameter("byte", byte);
  }

#define bytes_should_be_sent(...) _bytes_should_be_sent<__VA_ARGS__>()
  template <uint8_t... bytes>
  void _bytes_should_be_sent()
  {
    for(auto byte : {bytes...}) {
      byte_should_be_sent(byte);
    }
  }

#define crc_of(...) crc_msb(__VA_ARGS__), crc_lsb(__VA_ARGS__)

  template <uint8_t... bytes>
  static constexpr uint16_t crc()
  {
    uint16_t crc = 0xFFFF;
    if constexpr(sizeof...(bytes) > 0) {
      uint8_t _bytes[] = {bytes...};
      for(uint8_t byte : _bytes) {
        crc = calculate_crc(crc, byte);
      }
    }
    return crc;
  }

  static constexpr uint16_t calculate_crc(uint16_t seed, uint8_t byte)
  {
    uint16_t crc = seed;
    byte = crc >> 8 ^ byte;
    byte ^= byte >> 4;
    return (crc << 8) ^ ((uint16_t)(byte << 12)) ^ ((uint16_t)(byte << 5)) ^ ((uint16_t)byte);
  }

#define crc_msb(...) _crc_msb<__VA_ARGS__>()
  template <uint8_t... bytes>
  static constexpr uint8_t _crc_msb()
  {
    return crc<bytes...>() >> 8;
  }

#define crc_lsb(...) _crc_lsb<__VA_ARGS__>()
  template <uint8_t... bytes>
  static constexpr uint8_t _crc_lsb()
  {
    return crc<bytes...>() & 0xFF;
  }

#define when_payload_is_sent(...) _when_payload_is_sent<__VA_ARGS__>()
  template <uint8_t... bytes>
  void _when_payload_is_sent()
  {
    if constexpr(sizeof...(bytes) > 0) {
      uint8_t payload[] = {bytes...};
      comm.send(payload, sizeof(payload));
    }
    else {
      uint8_t dummy;
      comm.send(&dummy, 0);
    }
  }

#define given_that_payload_has_been_sent(...) _given_that_payload_has_been_sent<__VA_ARGS__>()
  template <uint8_t... bytes>
  void _given_that_payload_has_been_sent()
  {
    mock().disable();
    _when_payload_is_sent<bytes...>();
    mock().enable();
  }

  void should_be_sending()
  {
    CHECK_TRUE(comm.sending());
  }

  void should_not_be_sending()
  {
    CHECK_FALSE(comm.sending());
  }

  void and_all_sends_complete()
  {
    while(uart.sending()) {
      uart.trigger_send_complete();
    }
  }

  void and_all_sends_have_completed()
  {
    mock().disable();
    and_all_sends_complete();
    mock().enable();
  }

#define after_bytes_are_received(...) _after_bytes_are_received<__VA_ARGS__>()
  template <uint8_t... bytes>
  void _after_bytes_are_received()
  {
    for(auto byte : {bytes...}) {
      uart.trigger_receive(byte);
    }
  }

#define should_receive_payload(...) _should_receive_payload<__VA_ARGS__>()
  template <uint8_t... bytes>
  void _should_receive_payload()
  {
    if constexpr(sizeof...(bytes) > 0) {
      static const uint8_t payload[] = {bytes...};

      mock()
        .expectOneCall("payload_received")
        .withMemoryBufferParameter("payload", payload, sizeof(payload));
    }
    else {
      mock()
        .expectOneCall("payload_received")
        .withMemoryBufferParameter("payload", nullptr, 0); //, sizeof(payload));
    }
  }

  void nothing_should_happen()
  {
  }

  void after_being_run()
  {
    comm.run();
  }

#define given_that_payload_has_been_received(...)                         \
  do {                                                                    \
    should_receive_payload(__VA_ARGS__);                                  \
    after_bytes_are_received(stx, crc_of(__VA_ARGS__), __VA_ARGS__, etx); \
    and_ after_being_run();                                               \
  } while(0)
};

TEST(Comm, should_report_that_send_is_in_process_when_sending)
{
  given_that_payload_has_been_sent(11, 12, 13);
  should_be_sending();
}

TEST(Comm, should_report_that_send_is_not_in_process_after_send_completes)
{
  given_that_payload_has_been_sent(11, 12, 13);
  and_all_sends_have_completed();
  should_not_be_sending();
}

TEST(Comm, should_not_send_payloads_that_are_too_large)
{
  given_that_payload_has_been_sent(11, 12, 13, 14, 15, 16);
  should_not_be_sending();
}

TEST(Comm, should_send_the_next_byte_only_after_previous_send_completes)
{
  byte_should_be_sent(stx);
  when_payload_is_sent(11, 12, 13);

  byte_should_be_sent(crc_msb(11, 12, 13));
  after_send_completes();

  byte_should_be_sent(crc_lsb(11, 12, 13));
  after_send_completes();
}

TEST(Comm, should_send_packets_with_an_empty_payload)
{
  bytes_should_be_sent(stx, crc_of(), etx);
  when_payload_is_sent();
  and_all_sends_complete();
}

TEST(Comm, should_send_packets_with_a_non_empty_payload)
{
  bytes_should_be_sent(stx, crc_of(11, 12, 13), 11, 12, 13, etx);
  when_payload_is_sent(11, 12, 13);
  and_all_sends_complete();
}

TEST(Comm, should_send_packets_with_full_size_payloads)
{
  bytes_should_be_sent(stx, crc_of(15, 14, 13, 12, 11), 15, 14, 13, 12, 11, etx);
  when_payload_is_sent(15, 14, 13, 12, 11);
  and_all_sends_complete();
}

TEST(Comm, should_escape_control_characters_in_the_send_payload)
{
  bytes_should_be_sent(stx, crc_of(stx, etx, dle), dle, stx, dle, etx, dle, dle, etx);
  when_payload_is_sent(stx, etx, dle);
  and_all_sends_complete();
}

TEST(Comm, should_escape_control_characters_in_the_crc)
{
  bytes_should_be_sent(stx, dle, stx, dle, etx, 0x0E, 0x65, etx);
  when_payload_is_sent(0x0E, 0x65);
  and_all_sends_complete();
}

TEST(Comm, should_reject_received_packets_with_an_invalid_crc)
{
  nothing_should_happen();
  after_bytes_are_received(stx, crc_of(22, 33), 11, 22, 33, etx);
  and_ after_being_run();
}

TEST(Comm, should_not_receive_packets_before_they_are_complete)
{
  nothing_should_happen();
  after_bytes_are_received(stx, crc_of());
  and_ after_being_run();
}

TEST(Comm, should_receive_packets_with_no_payload)
{
  should_receive_payload();
  after_bytes_are_received(stx, crc_of(), etx);
  and_ after_being_run();
}

TEST(Comm, should_receive_packets_with_a_payload)
{
  should_receive_payload(11, 22, 33);
  after_bytes_are_received(stx, crc_of(11, 22, 33), 11, 22, 33, etx);
  and_ after_being_run();
}

TEST(Comm, should_receive_packets_with_full_size_payloads)
{
  should_receive_payload(11, 22, 33, 44);
  after_bytes_are_received(stx, crc_of(11, 22, 33, 44), 11, 22, 33, 44, etx);
  and_ after_being_run();
}

TEST(Comm, should_not_publish_the_same_received_packet_twice)
{
  given_that_payload_has_been_received(11);
  nothing_should_happen();
  after_being_run();
}

TEST(Comm, should_reject_packets_with_payloads_that_are_too_large)
{
  nothing_should_happen();
  after_bytes_are_received(stx, crc_of(11, 22, 33, 44, 55), 11, 22, 33, 44, 55, etx);
  and_ after_being_run();
}

TEST(Comm, should_receive_packets_with_escaped_payloads)
{
  should_receive_payload(stx, etx, dle);
  after_bytes_are_received(stx, crc_of(stx, etx, dle), dle, stx, dle, etx, dle, dle, etx);
  and_ after_being_run();
}

TEST(Comm, should_receive_packets_with_escaped_crcs)
{
  should_receive_payload(0x0E, 0x65);
  after_bytes_are_received(stx, dle, stx, dle, etx, 0x0E, 0x65, etx);
  and_ after_being_run();
}

TEST(Comm, should_receive_multiple_packets)
{
  given_that_payload_has_been_received(11);
  should_receive_payload(22);
  after_bytes_are_received(stx, crc_of(22), 22, etx);
  and_ after_being_run();
}

TEST(Comm, should_receive_a_packet_that_is_interrupted_by_another_packet)
{
  should_receive_payload(11);
  after_bytes_are_received(stx, crc_of(22), 22, stx, crc_of(11), 11, etx);
  and_ after_being_run();
}

TEST(Comm, should_drop_packets_received_before_the_previous_packet_has_been_published)
{
  should_receive_payload(11);
  after_bytes_are_received(stx, crc_of(11), 11, etx, stx, crc_of(22), 22, etx);
  and_ after_being_run();
}

TEST(Comm, should_not_publish_the_same_packet_again_if_multiple_etxs_are_received)
{
  given_that_payload_has_been_received(11);
  nothing_should_happen();
  after_bytes_are_received(etx);
  and_ after_being_run();
}

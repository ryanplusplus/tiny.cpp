/*!
 * @file
 * @brief
 */

#include <cstdint>
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "tiny/Fsm.hpp"

using namespace tiny;
using namespace std;

#define then

TEST_GROUP(Fsm)
{
  typedef void (*State)(uint8_t*, uint8_t, const void*);

  uint8_t context{};
  unique_ptr<Fsm> fsm{};

  enum {
    signal_1 = FsmSignal::user_start,
    signal_2
  };

  void setup()
  {
    mock().strictOrder();
  }

  static void state_a(uint8_t * context, uint8_t signal, const void* data)
  {
    mock()
      .actualCall("state_a")
      .withParameter("context", context)
      .withParameter("signal", static_cast<uint8_t>(signal))
      .withParameter("data", data);
  }

  static void state_b(uint8_t * context, uint8_t signal, const void* data)
  {
    mock()
      .actualCall("state_b")
      .withParameter("context", context)
      .withParameter("signal", static_cast<uint8_t>(signal))
      .withParameter("data", data);
  }

  void signal_should_be_sent_to_state(State state, uint8_t signal, const void* data)
  {
    ((state == state_a) ? mock().expectOneCall("state_a") : mock().expectOneCall("state_b"))
      .withParameter("context", &context)
      .withParameter("signal", static_cast<uint8_t>(signal))
      .withParameter("data", data);
  }

  void when_the_fsm_is_initialized_with_state(State state)
  {
    fsm = make_unique<Fsm>(state, &context);
  }

  void given_that_the_fsm_has_been_initialized_with_state(State state)
  {
    mock().disable();
    when_the_fsm_is_initialized_with_state(state);
    mock().enable();
  }

  void when_the_fsm_is_transitioned_to(State state)
  {
    fsm->transition(state);
  }

  void given_that_the_fsm_has_been_transitioned_to(State state)
  {
    mock().disable();
    when_the_fsm_is_transitioned_to(state);
    mock().enable();
  }

  void when_signal_is_sent(uint8_t signal, const void* data)
  {
    fsm->send_signal(signal, data);
  }
};

TEST(Fsm, should_send_entry_to_the_initial_state)
{
  signal_should_be_sent_to_state(state_a, FsmSignal::entry, nullptr);
  when_the_fsm_is_initialized_with_state(state_a);
}

TEST(Fsm, should_send_exit_to_current_state_then_entry_to_new_state_during_transition)
{
  given_that_the_fsm_has_been_initialized_with_state(state_a);

  signal_should_be_sent_to_state(state_a, FsmSignal::exit, nullptr);
  then signal_should_be_sent_to_state(state_b, FsmSignal::entry, nullptr);
  when_the_fsm_is_transitioned_to(state_b);
}

TEST(Fsm, should_send_signals_to_the_current_state)
{
  given_that_the_fsm_has_been_initialized_with_state(state_a);
  signal_should_be_sent_to_state(state_a, signal_1, reinterpret_cast<const void*>(0x1234));
  when_signal_is_sent(signal_1, reinterpret_cast<const void*>(0x1234));

  given_that_the_fsm_has_been_transitioned_to(state_b);
  signal_should_be_sent_to_state(state_b, signal_2, reinterpret_cast<const void*>(0x4567));
  when_signal_is_sent(signal_2, reinterpret_cast<const void*>(0x4567));
}

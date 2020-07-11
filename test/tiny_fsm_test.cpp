/*!
 * @file
 * @brief
 */

#include <cstdint>
#include "tiny_fsm.h"
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

using namespace tiny;
using namespace std;

#define then

TEST_GROUP(tiny_fsm)
{
  static void dummy(uint8_t * context, uint8_t signal, const void* data)
  {
    (void)context;
    (void)signal;
    (void)data;
  };

  uint8_t context;
  Fsm<uint8_t> fsm{dummy, &context};

  enum
  {
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
      .withParameter("signal", (uint8_t)signal)
      .withParameter("data", data);
  }

  static void state_b(uint8_t * context, uint8_t signal, const void* data)
  {
    mock()
      .actualCall("state_b")
      .withParameter("context", context)
      .withParameter("signal", (uint8_t)signal)
      .withParameter("data", data);
  }

  void signal_should_be_sent_to_state(Fsm<uint8_t>::State state, uint8_t signal, const void* data)
  {
    (state == state_a ? mock().expectOneCall("state_a") : mock().expectOneCall("state_b"))
      .withParameter("context", &context)
      .withParameter("signal", (uint8_t)signal)
      .withParameter("data", data);
  }

  void when_the_fsm_is_initialized_with_state(Fsm<uint8_t>::State state)
  {
    fsm = Fsm<uint8_t>{state, &context};
  }

  void given_that_the_fsm_has_been_initialized_with_state(Fsm<uint8_t>::State state)
  {
    mock().disable();
    when_the_fsm_is_initialized_with_state(state);
    mock().enable();
  }

  void when_the_fsm_is_transitioned_to(Fsm<uint8_t>::State state)
  {
    fsm.transition(state);
  }

  void given_that_the_fsm_has_been_transitioned_to(Fsm<uint8_t>::State state)
  {
    mock().disable();
    when_the_fsm_is_transitioned_to(state);
    mock().enable();
  }

  void when_signal_is_sent(uint8_t signal, const void* data)
  {
    fsm.send_signal(signal, data);
  }
};

TEST(tiny_fsm, should_send_entry_to_the_initial_state)
{
  signal_should_be_sent_to_state(state_a, FsmSignal::entry, NULL);
  when_the_fsm_is_initialized_with_state(state_a);
}

TEST(tiny_fsm, should_send_exit_to_current_state_then_entry_to_new_state_during_transition)
{
  given_that_the_fsm_has_been_initialized_with_state(state_a);

  signal_should_be_sent_to_state(state_a, FsmSignal::exit, NULL);
  then signal_should_be_sent_to_state(state_b, FsmSignal::entry, NULL);
  when_the_fsm_is_transitioned_to(state_b);
}

TEST(tiny_fsm, should_send_signals_to_the_current_state)
{
  given_that_the_fsm_has_been_initialized_with_state(state_a);
  signal_should_be_sent_to_state(state_a, signal_1, (const void*)0x1234);
  when_signal_is_sent(signal_1, (const void*)0x1234);

  given_that_the_fsm_has_been_transitioned_to(state_b);
  signal_should_be_sent_to_state(state_b, signal_2, (const void*)0x4567);
  when_signal_is_sent(signal_2, (const void*)0x4567);
}

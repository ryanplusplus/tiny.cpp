/*!
 * @file
 * @brief
 */

#include <iterator>
#include "tiny/Hsm.hpp"
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

/*!
 * +---------------------+ +---+
 * | A                   | | F |
 * | +-------+ +-------+ | +---+
 * | | B     | | D     | |
 * | | +---+ | | +---+ | |
 * | | | C | | | | E | | |
 * | | +---+ | | +---+ | |
 * | +-------+ +-------+ |
 * +---------------------+
 */

using namespace tiny;
using namespace std;

enum State : uint8_t {
  state_a,
  state_b,
  state_c,
  state_d,
  state_e,
  state_f
};

#define state_function(_name)                                                      \
  static Hsm::Result _##_name(uint16_t* context, uint8_t signal, const void* data) \
  {                                                                                \
    bool result =                                                                  \
      mock()                                                                       \
        .actualCall(#_name)                                                        \
        .withParameter("context", context)                                         \
        .withParameter("signal", signal)                                           \
        .withParameter("data", data)                                               \
        .returnIntValue();                                                         \
                                                                                   \
    return result ? Hsm::Result::consumed : Hsm::Result::deferred;                 \
  }

// clang-format off
state_function(state_a)
state_function(state_b)
state_function(state_c)
state_function(state_d)
state_function(state_e)
state_function(state_f)
  // clang-format on

  static const char* state_names[] = {
    "state_a",
    "state_b",
    "state_c",
    "state_d",
    "state_e",
    "state_f",
  };

typedef Hsm::Result (*StateFunction)(uint16_t* context, uint8_t signal, const void* data);

static const StateFunction states[] = {
  _state_a,
  _state_b,
  _state_c,
  _state_d,
  _state_e,
  _state_f,
};

static const Hsm::StateDescriptor descriptors[] = {
  { reinterpret_cast<Hsm::State>(_state_a), Hsm::no_parent },
  { reinterpret_cast<Hsm::State>(_state_b), reinterpret_cast<Hsm::State>(_state_a) },
  { reinterpret_cast<Hsm::State>(_state_c), reinterpret_cast<Hsm::State>(_state_b) },
  { reinterpret_cast<Hsm::State>(_state_d), reinterpret_cast<Hsm::State>(_state_a) },
  { reinterpret_cast<Hsm::State>(_state_e), reinterpret_cast<Hsm::State>(_state_d) },
  { reinterpret_cast<Hsm::State>(_state_f), Hsm::no_parent },
};

static const Hsm::Configuration configuration = {
  descriptors,
  std::size(descriptors),
};

TEST_GROUP(Hsm)
{
  uint16_t context{};
  unique_ptr<Hsm> hsm{};

  enum {
    signal_1 = HsmSignal::user_start,
    signal_2
  };

  static constexpr Hsm::Result deferred{ Hsm::Result::deferred };
  static constexpr Hsm::Result consumed{ Hsm::Result::consumed };

  void setup()
  {
    mock().strictOrder();
  }

  void signal_should_be_sent_to_state(
    State state,
    uint8_t signal,
    const void* data = nullptr,
    Hsm::Result result = Hsm::Result::consumed)
  {
    mock()
      .expectOneCall(state_names[state])
      .withParameter("context", &context)
      .withParameter("signal", signal)
      .withParameter("data", data)
      .andReturnValue(static_cast<int>(result));
  }

  void when_the_hsm_is_initialized_with_state(State state)
  {
    hsm = make_unique<Hsm>(&context, configuration, states[state]);
  }

  void given_that_the_hsm_has_been_initialized_with_state(State state)
  {
    mock().disable();
    when_the_hsm_is_initialized_with_state(state);
    mock().enable();
  }

  void when_the_hsm_is_transitioned_to(State state)
  {
    hsm->transition(states[state]);
  }

  void given_that_the_hsm_has_been_transitioned_to(State state)
  {
    mock().disable();
    when_the_hsm_is_transitioned_to(state);
    mock().enable();
  }

  void when_signal_is_sent(uint8_t signal, const void* data)
  {
    hsm->send_signal(signal, data);
  }
};

TEST(Hsm, should_enter_the_initial_state_when_it_is_top_level)
{
  signal_should_be_sent_to_state(state_a, HsmSignal::entry);
  when_the_hsm_is_initialized_with_state(state_a);
}

TEST(Hsm, should_enter_the_initial_state_and_its_parents)
{
  signal_should_be_sent_to_state(state_a, HsmSignal::entry);
  signal_should_be_sent_to_state(state_b, HsmSignal::entry);
  signal_should_be_sent_to_state(state_c, HsmSignal::entry);
  when_the_hsm_is_initialized_with_state(state_c);
}

TEST(Hsm, should_send_a_signal_to_the_current_state)
{
  given_that_the_hsm_has_been_initialized_with_state(state_c);
  signal_should_be_sent_to_state(state_c, signal_1, reinterpret_cast<void*>(0x1234));
  when_signal_is_sent(signal_1, reinterpret_cast<void*>(0x1234));
}

TEST(Hsm, should_propagate_signals_when_they_are_not_consumed)
{
  given_that_the_hsm_has_been_initialized_with_state(state_c);
  signal_should_be_sent_to_state(state_c, signal_2, reinterpret_cast<void*>(0x5678), deferred);
  signal_should_be_sent_to_state(state_b, signal_2, reinterpret_cast<void*>(0x5678), deferred);
  signal_should_be_sent_to_state(state_a, signal_2, reinterpret_cast<void*>(0x5678), deferred);
  when_signal_is_sent(signal_2, reinterpret_cast<void*>(0x5678));
}

TEST(Hsm, should_stop_propagating_a_signal_once_consumed)
{
  given_that_the_hsm_has_been_initialized_with_state(state_c);
  signal_should_be_sent_to_state(state_c, signal_2, reinterpret_cast<void*>(0x5678), deferred);
  signal_should_be_sent_to_state(state_b, signal_2, reinterpret_cast<void*>(0x5678), consumed);
  when_signal_is_sent(signal_2, reinterpret_cast<void*>(0x5678));
}

TEST(Hsm, should_exit_and_reenter_state_during_a_self_transition)
{
  given_that_the_hsm_has_been_initialized_with_state(state_d);
  signal_should_be_sent_to_state(state_d, HsmSignal::exit);
  signal_should_be_sent_to_state(state_d, HsmSignal::entry);
  when_the_hsm_is_transitioned_to(state_d);
}

TEST(Hsm, should_transition_between_peers)
{
  given_that_the_hsm_has_been_initialized_with_state(state_b);
  signal_should_be_sent_to_state(state_b, HsmSignal::exit);
  signal_should_be_sent_to_state(state_d, HsmSignal::entry);
  when_the_hsm_is_transitioned_to(state_d);
}

TEST(Hsm, should_transition_to_parents)
{
  given_that_the_hsm_has_been_initialized_with_state(state_e);
  signal_should_be_sent_to_state(state_e, HsmSignal::exit);
  signal_should_be_sent_to_state(state_d, HsmSignal::exit);
  when_the_hsm_is_transitioned_to(state_a);
}

TEST(Hsm, should_transition_between_cousins)
{
  given_that_the_hsm_has_been_initialized_with_state(state_c);
  signal_should_be_sent_to_state(state_c, HsmSignal::exit);
  signal_should_be_sent_to_state(state_b, HsmSignal::exit);
  signal_should_be_sent_to_state(state_d, HsmSignal::entry);
  signal_should_be_sent_to_state(state_e, HsmSignal::entry);
  when_the_hsm_is_transitioned_to(state_e);
}

TEST(Hsm, should_update_state_during_a_transition)
{
  given_that_the_hsm_has_been_initialized_with_state(state_c);
  given_that_the_hsm_has_been_transitioned_to(state_e);
  signal_should_be_sent_to_state(state_e, signal_1, reinterpret_cast<void*>(0x1234));
  when_signal_is_sent(signal_1, reinterpret_cast<void*>(0x1234));
}

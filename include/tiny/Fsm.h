/*!
 * @file
 * @brief Simple implementation of a state chart that models functions as states.
 *
 * Signals are sent to the current state by invoking the state function with the
 * signal and associated data.
 *
 * When the FSM transitions to a new state, the exit signal is sent to the current
 * state, the current state is changed to the new state, then the entry signal is
 * sent to the new state.
 */

#ifndef Fsm_h
#define Fsm_h

#include <cstddef>
#include <cstdint>

namespace tiny {
  enum FsmSignal : uint8_t {
    entry,
    exit,
    user_start
  };

  class Fsm {
   public:
    typedef void (*State)(void* context, uint8_t signal, const void* data);

   public:
    template <typename Context>
    Fsm(void (*initial)(Context* context, uint8_t signal, const void* data), Context* context)
      : current(reinterpret_cast<State>(initial)), context(context)
    {
      this->current(context, FsmSignal::entry, nullptr);
    }

    auto send_signal(uint8_t signal, const void* data) -> void
    {
      this->current(this->context, signal, data);
    }

    template <typename Context>
    auto transition(void (*next)(Context* context, uint8_t signal, const void* data)) -> void
    {
      this->_transition(reinterpret_cast<State>(next));
    }

   private:
    auto _transition(State next) -> void;

   private:
    State current;
    void* context;
  };
}

#endif

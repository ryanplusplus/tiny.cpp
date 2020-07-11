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

#ifndef tiny_fsm_h
#define tiny_fsm_h

#include <cstddef>
#include <cstdint>

namespace tiny
{
  enum FsmSignal : std::uint8_t
  {
    entry,
    exit,
    user_start
  };

  template <typename T>
  class Fsm
  {
   public:
    typedef void (*State)(T* context, uint8_t signal, const void* data);

   public:
    Fsm(State initial, T* context)
      : current(initial), context(context)
    {
      this->current(context, FsmSignal::entry, NULL);
    };

    auto send_signal(uint8_t signal, const void* data) -> void
    {
      this->current(this->context, signal, data);
    }

    auto transition(State next) -> void
    {
      this->current(this->context, FsmSignal::exit, NULL);
      this->current = next;
      this->current(this->context, FsmSignal::entry, NULL);
    }

   private:
    State current;
    T* context;
  };
}

#endif

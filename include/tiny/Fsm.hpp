/*!
 * @file
 * @brief Simple implementation of a state machine that models functions as states.
 *
 * Signals are sent to the current state by invoking the state function with the
 * signal and associated data.
 *
 * When the FSM transitions to a new state, the exit signal is sent to the current
 * state, the current state is changed to the new state, then the entry signal is
 * sent to the new state.
 */

#ifndef tiny_Fsm_hpp
#define tiny_Fsm_hpp

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
    /*!
     * Initializes an FSM with the specified initial state. Sends the entry signal to the
     * initial state.
     */
    template <typename Context>
    Fsm(void (*initial)(Context* context, uint8_t signal, const void* data), Context* context)
      : current{ reinterpret_cast<State>(initial) }, context{ context }
    {
      current(context, FsmSignal::entry, nullptr);
    }

    Fsm(const Fsm&) = delete;

    void operator=(const Fsm& other) = delete;

    /*!
     * Sends a signal and optional signal data to the current state.
     */
    void send_signal(uint8_t signal, const void* data)
    {
      current(context, signal, data);
    }

    /*!
     * Transitions the FSM to the target state. Sends exit to the current state, changes
     * state, then sends entry to the target state.
     */
    template <typename Context>
    void transition(void (*next)(Context* context, uint8_t signal, const void* data))
    {
      _transition(reinterpret_cast<State>(next));
    }

   private:
    void _transition(State next);

   private:
    State current;
    void* context;
  };
}

#endif

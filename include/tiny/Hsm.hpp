/*!
 * @file
 * @brief Hierarchical version of a finite state machine.
 *
 * Differs from an FSM in that a state can have a parent state that can be used to share
 * behavior via a mechanism similar to inheritance. The parent-child relationship between
 * states impacts both signal handling and transitions.
 *
 * In order to explore how signal handling and transitions work in an HSM, consider the
 * below state machine:
 *
 * +---------------------+ +---+
 * | A                   | | F |
 * | +-------+ +-------+ | +---+
 * | | B     | | D     | |
 * | | +---+ | | +---+ | |
 * | | | C | | | | E | | |
 * | | +---+ | | +---+ | |
 * | +-------+ +-------+ |
 * +---------------------+
 *
 * States B and D are children of A. States C and E are children of B and D, respectively.
 * State F has no children and no parents.
 *
 * Signals are always sent first to the active state. The active state can choose whether
 * to consume the signal or to defer to its parent. If the state chooses to consume the
 * signal then signal handling ends with the state. If, however, the state chooses to
 * defer then the signal will then be sent to the state's parent. At this point the parent
 * must make the same decision. Signal handling ends when the state or one of its
 * ancestors consumes the signal or the list of ancestors is exhausted.
 *
 * Assume that the state C shown above is active and a signal is sent to the state
 * machine. State C will be the first state to receive this signal. If it chooses to
 * defer then the signal will be sent to state B, its direct parent. If state B also
 * chooses to defer then the signal will finally be sent to state A. If A chooses to defer
 * then signal handling will end because it has no parent state.
 *
 * When transitioning, exit signals are sent up the ancestor chain until reaching the
 * nearest common ancestor of the current and target states. Then, entry signals are sent
 * down the ancestor chain to the target state. The nearest common ancestor does not
 * receive an exit signal nor does it receive an entry signal. There is a special case
 * when the current and target states match (a self-transition). In this scenario the
 * current state will be sent an exit and then an entry signal.
 *
 * For example, if C is the current state and E is the target state, then the nearest
 * common ancestor is state A. This means that exit signals are sent to C and B and then
 * entry signals are sent to D and E.
 *
 * If B is the current state and F is the target state, then there is no nearest common
 * ancestor, so exit signals are sent to B and A and then an entry signal is sent to F.
 *
 * If C is the current state and the target state, this exercises the special case of a
 * self-transition so C will be sent an exit signal then an entry signal.
 *
 * @note Transitions can only be initiated when a transition is not active. This means
 * that transitions can never be triggered from an exit signal handler and they can only
 * be triggered from an entry signal handler if the current state is the target state
 * (thus there are not more entry signals that are going to be sent).
 */

#ifndef tiny_Hsm_hpp
#define tiny_Hsm_hpp

#include <cstddef>
#include <cstdint>

namespace tiny {
  enum HsmSignal : uint8_t {
    entry,
    exit,
    user_start
  };

  class Hsm {
   public:
    enum class Result : uint8_t {
      deferred,
      consumed
    };

    typedef Result (*State)(void* context, uint8_t signal, const void* data);

    static constexpr State no_parent = nullptr;
    static constexpr State top = nullptr;

    struct StateDescriptor {
      State state;
      State parent;
    };

    struct Configuration {
      const StateDescriptor* states;
      uint8_t state_count;
    };

   public:
    template <typename Context>
    Hsm(
      Context* context,
      const Configuration& configuration,
      Result (*initial)(Context* context, uint8_t signal, const void* data))
      : context{context},
        configuration{configuration},
        current{reinterpret_cast<State>(initial)}
    {
      send_entries(top, current);
    }

    Hsm(const Hsm&) = delete;

    void operator=(const Hsm& other) = delete;

    void send_signal(uint8_t signal, const void* data);

    template <typename Context>
    void transition(Hsm::Result (*next)(Context* context, uint8_t signal, const void* data))
    {
      _transition(reinterpret_cast<State>(next));
    }

   private:
    void _transition(State next);
    State parent_of(State child);
    uint8_t distance_between(State child, State parent);
    State nth_parent(State state, uint8_t n);
    void send_entries(State after, State to);
    void send_exits(State from, State before);
    State nearest_common_ancestor_of(State a, State b);

   private:
    void* context;
    const Configuration& configuration;
    State current;
  };
}

#endif

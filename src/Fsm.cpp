/*!
 * @file
 * @brief
 */

#include "tiny/Fsm.hpp"

using namespace tiny;

auto Fsm::_transition(State next) -> void
{
  current(context, FsmSignal::exit, nullptr);
  current = next;
  current(context, FsmSignal::entry, nullptr);
}

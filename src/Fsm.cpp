/*!
 * @file
 * @brief
 */

#include "tiny/Fsm.hpp"

using namespace tiny;

void Fsm::_transition(State next)
{
  current(context, FsmSignal::exit, nullptr);
  current = next;
  current(context, FsmSignal::entry, nullptr);
}

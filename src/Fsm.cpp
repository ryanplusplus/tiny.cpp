/*!
 * @file
 * @brief
 */

#include "tiny/Fsm.h"

using namespace tiny;

auto Fsm::_transition(State next) -> void
{
  current(context, FsmSignal::exit, nullptr);
  current = next;
  current(context, FsmSignal::entry, nullptr);
}

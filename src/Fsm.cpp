/*!
 * @file
 * @brief
 */

#include "tiny/Fsm.h"

using namespace tiny;

auto Fsm::_transition(State next) -> void
{
  this->current(this->context, FsmSignal::exit, nullptr);
  this->current = next;
  this->current(this->context, FsmSignal::entry, nullptr);
}

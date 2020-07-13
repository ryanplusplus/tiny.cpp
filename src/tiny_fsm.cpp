/*!
 * @file
 * @brief
 */

#include "tiny_fsm.h"

auto tiny::Fsm::_transition(State next) -> void
{
  this->current(this->context, FsmSignal::exit, NULL);
  this->current = next;
  this->current(this->context, FsmSignal::entry, NULL);
}

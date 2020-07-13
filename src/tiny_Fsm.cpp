/*!
 * @file
 * @brief
 */

#include "tiny_Fsm.h"

auto tiny::Fsm::_transition(State next) -> void
{
  this->current(this->context, FsmSignal::exit, nullptr);
  this->current = next;
  this->current(this->context, FsmSignal::entry, nullptr);
}

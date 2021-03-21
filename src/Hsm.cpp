/*!
 * @file
 * @brief
 */

#include "tiny/Hsm.h"

using namespace tiny;

auto Hsm::send_signal(uint8_t signal, const void* data) -> void
{
  auto current = this->current;

  while(current != top) {
    if(current(context, signal, data) == Result::consumed) {
      return;
    }
    current = this->parent_of(current);
  }
}

auto Hsm::_transition(State next) -> void
{
  if(this->current == next) {
    this->current(context, HsmSignal::exit, nullptr);
    this->current(context, HsmSignal::entry, nullptr);
  }
  else {
    auto nearest_common_ancestor = this->nearest_common_ancestor_of(this->current, next);
    this->send_exits(this->current, nearest_common_ancestor);
    this->current = next;
    this->send_entries(nearest_common_ancestor, next);
  }
}

auto Hsm::parent_of(State child) -> State
{
  for(uint8_t i = 0; i < this->configuration.state_count; i++) {
    if(this->configuration.states[i].state == child) {
      return this->configuration.states[i].parent;
    }
  }

  return top;
}

auto Hsm::distance_between(State child, State parent) -> uint8_t
{
  uint8_t distance = 0;
  auto current = child;

  while(current != parent) {
    distance++;
    current = this->parent_of(current);
  }

  return distance;
}

auto Hsm::nth_parent(State state, uint8_t n) -> State
{
  auto current = state;

  for(uint8_t i = 0; i < n; i++) {
    current = this->parent_of(current);
  }

  return current;
}

auto Hsm::send_entries(State after, State to) -> void
{
  if(after == to) {
    return;
  }

  for(uint8_t n = this->distance_between(to, after) - 1; n > 0; n--) {
    this->nth_parent(to, n)(context, HsmSignal::entry, nullptr);
  }

  to(context, HsmSignal::entry, nullptr);
}

auto Hsm::send_exits(State from, State before) -> void
{
  auto current = from;

  while(current != before) {
    current(context, HsmSignal::exit, nullptr);
    current = this->parent_of(current);
  }
}

auto Hsm::nearest_common_ancestor_of(State a, State b) -> State
{
  while(a != top) {
    auto bb = b;

    while(bb != top) {
      if(a == bb) {
        return a;
      }

      bb = this->parent_of(bb);
    }

    a = this->parent_of(a);
  }

  return top;
}

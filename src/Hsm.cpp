/*!
 * @file
 * @brief
 */

#include "tiny/Hsm.hpp"

using namespace tiny;

void Hsm::send_signal(uint8_t signal, const void* data)
{
  auto current = this->current;

  while(current != top) {
    if(current(context, signal, data) == Result::consumed) {
      return;
    }
    current = parent_of(current);
  }
}

void Hsm::_transition(State next)
{
  if(current == next) {
    current(context, HsmSignal::exit, nullptr);
    current(context, HsmSignal::entry, nullptr);
  }
  else {
    auto nearest_common_ancestor = nearest_common_ancestor_of(current, next);
    send_exits(current, nearest_common_ancestor);
    current = next;
    send_entries(nearest_common_ancestor, next);
  }
}

Hsm::State Hsm::parent_of(State child)
{
  for(uint8_t i = 0; i < configuration.state_count; i++) {
    if(configuration.states[i].state == child) {
      return configuration.states[i].parent;
    }
  }

  return top;
}

uint8_t Hsm::distance_between(State child, State parent)
{
  uint8_t distance = 0;
  auto current = child;

  while(current != parent) {
    distance++;
    current = parent_of(current);
  }

  return distance;
}

Hsm::State Hsm::nth_parent(State state, uint8_t n)
{
  auto current = state;

  for(uint8_t i = 0; i < n; i++) {
    current = parent_of(current);
  }

  return current;
}

void Hsm::send_entries(State after, State to)
{
  if(after == to) {
    return;
  }

  for(uint8_t n = distance_between(to, after) - 1; n > 0; n--) {
    nth_parent(to, n)(context, HsmSignal::entry, nullptr);
  }

  to(context, HsmSignal::entry, nullptr);
}

void Hsm::send_exits(State from, State before)
{
  auto current = from;

  while(current != before) {
    current(context, HsmSignal::exit, nullptr);
    current = parent_of(current);
  }
}

Hsm::State Hsm::nearest_common_ancestor_of(State a, State b)
{
  while(a != top) {
    auto bb = b;

    while(bb != top) {
      if(a == bb) {
        return a;
      }

      bb = parent_of(bb);
    }

    a = parent_of(a);
  }

  return top;
}

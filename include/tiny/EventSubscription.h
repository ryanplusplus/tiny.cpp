/*!
 * @file
 * @brief Manages the combination of an event callback and context for an event
 * subscriber.
 */

#ifndef tiny_EventSubscription_h
#define tiny_EventSubscription_h

#include "tiny/List.h"

namespace tiny {
  template <typename... Args>
  class EventSubscription final : public List::Node {
   public:
    typedef void (*Callback)(void* context, Args... args);

    template <typename Context>
    EventSubscription(Context* context, void (*callback)(Context*, Args...))
      : context(context),
        callback(reinterpret_cast<Callback>(callback))
    {
    }

    EventSubscription(const EventSubscription& other) = delete;

    auto operator=(const EventSubscription& other) -> void = delete;

    auto publish(Args... args) const -> void
    {
      callback(context, args...);
    }

   private:
    void* context;
    Callback callback;
  };
}

#endif

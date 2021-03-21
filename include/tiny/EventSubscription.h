/*!
 * @file
 * @brief Manages the combination of an event callback and context for an event
 * subscriber.
 */

#ifndef EventSubscription_h
#define EventSubscription_h

#include "tiny/List.h"

namespace tiny {
  template <typename Args>
  class EventSubscription {
   public:
    typedef void (*Callback)(void* context, const Args& args);

    EventSubscription() = delete;

    template <typename Context>
    EventSubscription(Context* context, void (*callback)(Context*, const Args&))
      : node(),
        context(reinterpret_cast<void*>(context)),
        callback(reinterpret_cast<Callback>(callback))
    {
    }

    auto publish(const Args& args) const -> void
    {
      this->callback(this->context, args);
    }

   private:
    List::Node node;
    void* context;
    Callback callback;
  };
}

#endif

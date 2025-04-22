/*!
 * @file
 * @brief Manages the combination of a callback and context for an event subscriber.
 */

#ifndef tiny_EventSubscription_hpp
#define tiny_EventSubscription_hpp

#include "tiny/List.hpp"

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

    explicit EventSubscription(void (*callback)(void*, Args...))
      : context(nullptr),
        callback(callback)
    {
    }

    EventSubscription(const EventSubscription& other) = delete;

    void operator=(const EventSubscription& other) = delete;

    /*!
     * Publishes to the event subscriber.
     */
    void publish(Args... args) const
    {
      callback(context, args...);
    }

   private:
    void* context;
    Callback callback;
  };
}

#endif

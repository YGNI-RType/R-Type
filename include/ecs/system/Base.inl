/*
** EPITECH PROJECT, 2024
** game-engine-headers
** File description:
** Base.inl
*/

namespace ecs::system {

    template<typename Derived>
    Base<Derived>::Base(): m_isRunning(true) {
        m_eventBus.subscribe<Derived, event::MainLoop>(*static_cast<Derived*>(this), &Derived::MainLoop);
    }

    template<typename Derived>
    template<typename EventType>
    void Base<Derived>::subscribeToEvent(void (Derived::*callbackMethod)(EventType&)) {
        m_eventBus.subscribe<EventType>([this, callbackMethod](EventType &e) {
            if (this->m_isRunning) {
                (static_cast<Derived*>(this)->*callbackMethod)(e);
            }
        });
    }

    template<typename Derived>
    void Base<Derived>::pause(void) {
        m_isRunning = false;
    }

    template<typename Derived>
    void Base<Derived>::resume(void) {
        m_isRunning = true;
    }

    template<typename BaseType>
    void register_system(event::Bus &eventBus) {
        BaseType system(eventBus);
    }
}

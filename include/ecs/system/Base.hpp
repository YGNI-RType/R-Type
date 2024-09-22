/*
** EPITECH PROJECT, 2024
** game-engine-headers
** File description:
** Base.hpp
*/

#pragma once

#include "ecs/system/event/Bus.hpp"
#include "ecs/system/event/Base.hpp"

namespace ecs::system {
    /**
     * @brief Base class for all systems in the game engine.
     *
     * This class provides a common interface for systems, including the ability to
     * subscribe to events and manage the running state of the system.
     *
     * @tparam Derived The derived class that inherits from this Base class.
     */
    template<typename Derived>
    class Base {
    public:
        /**
         * @brief Constructs a Base instance.
         *
         * Initializes the system with the event bus and sets the system to running.
         *
         * @param eventBus Reference to the event bus used for subscribing to events.
         */
        Base(event::Bus &eventBus);

        /**
         * @brief Virtual destructor for the Base class.
         */
        virtual ~Base() = default;

        /**
         * @brief Virtual method for handling the main loop event.
         *
         * This method is intended to be overridden by derived classes to handle
         * the main loop event.
         *
         * @param event The main loop event.
         */
        virtual void MainLoop(event::MainLoop &event) {}

        /**
         * @brief Subscribe to a specific event type with a callback method.
         *
         * @tparam EventType The type of event to subscribe to.
         * @param callbackMethod Pointer to the member function that handles the event.
         *
         * This method registers a callback for a specific event type and ensures that
         * the callback is only invoked if the system is running.
         */
        template<typename EventType>
        void subscribeToEvent(void (Derived::*callbackMethod)(EventType&));

        /**
         * @brief Pauses the system, preventing it from processing events.
         *
         * Sets the system's running state to false.
         */
        void pause(void);

        /**
         * @brief Resumes the system, allowing it to process events again.
         *
         * Sets the system's running state to true.
         */
        void resume(void);

    private:
        event::Bus &m_eventBus; ///< Bus used for subscribing to events.
        bool m_isRunning; ///< Flag indicating whether the system is currently running.
    };

    /**
     * @brief Registers a system with the event bus.
     *
     * Creates an instance of the specified system type and initializes it with the
     * event bus.
     *
     * @tparam BaseType The type of system to register.
     * @param eventBus Reference to the event bus.
     */
    template<typename BaseType>
    void register_system(event::Bus &eventBus);
}

#include "Base.inl"


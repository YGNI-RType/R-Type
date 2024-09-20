/*
** EPITECH PROJECT, 2024
** game-engine-headers
** File description:
** Bus.hpp
*/

#pragma once

#include <unordered_map>
#include <typeindex>
#include <functional>
#include <vector>
#include <memory>
#include "ecs/system/event/Callback.hpp"

namespace ecs::system::event {

    /**
     * @brief Event bus for managing and dispatching events to registered callbacks.
     *
     * The Event::Bus class maintains a registry of callbacks for different event types.
     * It provides methods for subscribing to events and publishing events to trigger
     * the associated callbacks.
     */
    class Bus {
    public:
        /**
         * @brief Publishes an event to all subscribed callbacks.
         *
         * This method searches for callbacks associated with the event type and
         * invokes them with the provided event.
         *
         * @tparam Type The type of the event to publish.
         * @param event The event to publish.
         */
        template<typename Type>
        void publish(const Type& event);

        /**
         * @brief Subscribes a member function callback to a specific event type.
         *
         * This method registers a member function of a class to be called when an
         * event of the specified type is published.
         *
         * @tparam T The type of the class containing the member function.
         * @tparam Type The type of the event to subscribe to.
         * @param instance The instance of the class containing the member function.
         * @param callbackMethod Pointer to the member function to be called.
         */
        template<typename T, typename Type>
        void subscribe(T& instance, void (T::*callbackMethod)(Type&));

        /**
         * @brief Subscribes a lambda function callback to a specific event type.
         *
         * This method registers a lambda function to be called when an event of the
         * specified type is published.
         *
         * @tparam Type The type of the event to subscribe to.
         * @param callback The lambda function to be called.
         */
        template<typename Type>
        void subscribe(std::function<void(Type&)> callback);

    private:
        std::unordered_map<std::type_index, std::vector<std::unique_ptr<Callback>>> m_callbacks; ///< Map of callbacks registered for each event type.
    };

}

#include "Bus.inl"

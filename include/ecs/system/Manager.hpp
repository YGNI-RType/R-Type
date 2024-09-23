/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** SystemManager.hpp
*/


#include "ecs/system/event/Bus.hpp"
#include "ecs/system/Base.hpp"
#include <typeindex>
#include <unordered_map>
#include <memory>
#include <any>

namespace ecs::system {
    class Manager {
        public:
            Manager(): m_eventBus() {}

            template <class T, typename ...Params>
            void register_system(Params &&...p) {
                m_systemTable[std::type_index(typeid(T))] = std::make_unique<T>(m_eventBus, std::forward(p)...);
            }

            template <class T>
            void pause_system(void) {
                auto it = m_systemTable.find(std::type_index(typeid(T)));

                if (it == m_systemTable.end())
                    throw "";

                (std::any_cast<Base<T>&>(it->second)).pause();
            }

            template <class T>
            void resume_system(void) {
                auto it = m_systemTable.find(std::type_index(typeid(T)));

                if (it == m_systemTable.end())
                    throw "";

                (std::any_cast<Base<T>&>(it->second)).resume();
            }

        private:
            std::unordered_map<std::type_index, std::any> m_systemTable;
            event::Bus m_eventBus;
    };
}

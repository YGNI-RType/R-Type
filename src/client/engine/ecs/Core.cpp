/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Core.cpp
*/

#include "Core.hpp"
#include <iostream>
namespace ecs {
    Core::Core()
    {
        // m_componentManager = std::make_unique<ComponentManager>();
		// m_entityManager = std::make_unique<EntityManager>();
		// m_EventManager = std::make_unique<EventManager>();
		// m_SystemManager = std::make_unique<SystemManager>();
    }

    void Core::deleteEntity(Entity entity)
    {
        for (auto& [typeIndex, anyArray] : m_componentArrays) {
            try {
                auto& sparseArray = std::any_cast<ComponentSparseArray<std::any>&>(anyArray);
                if (sparseArray.contains(entity)) {
                    sparseArray.erase(entity);
                }
            } catch (const std::bad_any_cast&) {
                // std::cout << "zut" << std::endl;
            }
        }
        destroyEntity(entity);
    }

    // template <typename Component>
    // void Core::registerComponent()
    // {
    //     m_componentManager->registerComponent<Component>();
    // }

    // Entity Core::createEntity()
    // {
    //     return m_entityManager->createEntity();
    // }

} // namespace ecs

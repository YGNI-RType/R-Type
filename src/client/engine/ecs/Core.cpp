#include "Core.hpp"

namespace ecs {
    Core::Core()
    {
        m_componentManager = std::make_unique<ComponentManager>();
		m_entityManager = std::make_unique<EntityManager>();
		// m_EventManager = std::make_unique<EventManager>();
		// m_SystemManager = std::make_unique<SystemManager>();
    }

    template <typename Component>
    void Core::registerComponent()
    {
        m_componentManager->registerComponent<Component>();
    }

    Entity Core::createEntity()
    {
        return m_entityManager->createEntity();
    }

} // namespace ecs

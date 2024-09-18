#include "client.hpp"
#include "shared.hpp"
#include <iostream>

// Test ECS
#include "Core.hpp"
#include "Transform.hpp"

int main()
{
    std::cout << "Client is running." << std::endl;
    sharedFunction();

    ecs::Core core;
    core.registerComponent<Transform>();
    ecs::Entity entity = core.createEntity();
    core.setComponent<Transform>(entity, Transform{ 1, 3 });
    core.setComponent<Transform>(entity, 1, 3);
    // core.registerSystem<Motion>()


    return 0;
}

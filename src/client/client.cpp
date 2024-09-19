#include "client.hpp"
#include "shared.hpp"
#include <iostream>

// Test ECS
// #include "Core.hpp"
// #include "ComponentSparseArray.hpp"
// #include "ComponentManager.hpp"
#include "Transform.hpp"
#include "Motion.hpp"
#include "Sprite.hpp"
#include "Core.hpp"

#include "SystemTest.hpp"

int main()
{
    std::cout << "Client is running." << std::endl;
    sharedFunction();

    // ecs::Core core;
    // core.registerComponent<Transform>();
    // ecs::Entity entity = core.createEntity();

    // core.setComponent<Transform>(entity, Transform{ 1, 3 });
    // core.setComponent<Transform>(entity, 1, 3);
    // core.registerSystem<Motion>()
    ecs::Core core;


    core.registerComponent<Transform>();
    core.registerComponent<Motion>();
    core.registerComponent<Sprite>();
    ecs::MovementSystem mov(core);
    ecs::DrawSystem draw(core);
    ecs::KillerSystem killer(core);

    // core.destroyEntity<Archetype>(player);



    // core.setComponent<Transform>(0, 0.0f, 0.0f);
    for (int i = 1; i < 5000; i++) {
        ecs::Entity player = core.spawnEntity(Transform{float(rand() % 1000), 0.0f},
        Motion{0.0f, float(rand() % 100 / 100.0f)},
        Sprite{sf::RectangleShape({50, 50}), sf::Color(rand())});
        // core.setComponent<Transform>(i, float(rand() % 1000), 0.0f);
        // core.setComponent<Motion>(i, 0.0f, float(rand() % 100 / 100.0f));
        // core.setComponent<Sprite>(i, sf::RectangleShape({50, 50}), sf::Color(rand()));
    }

    // for (int i = 1; i < 5000; i++) {
    //     core.setComponent<Transform>(i, float(rand() % 1000), 0.0f);
    //     core.setComponent<Motion>(i, 0.0f, float(rand() % 100 / 100.0f));
    //     core.setComponent<Sprite>(i, sf::RectangleShape({50, 50}), sf::Color(rand()));
    // }

    float time = std::clock();
    float newTime = std::clock();
    for (int i = 0; i < 5000; i++) {
        std::cout << std::endl << "Update " << i << " " << newTime - time << std::endl;
        mov.update((newTime - time) / 1000.0f);
        draw.update((newTime - time) / 1000.0f);
        killer.update((newTime - time) / 1000.0f);
        time = newTime;
        newTime = std::clock();
    }


    return 0;
}

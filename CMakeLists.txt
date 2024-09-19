cmake_minimum_required(VERSION 3.10)

project(R-Type VERSION 0.0.1)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the shared library
add_library(shared_rtype src/shared/shared.cpp)
target_include_directories(shared_rtype PUBLIC include/shared)

# Add the client executable
file(GLOB ENGINE_ECS_SOURCES src/client/engine/ecs/*.cpp)
add_executable(r-type_client src/client/client.cpp ${ENGINE_ECS_SOURCES})
target_include_directories(r-type_client PRIVATE include/client)
target_include_directories(r-type_client PRIVATE include/client/engine/ecs)
target_include_directories(r-type_client PRIVATE include/client/game/components)
target_link_libraries(r-type_client PRIVATE sfml-system sfml-window sfml-graphics sfml-network sfml-audio)
target_link_libraries(r-type_client PRIVATE shared_rtype)

# Add the server executable
add_executable(r-type_server src/server/server.cpp)
target_include_directories(r-type_server PRIVATE include/server)
target_link_libraries(r-type_server PRIVATE shared_rtype)

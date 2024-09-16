cmake_minimum_required(VERSION 3.10)

project(R-Type VERSION 0.0.1)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the shared library
add_library(shared_rtype src/shared/shared.cpp)
target_include_directories(shared_rtype PUBLIC include/shared)

# Add the client executable
add_executable(client src/client/client.cpp)
target_include_directories(client PRIVATE include/client)
target_link_libraries(client PRIVATE shared_rtype)

# Add the server executable
add_executable(server src/server/server.cpp)
target_include_directories(server PRIVATE include/server)
target_link_libraries(server PRIVATE shared_rtype)

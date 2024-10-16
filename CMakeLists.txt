cmake_minimum_required(VERSION 3.10)
project(rtype)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

include(cmake/automate-vcpkg-gengine.cmake)
vcpkg_bootstrap()
vcpkg_install_gengine()

include_directories(include)

file(GLOB_RECURSE SOURCES source/*.cpp)

add_executable(r_type-server ${SOURCES})
add_executable(r_type-client ${SOURCES})


find_package(GEngine CONFIG REQUIRED)
target_link_libraries(r_type-server PRIVATE GEngine)
target_link_libraries(r_type-client PRIVATE GEngine)

target_compile_definitions(r_type-server PRIVATE Server)
target_compile_definitions(r_type-client PRIVATE Client)

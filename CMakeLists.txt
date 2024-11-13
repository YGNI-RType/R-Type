cmake_minimum_required(VERSION 3.10)
project(rtype)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

if (DEFINED CMAKE_GENGINE_PATH)
    include(cmake/custom-gengine.cmake)
else()
    include(cmake/automate-vcpkg-gengine.cmake)
    vcpkg_bootstrap()
    vcpkg_install_gengine()
endif()

include_directories(include)

file(GLOB_RECURSE SOURCES source/*.cpp)

add_executable(r-type_server ${SOURCES})
add_executable(r-type_client ${SOURCES})

if (NOT DEFINED CMAKE_GENGINE_PATH)
find_package(GEngine CONFIG REQUIRED)
target_link_libraries(r-type_server PRIVATE GEngine)
target_link_libraries(r-type_client PRIVATE GEngine)
else()
target_link_libraries(r-type_server PRIVATE nlohmann_json::nlohmann_json stduuid ${GEngine_LIB})
target_link_libraries(r-type_client PRIVATE nlohmann_json::nlohmann_json stduuid ${GEngine_LIB})
endif()

target_compile_definitions(r-type_server PRIVATE GEngine_Server)
target_compile_definitions(r-type_client PRIVATE GEngine_Client)

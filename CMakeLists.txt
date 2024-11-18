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
    if(NOT VCPKG_SHOULD_NOT_UPDATE)
        vcpkg_install_gengine()
    endif()
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
target_link_libraries(r-type_server PRIVATE raylib nlohmann_json::nlohmann_json stduuid ${GEngine_LIB})
target_link_libraries(r-type_client PRIVATE raylib nlohmann_json::nlohmann_json stduuid ${GEngine_LIB})
endif()

target_compile_definitions(r-type_server PRIVATE GEngine_Server)
target_compile_definitions(r-type_client PRIVATE GEngine_Client)

# This is mostly for the CD pipeline, it uses the VCPKG installed Gengine
if (CMAKE_BUILD_TYPE STREQUAL "Release" AND NOT DEFINED CMAKE_GENGINE_PATH)
    install(TARGETS r-type_server r-type_client
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION bin
        ARCHIVE DESTINATION bin
    )

    if(UNIX)
        set_target_properties(r-type_server r-type_client PROPERTIES INSTALL_RPATH "$ORIGIN")
    endif()

    install(DIRECTORY assets/ DESTINATION assets)
    install(DIRECTORY config/ DESTINATION config)
    install(FILES r-type.json DESTINATION .)

    include(cmake/pack.cmake)
endif()

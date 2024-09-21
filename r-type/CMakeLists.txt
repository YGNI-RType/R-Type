cmake_minimum_required(VERSION 3.10)

project(R-Type)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

include_directories(../include)

find_library(GEngine_LIB NAMES GEngine PATHS ../)

file(GLOB_RECURSE CLIENT_SOURCES client/*.cpp)
file(GLOB_RECURSE SERVER_SOURCES server/*.cpp)
file(GLOB_RECURSE SHARED_SOURCES shared/*.cpp)

set(CLIENT_SOURCES client/main.cpp)
add_executable(r-type_client ${CLIENT_SOURCES} ${SHARED_SOURCES})
target_link_libraries(r-type_client PRIVATE ${GEngine_LIB})

set(SERVER_SOURCES server/main.cpp)
add_executable(r-type_server ${SERVER_SOURCES} ${SHARED_SOURCES})
target_link_libraries(r-type_server PRIVATE ${GEngine_LIB})

install(TARGETS r-type_client r-type_server
    RUNTIME DESTINATION bin
)

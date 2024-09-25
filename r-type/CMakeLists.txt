cmake_minimum_required(VERSION 3.10)

project(R-Type)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)


find_library(GEngine_LIB NAMES GEngine PATHS ../build/Debug ../build/Release ../build)

file(GLOB_RECURSE CLIENT_SOURCES client/*.cpp)
file(GLOB_RECURSE SERVER_SOURCES server/*.cpp)
file(GLOB_RECURSE SHARED_SOURCES shared/*.cpp)
file(GLOB_RECURSE HEADERS ../include/*.h)

include_directories(../include)

add_executable(r-type_client ${CLIENT_SOURCES} ${SHARED_SOURCES} ${HEADERS})
target_link_libraries(r-type_client PRIVATE ${GEngine_LIB})

add_executable(r-type_server ${SERVER_SOURCES} ${SHARED_SOURCES} ${HEADERS})
target_link_libraries(r-type_server PRIVATE ${GEngine_LIB})

install(TARGETS r-type_client r-type_server
    RUNTIME DESTINATION bin
)

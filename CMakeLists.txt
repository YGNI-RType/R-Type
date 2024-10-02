cmake_minimum_required(VERSION 3.10)

project(GEngine)

# find_package(raylib REQUIRED)

file(GLOB_RECURSE SOURCES source/*.cpp)

list(FILTER SOURCES EXCLUDE REGEX ".*/deprecated/.*")

file(GLOB_RECURSE HEADERS include/*.h)

list(FILTER HEADERS EXCLUDE REGEX ".*/deprecated/.*")

include_directories(include)


add_library(GEngine         SHARED ${SOURCES} ${HEADERS})
add_library(GEngine_static  STATIC ${SOURCES} ${HEADERS})

if(WIN32)
  target_link_libraries(GEngine wsock32 ws2_32)
  target_link_libraries(GEngine_static wsock32 ws2_32)
endif()

set(CMAKE_DEBUG_POSTFIX d)
set_target_properties(GEngine PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
    WINDOWS_EXPORT_ALL_SYMBOLS ON
    VERSION 0.0.1
)
set_target_properties(GEngine_static PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
    VERSION 0.0.1
    OUTPUT_NAME "GEngine-s"
)

target_link_libraries(GEngine raylib)
target_link_libraries(GEngine_static raylib)

install(TARGETS GEngine GEngine_static
LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin # Windows
)
install(DIRECTORY include/ DESTINATION include)

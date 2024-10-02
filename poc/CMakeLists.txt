cmake_minimum_required(VERSION 3.10)

project(hagar-i-oop)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

# Inclure les répertoires d'en-têtes
include_directories(../include/)
include_directories(../include/GEngine/)
include_directories(include/)
include_directories(/opt/homebrew/Cellar/raylib/5.0/include/)

# Trouver les fichiers source
file(GLOB_RECURSE SOURCES source/*.cpp)

# Ajouter l'exécutable
add_executable(hagar-i-oop ${SOURCES})

# Trouver la bibliothèque GEngine
find_library(GEngine_LIB NAMES GEngine PATHS ../build/Debug ../build/Release ../build)

# Lier les bibliothèques et les frameworks nécessaires
target_link_libraries(hagar-i-oop PRIVATE ${GEngine_LIB})

# Ajouter les frameworks macOS nécessaires
# target_link_libraries(hagar-i-oop PRIVATE
#     "-framework CoreVideo"
#     "-framework IOKit"
#     "-framework Cocoa"
#     "-framework GLUT"
#     "-framework OpenGL"
# )

# Installer l'exécutable
install(TARGETS hagar-i-oop
    RUNTIME DESTINATION bin
)

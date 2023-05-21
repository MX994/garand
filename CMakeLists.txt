cmake_minimum_required(VERSION 3.20)
project(garand
    VERSION 0.0.1
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)

# Requirements:
# - fmt
# - SDL2
# - SDL2PP
# - imgui

# vcpkg install sdl2pp fmt sdl2 sdl2-image sdl2-mixer imgui[sdl2-binding,sdl2-renderer-binding]
find_package(fmt REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(SDL2PP REQUIRED)
find_package(SDL2_mixer CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
include(FetchContent)
FetchContent_Declare(
  memoryview
  GIT_REPOSITORY https://github.com/ocornut/imgui_club.git
  GIT_TAG        ea49dd3c6803088d50b496e3fe981501543b7cbc
)
FetchContent_Populate(memoryview)
file(GLOB sources src/simulator/*.cpp src/simulator/src/*.cpp)

add_executable(garand ${sources})
target_include_directories(garand
    PRIVATE ${SDL2PP_INCLUDE_DIRS}
    PRIVATE ${memoryview_SOURCE_DIR}/imgui_memory_editor
    PRIVATE src/simulator/include)
target_link_libraries(garand
    PRIVATE
    fmt::fmt
    SDL2::SDL2main
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
    $<IF:$<TARGET_EXISTS:SDL2_mixer::SDL2_mixer>,SDL2_mixer::SDL2_mixer,SDL2_mixer::SDL2_mixer-static>
    $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>
    ${SDL2PP_LIBRARIES}
    imgui::imgui
)
cmake_minimum_required(VERSION 3.11)

project(Breakout C CXX)

# Set C and C++ standards
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Enable FetchContent module to download raylib
include(FetchContent)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG        master
)
FetchContent_MakeAvailable(raylib)

FetchContent_Declare(
    raygui
    GIT_REPOSITORY https://github.com/raysan5/raygui.git
    GIT_TAG        master
)
FetchContent_MakeAvailable(raygui)

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable(Breakout ${SRC_FILES})

target_link_libraries(Breakout PRIVATE raylib)
target_include_directories(Breakout PRIVATE ${raygui_SOURCE_DIR}/src)
target_include_directories(Breakout PRIVATE "${CMAKE_SOURCE_DIR}/include")

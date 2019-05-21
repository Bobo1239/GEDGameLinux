# Visual Studio doesn't ship with a higher version
cmake_minimum_required(VERSION 3.12)

if(UNIX)
    set(CMAKE_CXX_COMPILER wineg++)
    # Need the gnu-extension for `typeof` which is used by winelib for `__wine__uuid`
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++17")
    # Needed for DirextXMath
    add_compile_options(-mavx)
endif()

project(GEDGame LANGUAGES CXX)

# Add vendored libraries
add_subdirectory(vendor)

# Enable stricter warnings
if(MSVC)
    add_compile_options(/W4)
    # Unfortunately Visual Studio Code doesn't keep the console open after the program ends (even after setting SUBSYSTEM:CONSOLE)
    # This is fixed in Visual Studio 2019: https://developercommunity.visualstudio.com/solutions/478034/view.html
else()
    add_compile_options(-Wall -Wextra -pedantic -Wconversion)
endif()

# Main game

add_executable(GEDGame
    GEDGame/src/Game.cpp
    GEDGame/src/Terrain.cpp
)
target_include_directories(GEDGame PUBLIC GEDGame/include)
target_link_libraries(GEDGame DirectXMath DXUT FX11)
target_link_libraries(GEDGame d3dcompiler)
if (NOT UNIX)
    set_target_properties(GEDGame PROPERTIES CXX_STANDARD 17)
    set_target_properties(GEDGame PROPERTIES CXX_STANDARD_REQUIRED ON)
endif()

# TODO: Find a better way to do this
configure_file(GEDGame/game.cfg game.cfg COPYONLY)
configure_file(GEDGame/shader/game.fx shader/game.fx COPYONLY)
configure_file(GEDGame/media/UI/Font.dds media/UI/Font.dds COPYONLY)

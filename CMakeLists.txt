cmake_minimum_required(VERSION 3.16)
project(MyGame CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# System SDL2 (installed via brew / apt / vcpkg). Contributors install these themselves.
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)

add_executable(MyGame
    src/Main.cpp
    src/core/Game.cpp
    src/core/World.cpp
    src/entities/Entity.cpp
    src/entities/Character.cpp
    src/entities/Weapons.cpp
    src/entities/Items.cpp
    src/graphics/Graphics.cpp
    src/map/Map.cpp
    src/map/Algo.cpp
)

target_include_directories(MyGame PRIVATE src)

target_compile_options(MyGame PRIVATE -Wall)

target_link_libraries(MyGame PRIVATE
    SDL2::SDL2
    SDL2::SDL2main
    SDL2_image::SDL2_image
    SDL2_ttf::SDL2_ttf
    SDL2_mixer::SDL2_mixer
)

# The game loads assets with paths relative to the working directory (Img/, MyMap/, sound/).
# Mirror those directories next to the built binary so it runs from the build tree.
foreach(asset_dir Img MyMap sound)
    add_custom_command(TARGET MyGame POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/${asset_dir}
            $<TARGET_FILE_DIR:MyGame>/${asset_dir}
        COMMENT "Copying ${asset_dir}/ next to MyGame binary"
    )
endforeach()

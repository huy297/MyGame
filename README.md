# Survival Game

## Overview
Welcome to **Survival Game**, a thrilling survival experience built using C++ and the SDL2 library. In this game, players must navigate a hostile environment, gather resources, and fend off dangers to survive as long as possible.

## How to Play

### Controls
- **Movement**: Use the `WASD` keys to move your character.
- **Switch Weapons**: Press `1`, `2`, or `3` to switch between weapons:
  - `1` for the sword
  - `2` for the gun
  - `3` for grenades
- **Attack**: Press `Space` to attack.

### Items
- **Ammo**: Collect ammo to reload your gun.
- **HP**: Pick up HP items to increase your health.
- **Grenades**: Find grenades to add to your grenade count.

## Building & Running

The build is CMake-based (C++17) and links system SDL2, SDL2_image, SDL2_ttf, and SDL2_mixer via `find_package`. You install those libraries yourself first (see your OS below), then the same three CMake commands build and run the game everywhere.

### macOS

```sh
brew install cmake sdl2 sdl2_image sdl2_ttf sdl2_mixer

cmake -B build
cmake --build build
./build/MyGame
```

### Linux (Debian / Ubuntu)

```sh
sudo apt update
sudo apt install cmake build-essential libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev

cmake -B build
cmake --build build
./build/MyGame
```

On other distros, install the equivalent `cmake`, a C++17 compiler, and the SDL2/SDL2_image/SDL2_ttf/SDL2_mixer dev packages from your package manager (e.g. `dnf install cmake gcc-c++ SDL2-devel SDL2_image-devel SDL2_ttf-devel SDL2_mixer-devel` on Fedora, `pacman -S cmake sdl2 sdl2_image sdl2_ttf sdl2_mixer` on Arch), then run the same `cmake -B build && cmake --build build && ./build/MyGame`.

### Windows

The simplest path is [vcpkg](https://github.com/microsoft/vcpkg) to supply SDL2, plus CMake and a C++17 compiler (Visual Studio 2022's "Desktop development with C++" workload, or MinGW-w64).

```powershell
git clone https://github.com/microsoft/vcpkg
.\vcpkg\bootstrap-vcpkg.bat
.\vcpkg\vcpkg install sdl2 sdl2-image sdl2-ttf sdl2-mixer

cmake -B build -DCMAKE_TOOLCHAIN_FILE=.\vcpkg\scripts\buildsystems\vcpkg.cmake
cmake --build build
```

Run the built executable — with the default Visual Studio generator that's `.\build\Debug\MyGame.exe` (or `.\build\Release\MyGame.exe` for a `--config Release` build); with a single-config generator like Ninja or MinGW Makefiles it's `.\build\MyGame.exe`.

### Notes
- There's no install step: the game loads assets (`Img/`, `MyMap/`, `sound/`) relative to its working directory, and the build copies those three folders next to the binary automatically after every build.
- Run the binary from a terminal/shell, not by double-clicking, if you want to see its log output (asset loading, SDL errors, etc).

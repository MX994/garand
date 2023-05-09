# Garand
Custom architecture for COMPSCI 535.

# Documentation
The documentation for the architecture can be found [here](https://github.com/Tensor497/garand-docs).

TODO: Add more details on how to invoke the assembler.

# Setup
## Prerequisites
- [CMake]()
- [vcpkg](https://github.com/microsoft/vcpkg#getting-started)

First, clone the repository.
```
git clone git@github.com:Tensor497/garand.git
cd garand
```

Afterwards, run the below commands depending on the system.

### Windows
```
vcpkg install fmt sdl2 sdl2-image sdl2-mixer imgui[sdl2-binding,sdl2-renderer-binding] sdl2pp
```

### Linux
```
vcpkg install fmt "sdl2[wayland,x11]" sdl2-image sdl2-mixer "imgui[sdl2-binding,sdl2-renderer-binding]" sdl2pp --recurse
```

__Save the path that it gives you for `vcpkg`. It is important.__

Once you are done, make sure you are in the project root directory, and run the following command to build:
```
cmake . -DCMAKE_TOOLCHAIN_FILE=`<vcpkg_path>`
```

If you are using the CMake extension in Visual Studio Code, you can set the CMake settings as follows:
```JSON
"cmake.configureSettings": {
    "CMAKE_TOOLCHAIN_FILE" : "<vcpkg_path>/scripts/buildsystems/vcpkg.cmake"
},
```

From there, you can use the Command Palette to build.

If the build was successful, you should have two executable files in `build`: one for the simulator, and one for the assembler.

# Using
TODO.
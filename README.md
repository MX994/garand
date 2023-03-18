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

Afterwards, run the following command in your terminal:
```
vcpkg install imgui[sdl2-binding,sdl2-renderer-binding]
```

If you are on Linux and are having issues with this command, try the following:
```
vcpkg install "imgui[sdl2-binding,sdl2-renderer-binding]"
```

__Save the path that it gives you for `vcpkg`. It is important.__

Once you are done, make sure you are in the project root directory, and run the following command to build:
```
cmake . -DCMAKE_TOOLCHAIN_FILE=`<vcpkg_path>`

```

If you are using the CMake extension in Visual Studio Code, you can set the CMake settings as follows:
```JSON
"cmake.configureSettings": {
    "CMAKE_TOOLCHAIN_FILE" : "/opt/vcpkg/scripts/buildsystems/vcpkg.cmake"
},
```

From there, you can use the Command Palette to build.

If the build was successful, you should have two executable files in `build`: one for the simulator, and one for the assembler.

# Using
TODO.
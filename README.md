# Garand
Custom architecture for COMPSCI 535.

## Documentation

The documentation for the architecture can be found [here](https://github.com/Tensor497/garand-docs).

Assembler for assembling code can be found [here](https://github.com/Tensor497/garand-as).

## Setup
### Prerequisites
- [CMake]()
- [vcpkg](https://github.com/microsoft/vcpkg#getting-started)
- [Git]()

First, clone the repository.
```
git clone git@github.com:Tensor497/garand.git
cd garand
```

Afterwards, run the below commands depending on the system.

#### Windows
```
vcpkg install fmt sdl2 sdl2-image sdl2-mixer imgui[sdl2-binding,sdl2-renderer-binding] sdl2pp
```

#### Linux

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

## Usage

## Troubleshoot

### Nothing is rendered.

-   This is a GUI program. Make sure your window manager works and you can
    see the display.
-   Make sure `sdl2` is compiled with proper graphic backend that is supported
    by your system and OS. For instance, if you are using vcpkg on Linux, try 
    installing `sdl2[wayland,x11]`. 

### This simulator is so slow.

-   By default, we accurately render every single cycle of the processor.
    If your program is slow, you can try using `Skipping` feature in "Features"
    window, which allow skipping frame. For example, setting it to 1000
    will cause the processor cycle 1001 times before rendering the next frame.
    Breakpoint still work and pause the program early.
-   Technically, the processor cycle function is called in every ImGUI frame.
    You may try increasing the framerate by setting `io.DeltaTime`.
    For example: `io.DeltaTime = 1.f / 1000.f;` will set update rate of ImGUI
    to at least 1ms.

### I can't quit.

-   Press `ESC`

### I want custom cache parameters.

-   Open `src/simulator/include/Memory.hpp`, you can see
    cache is constructed based on three compile-time parameter:
    `AddressSize` type, `CACHE_BLOCK_INDEX_BIT`, `CACHE_BLOCK_OFFSET_BIT`.
    Change it to what you like, the algorithm works fine as long as there
    are no unaligned accesses.

### My compiler/linker/builder generates error/warning/crash X

-   Create a new issue. Attach all output from compiler/linker. Describe
    the environment (OS, compiler version, ...). Optionally,
    try including a screenshot.

### Where can I find sample code?

-   Demo code are stored in
    [garand-as](https://github.com/Tensor497/garand-as) repo,
    in `examples/` folder.


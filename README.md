# README



## Dependency

OpenGL



## Build

The project is built using CMake

There are two ways of doing this

#### Build Using Visual Studio with CMake (Recommended)

Open the root directory with Visual Studio
VS will automatically run CMake, configure the project and generate CMake cache using CMakeLists.txt.
You can see this on the "Output" panel
After the CMake finishes its generation, you can select the build target "renderme.exe" on the upper tools panel

#### Build Using CMake

Run the subsequent shell commands at project root directory:

```shell
mkdir build
cd build
cmake ..
```
CMake will generate corresponding project according to your platform



## File Structure

- **/bin:** binary output files, project output is **"renderme.exe"**
- **/data:** .obj data files
- **/config:** .json config files
- **/doc:** documents, project report **"path-tracing.md"** is here
- **/external:** external dependency source files
- **/lib:** library output
- **/result:** path tracing image output path
- **/src:** source files, path-tracing source files are **"/src/integrators/path-tracer.hpp"** and **"/src/integrators/path-tracer.cpp"**



## How To Use

Just try try and you will know how to use renderme.

#### Run Specific Scene

renderme will run /config/stairscase.json by default

You can specify config path in command-line parameter

```shell
# Run at project root path
./bin/renderme.exe /config/cornell-box.json
```

#### Path Tracing Mode
Select **Path Tracing** at CONFIG menu under **"Renderme Config"**

#### Camera Control
- **W:** move front
- **A:** move left
- **S:** move back
- **D:** move right
- **Left CTRL:** move down
- **SPACE:** move up
- **MOUSE with RIGHT MOUSE BUTTON down:** yaw and pitch

#### Scene Switching
Use the Slider named **"Scene"** at CONFIG menu under **"Renderme Config"**

#### Scene Information
You can see the Scene information including the number of vertices and faces at CONFIG menu under **"Scenes Config"**

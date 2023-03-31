# README



## Dependency

OpenGL



## Build

The project is built using CMake.

There are two ways of doing this.

#### Build Using Visual Studio with CMake (Recommended)

Open the root directory with Visual Studio. 
VS will automatically run CMake, configure the project and generate CMake cache using CMakeLists.txt. 
You can see this on the "Output" panel.
After the CMake finishes its generation, you can select the build target "renderme.exe" on the upper tools panel.

#### Build Using CMake

Run the subsequent commands at root directory using Shell:

```shell
mkdir build
cd build
cmake ..
```
CMake will generate corresponding project according to your platform.



## File Structure

- **/bin:** binary output files, project output is **"renderme.exe"**
- **/data:** .obj data files
- **/config:** .json config files
- **/doc:** documents, project report **"path-tracing.md"** is here
- **/external:** external dependency source files
- **/lib:** library output
- **/src:** source files, path-tracing source files are **"/src/integrators/path-tracer.hpp"** and **"/src/integrators/path-tracer.cpp"**



## How To Use

Just try try and you will know how to use renderme.

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

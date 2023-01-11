# README

## Dependency

OpenGL

## Building

The project is built using CMake. There are two ways of doing this.

#### 1. Build Using Visual Studio with CMake (Recommended)

Open the root directory with Visual Studio. 
VS will automatically run CMake, configure the project and generate CMake cache using CMakeLists.txt. 
You can see this on VS "Output" panel.
After the CMake finishes its generation. 
You can select the build target "renderme.exe" on the upper tools panel.

#### 2. Build Using CMake

Run the subsequent instructions at root directory using Shell:

```shell
mkdir build
cd build
cmake ..
```
CMake will generate corresponding project according to your platform.


## File Structure

- **bin:** binary output files, project output is **"renderme.exe"**
- **data:** .obj data files
- **doc:** documents, project report **"ZBuffer.md"** is here
- **external:** external dependency source files
- **lib:** library output
- **src:** source files, ZBuffer source files are **"src/integrators/zbuffer.hpp"** and **"src/integrators/zbuffer.cpp"**

## How To Use

Just try try and you will know how to use renderme.

#### ZBuffer Mode
Select **ZBuffer** at CONFIG menu under **"Renderme Config"**

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

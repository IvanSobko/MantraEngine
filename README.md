# Mantra Engine

A small side project created to explore and learn the more about game development and engine architecture.
Plenty more to come!



## Setup

### On Linux:

#### Clone and init submodules
```
git clone https://github.com/IvanSobko/MantraEngine.git
cd MantraEngine
git submodule update --init
```
#### Install dependencies:
```
sudo apt-get install build-essential gdb # gcc compilers, debugger
sudo apt-get install cmake

sudo apt install libgl-dev # OpenGL
sudo apt install libwayland-dev libxkbcommon-dev xorg-dev # for glfw
```

### On Windows

```
TODO
```


## External libraries and dependencies

- GL family: OpenGl, glfw, glad, glm

- spdlog
- imgui


## Notes and reminders for future self

- MantraCore built as shared library, linked to client app (game, see SandboxApp)
- support multiple render and window apis, therefore abstraction!
- currently implemented only glfw support
- imgui as a main UI

### Rendering

Idea is to have API and platform agnostic renderer and render primitives, so that it would be easy to switch apis (e.g.from opengl to vulcan, metal(?), directx).

| **Renderer (Platform Agnostic)**   | **Render API (API Agnostic)**  |
|:-----------------------------------|:-------------------------------|
| 2D & 3D Renderer                    | Context                       |
| Scene Graph                        | Swap Chain                     |
| Sorting (what to render, when)     | Frame, Vertex, Index Buffers   |
| Culling                            | Texture                        |
| Materials                          | Shader                         |
| LOD                                | Render Pass                    |
| Animation                          | State(s)                       |
| Camera                             | Pipeline                       |
| VFX                                |                                |
| PostFX                             |                                |
| Other (e.g. reflections, AO)       |                                |

### General usage
- To avoid dependency issues, please compile and run on ArchLinux (or inside prepared Docker container, see below)
- `c.sh` is a wrapper script to compile the program using provided Makefile
- Best way to compile: `./c.sh [default Blender scene].blend [output].bmp 1` (run with GUI) or `./c.sh .[default Blender scene].blend [output].bmp` (headless)
- Example: `./c.sh ./scene/multilight.blend output.bmp 1`
- `[default Blender scene].blend` is a Blender scene that is used when running in headless mode, or a default selection of Blender scene in GUI mode
- `[output].bmp` - output will be save to this file
- last parameter (`1`) defines whether to open a GUI or run in headless mode

### GUI mode
- `./c.sh [default Blender scene].blend [output].bmp 1`
- Choose a scene, setup parameters and click `Render` or `Exit`
- Parameters overwrite those defined in Blender scene
- When rendering spawns SDL live window window to see rendering progress
- Saves an image when user presses `Exit`
- Respects any updates made to Blender scenes between opening a program and clicking `Render`

### Headless mode
- `./c.sh [default Blender scene].blend [output].bmp`
- Parameters are loaded from the Bender file directly
- Can be used with `./anim.sh` to create animations
- Does not require any GUI interactions, does not open the GUI
- `Program parameters` -> Blender parameters mappings:
  - Scene settings:
    - **MUST USE EEVEE rendering engine**
    - `antialiasing` -> Output -> Resolution Scale %
    - `background` -> World -> Color value
    - `max depth` -> Render -> Viewport Samples
    - `threads` -> Render -> Render Samples
    - `resolution x/y` -> Output -> Resolution x/y
    - `shade smooth` -> Render -> Viewport Denoising
    - `sensor spacing` -> Render -> Volumetrics -> Distribution
    - `sensor size` -> Render -> Volumetrics -> Samples
  - Lights:
    - Add a point light
    - `diffuse` -> light data -> color * diffuse
    - `specular` -> light_data -> color * specular
  - Objects:
    - Add an object composed of triangles / quads
    - **ALL OBJECTS NEED TO HAVE A MATERIAL**
    - `refraction` = Blender's alpha material variable, where exactly 1.0 is no refraction and -x translates to x in terms of our refractivity factor, so -1.2 alpha would be 1.2 refraction
    - `diffuse` = rgb color
    - `specular` = specular rgb color
    - `shininess` = roughness (not proportional mapping but works)
    - **FOR LENSES** set specular and diffuse color to pure black and refractive index - alpha to e.g. -1.2
- see `./scene` for example usage

### External Libraries (+features)
- Implemented multithreading using the `thread.h` class
- stb for saving output as BMP (`stb_image_write.h`)
- RapidJSON for reading json parameters (`document.h`, `filereadstream.h`)
- SDL for displaying the raytracing live in a window
- glm for vector math
- imgui for interactive settings window (`./imgui`)
- python and Blender for API interactions and scene setup: `python-bpy`
- (ffmpeg for stitching together animations)

### Native compilation
- Requires Linux distribution: Archlinux
- packages with version equals (most probably also above):
  - `sdl2 2.30.0-1`
  - `sdl2_image 2.8.2-4`
  - `rapidjson 1.1.0-5`
  - `glm 1.0.0-3`
  - `python 3.11.8-1`
  - `gcc 13.2.1-5`
  - `binutils 2.42-2`
  - `make 4.4.1-2`
  - `Blender 17:4.1.0-3`
  - (`sudo pacman -Sy sdl2 sdl2_image rapidjson glm python gcc binutils make ccze Blender ffmpeg`)

### Running in Docker
- Project has a prepared dockerfile that includes an interactive docker container with preinstalled all dependencies for running the program as well as compiling the program in a system agnostic manner
- You need to install docker on you system (follow official guide: `https://docs.docker.com/desktop/`), ensure you can correctly execute `docker ps` and run a sample hello world container beforehand (`https://www.geeksforgeeks.org/docker-hello-world/`)
- for `ARM64 / Aarch64` based system (M1/M2/M3... Mac)
  - go to `./arch-vnc-arm/` and execute `bash spawn_no_persist.sh`
  - when container is deployed open `localhost:6080`, default login is `password`
  - to stop container `bash stop_no_persist.sh`
  - to start it again `bash start_no_persist.sh`
  - you can also use persist version (preserves state across deployments) but depending on your system you can run into permisions synchronization issues
- for `x86-64` based system (Windows, any Linux distribution)
  - go to `./arch-vnc/` and execute `bash spawn_no_persist.sh`
  - when container is deployed open `localhost:6080`, default login is `password`
  - to stop container `bash stop_no_persist.sh`
  - to start it again `bash start_no_persist.sh`
  - you can also use persist version (preserves state across deployments) but depending on your system you can run into permisions synchronization issues
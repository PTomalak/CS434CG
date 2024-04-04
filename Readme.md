### General
- please compile and run on Linux
- `c.sh` is a wrapper script to compile the program
- best way to compile: `./c.sh cornell.json output.bmp 1` (run SDL) or `./c.sh cornell.json output.bmp 0` (no SDL)
- can compile by running `bash c.sh [input]` with applicable `[input]` or see Makefile for more options (`all clean run clear`)
- Implemented multithreading using the `thread` class instead of OpenMP to implement interactive live SDL window in additional thread (without SDL, OpenMP implementation was trivial with `#pragma omp parallel for`)

### RUNNING ON LINUX (/Mac?)
- If possible, compile main version of the program with installed:
  - sdl2 2.30.0-1
  - rapidjson 1.1.0-5
  - (on my system: `sudo pacman -Sy sdl2 rapidjson`, should not be hard on other distros / mac)


### External Libraries
- stb for saving output as BMP (`stb_image_write.h`)
- RapidJSON for reading json parameters (`document.h`, `filereadstream.h`)
- SDL for displaying the raytracing live in a window (optional in context of the assignment)
- glm for vector math

### Running base program
- `./ray (input file) (output file .bmp) [optional flag]`
- optional flag, if present, creates and SDL window, otherwise skips creating a window
- example usage, SDL live preview: `./ray cornell.json output.bmp 1`
- example usage, no live preview: `./ray cornell.json output.bmp`

### EC
- Using <threads> to speed up calculation on multiple CPU cores. (+5 points)
  - find `THREADS` settings in the .json specification, always use >=2 threads as one is used for SDL, so for rendering is dedicated N-1 threads
- Made an animation by providing different scene descriptions with moving objects. (+5 points)
  - see `./animation`
- Added antialiasing by regular supersampling. (+5 points)
  - find `ANTIALIAS` settings in the .json specification

### SDL 
- SDL window with live rendering of the scene
- quit the window with `q`
- refresh the window with `r`, can also modify input file in the meantime



### Misc / TODOs
- swap `-g` flag back to `-O3` in Makefile
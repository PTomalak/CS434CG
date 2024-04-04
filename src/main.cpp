#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "image.h"
#include "json_helper.h"
#include "ray.h"
#include "sdl_thread.h"
#include "sdl_gui.h"

#include <array>
#include <fstream>
#include <iostream>
#include <thread>
#include <tuple>
#include <unistd.h>
#include <vector>
#include <chrono>
#include <sys/wait.h>

int width;
int height;
int THREADS = 24; // one of those will be used for SDL
bool main_loop = true;
std::string blender_input = "scene/testscene.blend";


struct Vec3 {
  float x, y, z;
};

// Global variables
std::vector<std::vector<std::array<int, 3>>> pixels;
std::vector<glm::vec3> sensor_cell_locs;
float antialias;
float backgroundx;
float backgroundy;
float backgroundz;
int maxdepth;
int resolutionX;
int resolutionY;
int refresh = 0;
int smooth = 0;
float aperature = 0.03f;
int sensors = 4;

const int number_sensor_cells = 4; //makes sensor a 2x2 grid
const float sensor_cell_width = 1.0f / 256; //each cell a 0.25x0.25 square
glm::vec3 camera_pos(0.0f, 0.0f, -800.0f);

struct Light {
  Vec3 pos;
  Vec3 diff;
  Vec3 spec;
};

std::vector<Light> lights;
std::vector<std::tuple<Vec3, float, Vec3, Vec3, float>> spheres;
std::vector<std::tuple<std::vector<Vec3>, Vec3, Vec3, float, float, std::vector<Vec3>>> quads;

//std::thread threads[24];

void render_scene(std::string input, int argc) {
  //pixels.clear();
  //pixels.resize(width, std::vector<std::array<int, 3>>(height));
  
  // First we must reread from blender
  int ret1 = fork();
  if (ret1 == -1) perror("fork");

  if (ret1 == 0) {
    execlp("bash", "bash", "scene/extract.sh", blender_input.c_str());
  }

  waitpid(ret1, NULL, 0);

  int ret2 = fork();
  if (ret2 == -1) perror("fork");

  if (ret2 == 0) {
    execlp("python3", "python3", "scene/parser.py", NULL);
  }

  waitpid(ret2, NULL, 0);
 
  lights.clear();
  spheres.clear();
  quads.clear();

  if (readJSON(input)) {
    printf("Problem reading JSON file\n");
    return;
  }
  
  std::thread threads[THREADS];

  // First thread for SDL handling
  threads[0] = std::thread(handleSDL, argc);

  // setup threads for setting pixel colors
  for (int i = 1; i < THREADS; ++i) {
    threads[i] = std::thread([i]() {
      int start = (height * width * (i - 1)) / (THREADS - 1);
      int end = (height * width * i) / (THREADS - 1);
      for (int p = start; p < end; ++p) {
        int x = p % width;
        int y = p / width;
        raytrace_blur(x, y, i, 1, 0.0f);
      }
    });
  }

  for (int i = 0; i < THREADS; ++i) {
    threads[i].join();
  }

}

std::vector<glm::vec3> GenerateSensorCellArray() {
  std::vector<glm::vec3> sensor_cell_locs;
  for (int i = 0; i < number_sensor_cells; i++) {
    for (int j = 0; j < number_sensor_cells; j++) {
      glm::vec3 loc = glm::vec3((j - (number_sensor_cells / 2) + 0.5f) * sensor_cell_width, (i - (number_sensor_cells / 2) + 0.5f) * sensor_cell_width, camera_pos.z);
      sensor_cell_locs.push_back(loc);
      //printf("J: %d I: %d %f, %f, %f\n", j, i, loc.x, loc.y, loc.z);
    }
  }
  return sensor_cell_locs;
}

int main(int argc, char *argv[]) {
  // Making sure output is as expected
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << " <inputname>"
              << " <savename>" << std::endl;
    return 1;
  }


  // Handle JSON
  std::string input = argv[1];
  if (readJSON(input)) {
    printf("Problem reading JSON file\n");
    return 0;
  }

  width = resolutionX * antialias;
  height = resolutionY * antialias;

  sensor_cell_locs = GenerateSensorCellArray();
  auto startTime = std::chrono::steady_clock::now();

  // for debug prints uncomment:
  // printJSON();

  if(THREADS < 2) {
    printf("Running with %d thread(s), please run with at least 2\n", THREADS);
    return -1;
  }

  // Resize pixels
  pixels.resize(width, std::vector<std::array<int, 3>>(height));

  // This sucks but is a solution to imgui not being thread safe
  while (main_loop) {
    sdl_gui(argc);
    if (main_loop) 
      render_scene(input, argc);
  }

  auto endTime = std::chrono::steady_clock::now();
  auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
  int totalPixels = width * height;
  double timePerPixel = static_cast<double>(elapsedTime) / totalPixels;
  printf("processed %d pixels in %d s. Time per pixel: %.3f ms\n", totalPixels, elapsedTime/1000, timePerPixel);

  std::string savename = argv[2];
  save_bmp(savename, antialias);

  if (refresh == 1) {
    execvp(argv[0], argv);
    return 1;
  }

  return 0;
}

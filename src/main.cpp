#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "image.h"
#include "json_helper.h"
#include "ray.h"
#include "sdl_gui.h"
#include "sdl_thread.h"

#include <array>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sys/wait.h>
#include <thread>
#include <tuple>
#include <unistd.h>
#include <vector>

int width;
int height;
int THREADS = 24; // one of those will be used for SDL
bool main_loop = true;
std::string blender_input = "scene/lenstest.blend";

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
int smooth = 0;
float sensor_spacing = 0.00f;
int sensors = 1;
int headless = 0;

glm::vec3 camera_pos(0.0f, 0.0f, -800.0f);

struct Light {
  Vec3 pos;
  Vec3 diff;
  Vec3 spec;
};

std::vector<Light> lights;
std::vector<std::tuple<Vec3, float, Vec3, Vec3, float>> spheres;
std::vector<
    std::tuple<std::vector<Vec3>, Vec3, Vec3, float, float, std::vector<Vec3>>>
    quads;

// std::thread threads[24];

std::vector<glm::vec3> GenerateSensorCellArray() {
  std::vector<glm::vec3> sensor_cell_locs;
  for (int i = 0; i < sensors; i++) {
    for (int j = 0; j < sensors; j++) {
      glm::vec3 loc =
          glm::vec3((j - (sensors / 2) + 0.5f) * sensor_spacing,
                    (i - (sensors / 2) + 0.5f) * sensor_spacing, camera_pos.z);
      sensor_cell_locs.push_back(loc);
      // printf("J: %d I: %d %f, %f, %f\n", j, i, loc.x, loc.y, loc.z);
    }
  }
  return sensor_cell_locs;
}

void render_scene(std::string input) {
  pixels.clear();

  // First we must reread from blender
  int ret1 = fork();
  if (ret1 == -1)
    perror("fork");

  if (ret1 == 0) {
    printf("using scene: %s\n", blender_input.c_str());
    execlp("bash", "bash", "scene/extract.sh", blender_input.c_str(), nullptr);
    perror("extract.sh failed!!!");
    _exit(1);
  }

  waitpid(ret1, NULL, 0);

  int ret2 = fork();
  if (ret2 == -1)
    perror("fork");

  if (ret2 == 0) {
    execlp("python3", "python3", "scene/parser.py", NULL);
    perror("parser.py failed!!!");
    _exit(1);
  }

  waitpid(ret2, NULL, 0);

  lights.clear();
  spheres.clear();
  quads.clear();

  if (readJSON(input)) {
    printf("Problem reading JSON file\n");
    return;
  }

  width = resolutionX * antialias;
  height = resolutionY * antialias;

  sensor_cell_locs = GenerateSensorCellArray();
  pixels.resize(width, std::vector<std::array<int, 3>>(height));



  std::thread threads[THREADS];
  auto startTime = std::chrono::steady_clock::now();

  if (!headless) {
    // First thread for SDL handling
    threads[0] = std::thread(handleSDL);
  }

  // setup threads for setting pixel colors
  for (int i = 1; i < THREADS; ++i) {
    threads[i] = std::thread([i]() {
      int start = (height * width * (i - 1)) / (THREADS - 1);
      int end = (height * width * i) / (THREADS - 1);
      for (int p = start; p < end; ++p) {
        int x = p % width;
        int y = p / width;
        raytrace_blur(x, y, i);
      }
    });
  }

  for (int i = 1; i < THREADS; ++i) {
    threads[i].join();
  }
  auto endTime = std::chrono::steady_clock::now();
  auto elapsedTime =
      std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime)
          .count();
  int totalPixels = width * height;
  double timePerPixel = static_cast<double>(elapsedTime) / totalPixels;
  printf("processed %d pixels in %d s. Time per pixel: %.3f ms\n", totalPixels,
         elapsedTime / 1000, timePerPixel);
  if (!headless) {
    threads[0].join();
  }
}

int main(int argc, char *argv[]) {
  // Making sure output is as expected
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << " <inputname>"
              << " <savename>" << std::endl;
    return 1;
  }

  if (argc <= 3) {
    headless = 1;
    printf("\n\nrunning in headless mode\n");
  } else {
    printf("\n\nrunning in GUI mode\n");
  }

  // Handle JSON
  std::string input = "./scene/blended.json";
  blender_input = argv[1];

  int numThreads = sysconf(_SC_NPROCESSORS_ONLN);
  printf("Using %d of system threads.\n", numThreads);
  THREADS = numThreads * 5;

  // This sucks but is a solution to imgui not being thread safe
  while (main_loop) {
    if (headless) {
      render_scene(input);
      break;
    } else {
      sdl_gui(argc);
      if (main_loop)
        render_scene(input);
    }
  }

  std::string savename = argv[2];
  save_bmp(savename, antialias);

  return 0;
}

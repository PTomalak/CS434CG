#ifndef RAY_H
#define RAY_H

#include <vector>
#include <array>

#ifdef __APPLE__
#include </opt/homebrew/Cellar/glm/1.0.1/include/glm/glm.hpp>
#else
#include <glm/glm.hpp>
#endif

#include <cmath>
#include <tuple>
#include <iostream>

extern int maxdepth;
extern int width;
extern int height;

extern std::vector<std::vector<std::array<int, 3>>> pixels;

void raytrace(int x, int y, int thread_num);
void raytrace_blur(int x, int y, int thread_num);

#endif // RAY_H
#ifndef RAY_H
#define RAY_H

#include <vector>
#include <array>
#include <glm/glm.hpp>
#include <cmath>
#include <tuple>
#include <iostream>

extern int maxdepth;
extern int width;
extern int height;

extern std::vector<std::vector<std::array<int, 3>>> pixels;

void raytrace(int x, int y, int thread_num);

#endif // RAY_H
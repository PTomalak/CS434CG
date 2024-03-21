#ifndef IMAGE_H
#define IMAGE_H
#include "stb_image_write.h"
#include <iostream>
#include <vector>
#include <cstdint>
#include <array>

extern int width;
extern int height;

extern std::vector<std::vector<std::array<int, 3>>> pixels;

void save_bmp(std::string filename, int antialias);

#endif // IMAGE_H
#include "image.h"
#include <array>
#include <iostream>
#include <string>
#include <vector>

void save_bmp(std::string filename, int antialias) {
  if (pixels.size() < 1) {
    printf("no image to save, exiting...\n");
    return;
  }
  printf("Saving with antialiasing factor: %d\n", antialias);

  // To hold averages
  std::vector<std::array<uint8_t, 3>> averagedPixels;

  // Iterate over all pixels:
  for (size_t y = 0; y < pixels[0].size(); y += antialias) {
    for (size_t x = 0; x < pixels.size(); x += antialias) {

      int sumR = 0, sumG = 0, sumB = 0;
      // Iterate over antialiased group:
      for (int dy = 0; dy < antialias; ++dy) {
        for (int dx = 0; dx < antialias; ++dx) {
          if (x + dx < pixels.size() && y + dy < pixels[0].size()) {
            const auto &pixel = pixels[x + dx][y + dy];
            sumR += pixel[0];
            sumG += pixel[1];
            sumB += pixel[2];
          }
        }
      }
      // Calculate averages
      uint8_t avgR = static_cast<uint8_t>(sumR / (antialias * antialias));
      uint8_t avgG = static_cast<uint8_t>(sumG / (antialias * antialias));
      uint8_t avgB = static_cast<uint8_t>(sumB / (antialias * antialias));

      averagedPixels.push_back({avgR, avgG, avgB});
    }
  }

  int ogWidth = width / antialias;
  int ogHeight = height / antialias;

  // Use STBI to save
  stbi_write_bmp(filename.c_str(), ogWidth, ogHeight, 3,
                 averagedPixels.data());

  std::cout << "saved as BMP" << std::endl;
}

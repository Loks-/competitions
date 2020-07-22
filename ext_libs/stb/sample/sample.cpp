#include <iostream>

#include "../wrapper.h"

int main() {
  PNGWrapper w("message12.png");
  std::cout << w.getChannels() << " " << w.getWidth() << " " << w.getHeight()
            << std::endl;
  for (size_t i = 0; i < w.getChannels(); ++i) {
    for (size_t j = 0; j < w.getHeight(); ++j) {
      for (size_t k = 0; k < w.getWidth(); ++k) {
        std::cout << w.get(i, j, k) << " ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  }
  return 0;
}

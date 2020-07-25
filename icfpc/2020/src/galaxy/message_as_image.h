#pragma once

#include "common/linear_algebra/bool/matrix.h"

#include <string>

namespace galaxy {
class MessageAsImage {
 public:
  const unsigned png_scale = 4;

  la::MatrixBool m;

 public:
  MessageAsImage();
  MessageAsImage(const std::string& png_file_name);

  void ReadPNG(const std::string& png_file_name);
  void Print() const;
  static void PrintPNG(const std::string& png_file_name);
};
}  // namespace galaxy

#include "galaxy/message_as_image.h"

#include "stb/wrapper.h"

#include "common/assert_exception.h"
#include "common/base.h"
#include "common/linear_algebra/bool/matrix.h"

#include <iostream>
#include <string>

namespace galaxy {
namespace {
bool GetPixel(const PNGWrapper& f, unsigned scale, unsigned i, unsigned j) {
  return f.get(0, i * scale, j * scale);
}

bool VerifyPixel(const PNGWrapper& f, unsigned scale, unsigned i, unsigned j,
                 bool value) {
  for (unsigned k = i * scale; k < (i + 1) * scale; ++k) {
    for (unsigned l = j * scale; l < (j + 1) * scale; ++l) {
      if ((f.get(0, k, l) != (value ? 255 : 0)) &&
          (f.get(0, k, l) != (value ? 192 : 0)))
        return false;
    }
  }
  return true;
}

bool VerifyPixel(const PNGWrapper& f, unsigned scale, unsigned i, unsigned j) {
  return VerifyPixel(f, scale, i, j, f.get(0, i * scale, j * scale));
}

bool VerifyBorder(const PNGWrapper& f, unsigned scale, unsigned h, unsigned w) {
  for (unsigned i = 0; i < h; ++i) {
    if (!VerifyPixel(f, scale, i, 0, true) ||
        !VerifyPixel(f, scale, i, w - 1, true))
      return false;
  }
  for (unsigned i = 1; i < h - 1; ++i) {
    if (!VerifyPixel(f, scale, i, 1, false) ||
        !VerifyPixel(f, scale, i, w - 2, false))
      return false;
  }
  for (unsigned i = 0; i < w; ++i) {
    if (!VerifyPixel(f, scale, 0, i, true) ||
        !VerifyPixel(f, scale, h - 1, i, true))
      return false;
  }
  for (unsigned i = 1; i < w - 1; ++i) {
    if (!VerifyPixel(f, scale, 1, i, false) ||
        !VerifyPixel(f, scale, h - 2, i, false))
      return false;
  }
  return true;
}

bool VerifyPNG(const PNGWrapper& f, unsigned scale, unsigned h, unsigned w) {
  if (!VerifyBorder(f, scale, h, w)) return false;
  for (unsigned i = 2; i < h - 2; ++i) {
    for (unsigned j = 2; j < w - 2; ++j) {
      if (!VerifyPixel(f, scale, i, j)) return false;
    }
  }
  return true;
}
}  // namespace

MessageAsImage::MessageAsImage() : m(0) {}

MessageAsImage::MessageAsImage(const std::string& png_file_name) : m(0) {
  ReadPNG(png_file_name);
}

void MessageAsImage::ReadPNG(const std::string& png_file_name) {
  PNGWrapper f(png_file_name);
  Assert((f.getHeight() % png_scale) == 0);
  Assert((f.getWidth() % png_scale) == 0);
  unsigned h = f.getHeight() / png_scale, w = f.getWidth() / png_scale;
  Assert(VerifyPNG(f, png_scale, h, w));
  m = la::MatrixBool(h - 2, w - 2);  // Add one more empty row and column
  for (unsigned i = 0; i < h - 3; ++i) {
    for (unsigned j = 0; j < w - 3; ++j) {
      m.Set(i, j, GetPixel(f, png_scale, i + 2, j + 2));
    }
  }
}

void MessageAsImage::Print() const {
  std::cout << m.Rows() << "\t" << m.Columns() << std::endl;
  for (unsigned i = 0; i < m.Rows(); ++i) {
    for (unsigned j = 0; j < m.Columns(); ++j)
      std::cout << (m.Get(i, j) ? 1 : 0);
    std::cout << std::endl;
  }
}

void MessageAsImage::PrintPNG(const std::string& png_file_name) {
  PNGWrapper f(png_file_name);
  std::cout << f.getChannels() << "\t" << f.getHeight() << "\t" << f.getWidth()
            << std::endl;
  for (int d = 0; d < f.getChannels(); ++d) {
    for (int h = 0; h < f.getHeight(); ++h) {
      for (int w = 0; w < f.getWidth(); ++w) {
        std::cout << unsigned(f.get(d, h, w)) << " ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  }
}
}  // namespace galaxy

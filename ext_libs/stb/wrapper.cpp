#include "wrapper.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

class PNGWrapper::Impl {
 public:
  Impl(const std::string& filename) : filename_(filename) {
    unsigned char* img =
        stbi_load(filename_.c_str(), &width_, &height_, &channels_, 0);
    if (img == NULL) {
      throw std::exception();
    }
    img_.resize(channels_);
    for (size_t i = 0; i < channels_; ++i) {
      img_[i].resize(height_);
      for (size_t j = 0; j < height_; ++j) {
        img_[i][j].resize(width_);
        for (size_t k = 0; k < width_; ++k) {
          img_[i][j][k] = img[i + k * channels_ + j * channels_ * width_];
        }
      }
    }
  }

  ~Impl() {}

  int getWidth() const { return width_; }
  int getHeight() const { return height_; }
  int getChannels() const { return channels_; }

  unsigned char get(size_t channel, size_t x, size_t y) const {
    return img_[channel][x][y];
  }

 private:
  std::string filename_;
  std::vector<std::vector<std::vector<char>>> img_;
  int width_;
  int height_;
  int channels_;
};

PNGWrapper::PNGWrapper(const std::string& filename)
    : impl_(std::make_unique<Impl>(filename)) {}

PNGWrapper::~PNGWrapper() {}

int PNGWrapper::getWidth() const { return impl_->getWidth(); }
int PNGWrapper::getHeight() const { return impl_->getHeight(); }
int PNGWrapper::getChannels() const { return impl_->getChannels(); }

unsigned char PNGWrapper::get(size_t channel, size_t x, size_t y) const {
  return impl_->get(channel, x, y);
}

#pragma once

#include <memory>
#include <string>
#include <vector>

class PNGWrapper {
 public:
  PNGWrapper(const std::string& filename);
  ~PNGWrapper();

  int getWidth() const;
  int getHeight() const;
  int getChannels() const;
  unsigned char get(size_t channel, size_t x, size_t y) const;

 private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};

#pragma once

#include "common/base.h"

#include <algorithm>
#include <cstdarg>
#include <vector>

namespace ds {
template <class TTValue>
class MDVector {
 public:
  using TValue = TTValue;
  using TData = std::vector<TValue>;
  using TShape = std::vector<size_t>;
  using TSelf = MDVector<TValue>;

 protected:
  TData data;
  TShape shape;

 protected:
  static size_t SizeFromShape(const TShape& shape_) {
    size_t size = 1;
    for (auto d : shape_) size *= d;
    return size;
  }

  bool CheckSize() const { return Size() == SizeFromShape(shape); }

 public:
  MDVector(const TData& data_, const TShape& shape_)
      : data(data_), shape(shape_) {
    assert(CheckSize());
  }

  MDVector(const TValue& value, const TShape& shape_)
      : MDVector(TData(SizeFromShape(shape_), value), shape_) {}

  size_t ShapeSize() const { return shape.size(); }
  size_t DSize(size_t dim) const { return shape[dim]; }
  size_t Size() const { return data.size(); }

  const TData& GetData() const { return data; }
  const TShape& GetShape() const { return shape; }

  TValue& GetRI(size_t raw_index) { return data[raw_index]; }
  const TValue& GetRI(size_t raw_index) const { return data[raw_index]; }

  TValue& operator[](size_t raw_index) { return GetRI(raw_index); }
  const TValue& operator[](size_t raw_index) const { return GetRI(raw_index); }

  // Get
  TValue& Get(const std::vector<size_t>& vindex) {
    assert(vindex.size() == ShapeSize());
    size_t raw_index = 0;
    for (unsigned i = 0; i < vindex.size(); ++i)
      raw_index = shape[i] * raw_index + vindex[i];
    return GetRI(raw_index);
  }

  const TValue& Get(const std::vector<size_t>& vindex) const {
    assert(vindex.size() == ShapeSize());
    size_t raw_index = 0;
    for (unsigned i = 0; i < vindex.size(); ++i)
      raw_index = shape[i] * raw_index + vindex[i];
    return GetRI(raw_index);
  }

  TValue& Get() {
    assert(ShapeSize() == 0);
    return data[0];
  }

  const TValue& Get() const {
    assert(ShapeSize() == 0);
    return data[0];
  }

  TValue& Get(unsigned i0) {
    assert(ShapeSize() == 1);
    return GetRI(i0);
  }

  const TValue& Get(unsigned i0) const {
    assert(ShapeSize() == 1);
    return GetRI(i0);
  }

  TValue& Get(unsigned i0, unsigned i1) {
    assert(ShapeSize() == 2);
    return GetRI(i0 * shape[1] + i1);
  }

  const TValue& Get(unsigned i0, unsigned i1) const {
    assert(ShapeSize() == 2);
    return GetRI(i0 * shape[1] + i1);
  }

  TValue& Get(unsigned i0, ...) {
    unsigned ri = i0, ss = ShapeSize();
    std::va_list args;
    va_start(args, i0);
    for (unsigned j = 1; j < ss; ++j)
      ri = shape[j] * ri + va_arg(args, unsigned);
    va_end(args);
    return GetRI(ri);
  }

  const TValue& Get(unsigned i0, ...) const {
    unsigned ri = i0, ss = ShapeSize();
    std::va_list args;
    va_start(args, i0);
    for (unsigned j = 1; j < ss; ++j)
      ri = shape[j] * ri + va_arg(args, unsigned);
    va_end(args);
    return GetRI(ri);
  }

  // Proxy to Get
  template <typename... TArgs>
  TValue& operator()(const TArgs&... args) {
    return Get(args...);
  }

  template <typename... TArgs>
  const TValue& operator()(const TArgs&... args) const {
    return Get(args...);
  }

  void ReshapeI(const std::vector<size_t>& new_shape) {
    assert(SizeFromShape(new_shape) == Size());
    shape = new_shape;
  }

  TSelf Reshape(const std::vector<size_t>& new_shape) const {
    return TSelf(data, new_shape);
  }

  template <class TFunction>
  auto TransformV(TFunction f) const {
    using TOutput = decltype(f(data[0]));
    std::vector<TOutput> vo;
    vo.reserve(Size());
    for (auto& v : data) vo.push_back(f(v));
    return MDVector<TOutput>(vo, shape);
  }

  TValue Sum() const {
    TValue r(0);
    for (auto& v : data) r += v;
    return r;
  }

  void DDropI(unsigned dim) {
    assert(DSize(dim) == 1);
    shape.erase(shape.begin() + dim);
  }

  TSelf DDrop(unsigned dim) const {
    assert(DSize(dim) == 1);
    TShape new_shape = shape;
    new_shape.erase(new_shape.begin() + dim);
    return TSelf(data, new_shape);
  }

  TSelf DProject(unsigned dim, unsigned index) const {
    assert(index < shape[dim]);
    TShape new_shape = shape;
    new_shape[dim] = 1;
    TData new_data(SizeFromShape(new_shape));
    size_t ml = 1, mh = 1;
    for (unsigned i = 0; i < dim; ++i) ml *= shape[i];
    for (unsigned i = dim + 1; i < shape.size(); ++i) mh *= shape[i];
    for (size_t i = 0, rnew = 0, rold = index * mh; i < ml;
         ++i, rold += mh * (shape[dim] - 1)) {
      for (size_t j = 0; j < mh; ++j) new_data[rnew++] = data[rold++];
    }
    return TSelf(new_data, new_shape);
  }

  TSelf DMin(unsigned dim) const {
    TShape new_shape = shape;
    new_shape[dim] = 1;
    TData new_data(SizeFromShape(new_shape));
    size_t ml = 1, mh = 1;
    for (unsigned i = 0; i < dim; ++i) ml *= shape[i];
    for (unsigned i = dim + 1; i < shape.size(); ++i) mh *= shape[i];
    for (size_t i = 0; i < ml; ++i) {
      for (size_t j = 0; j < shape[dim]; ++j) {
        for (size_t k = 0; k < mh; ++k) {
          size_t rnew = i * mh + k, rold = (i * shape[dim] + j) * mh + k;
          if ((j == 0) || (data[rold] < new_data[rnew]))
            new_data[rnew] = data[rold];
        }
      }
    }
    return TSelf(new_data, new_shape);
  }

  TSelf DMax(unsigned dim) const {
    TShape new_shape = shape;
    new_shape[dim] = 1;
    TData new_data(SizeFromShape(new_shape));
    size_t ml = 1, mh = 1;
    for (unsigned i = 0; i < dim; ++i) ml *= shape[i];
    for (unsigned i = dim + 1; i < shape.size(); ++i) mh *= shape[i];
    for (size_t i = 0; i < ml; ++i) {
      for (size_t j = 0; j < shape[dim]; ++j) {
        for (size_t k = 0; k < mh; ++k) {
          size_t rnew = i * mh + k, rold = (i * shape[dim] + j) * mh + k;
          if ((j == 0) || (new_data[rnew] < data[rold]))
            new_data[rnew] = data[rold];
        }
      }
    }
    return TSelf(new_data, new_shape);
  }
};
}  // namespace ds

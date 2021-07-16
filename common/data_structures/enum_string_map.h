#pragma once

#include "common/base.h"

#include <string>
#include <unordered_map>

namespace ds {
class IntStringMap {
 protected:
  std::unordered_map<int64_t, std::string> mi2s;
  std::unordered_map<std::string, int64_t> ms2i;

 public:
  void Add(int64_t i, const std::string& s) {
    assert(mi2s.find(i) == mi2s.end());
    assert(ms2i.find(s) == ms2i.end());
    mi2s[i] = s;
    ms2i[s] = i;
  }

  void Clear() {
    mi2s.clear();
    ms2i.clear();
  }

  size_t Size() const { return mi2s.size(); }

  const std::string& operator()(int64_t i) const {
    auto it = mi2s.find(i);
    assert(it != mi2s.end());
    return it->second;
  }

  int64_t operator()(const std::string& s) const {
    auto it = ms2i.find(s);
    assert(it != ms2i.end());
    return it->second;
  }
};

template <class TEnum>
class EnumStringMap : public IntStringMap {
 public:
  using TBase = IntStringMap;

  void Add(TEnum e, const std::string& s) { TBase::Add(int64_t(e), s); }

  TEnum operator()(const std::string& s) const {
    return TEnum(TBase::operator()(s));
  }

  const std::string& operator()(TEnum e) const {
    return TBase::operator()(int64_t(e));
  }
};
}  // namespace ds

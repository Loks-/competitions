#pragma once

#include "common/base.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace files {
class CSVReader {
 protected:
  std::ifstream file;
  char delimiter;
  std::unordered_map<std::string, unsigned> header;

 protected:
  std::string raw_line;
  std::vector<std::string> current_line;

 protected:
  void InitHeader() {
    NextLine();
    for (unsigned i = 0; i < Size(); ++i) header[current_line[i]] = i;
  }

 public:
  CSVReader(const std::string& filename, bool use_header = true,
            char _delimiter = ',')
      : file(filename), delimiter(_delimiter) {
    if (use_header) InitHeader();
  }

  unsigned ColumnIndex(const std::string& column_name) const {
    auto it = header.find(column_name);
    if (it == header.end()) {
      std::cerr << "Column name " << column_name << " is missed in header."
                << std::endl;
      assert(false);
      return -1u;
    }
    return it->second;
  }

  bool NextLine() {
    if (!file.good()) return false;
    if (!std::getline(file, raw_line)) return false;
    if (raw_line.empty()) return false;
    current_line.clear();
    for (size_t npos1 = 0, npos2;; npos1 = npos2 + 1) {
      npos2 = raw_line.find(delimiter, npos1);
      current_line.push_back(raw_line.substr(npos1, npos2 - npos1));
      if (npos2 == std::string::npos) break;
    }
    return true;
  }

  std::string operator()(unsigned index) const {
    assert(index < current_line.size());
    return current_line[index];
  }

  std::string operator()(const std::string& column_name) const {
    return operator()(ColumnIndex(column_name));
  }

  unsigned Size() const { return current_line.size(); }

  template <typename T>
  T Get(unsigned index) const {
    std::stringstream ss(operator()(index));
    T t;
    ss >> t;
    return t;
  }

  template <typename T>
  T Get(const std::string& column_name) const {
    std::stringstream ss(operator()(column_name));
    T t;
    ss >> t;
    return t;
  }

  bool GetBool(unsigned index) const { return Get<int>(index) != 0; }

  bool GetBool(const std::string& column_name) const {
    return Get<int>(column_name) != 0;
  }
};
}  // namespace files

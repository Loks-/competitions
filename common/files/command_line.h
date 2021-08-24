
#pragma once

#include "common/base.h"

#include <iostream>
#include <string>
#include <unordered_map>

namespace files {
class CommandLine {
 protected:
  std::unordered_map<std::string, int> args_int;
  std::unordered_map<std::string, std::string> args_string;

  void AddArgAutoI(const std::string& key, const std::string& value) {
    if (args_int.find(key) != args_int.end()) {
      args_int[key] = std::stoi(value.c_str());
    } else if (args_string.find(key) != args_string.end()) {
      args_string[key] = value;
    } else {
      std::cerr << "Unknown argument: " << key << std::endl;
      assert(false);
    }
  }

 public:
  void AddArg(const std::string& key, int default_value) {
    args_int[key] = default_value;
  }

  void AddArg(const std::string& key, const std::string& default_value) {
    args_string[key] = default_value;
  }

  void Parse(int argc, char** const argv) {
    for (int index = 1; index + 1 < argc; index += 2) {
      std::string arg = argv[index];
      if (arg[0] == '-') {
        arg = arg.substr(1);
      }
      AddArgAutoI(arg, argv[index + 1]);
    }
  }

  int GetInt(const std::string& key) const {
    auto it = args_int.find(key);
    assert(it != args_int.end());
    return it->second;
  }

  std::string GetString(const std::string& key) const {
    auto it = args_string.find(key);
    assert(it != args_string.end());
    return it->second;
  }
};
}  // namespace files

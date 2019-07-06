#include "command_line.h"

#include "common/assert_exception.h"
#include <cassert>

CommandLine cmd;

void CommandLine::AddArgAutoI(const std::string& key,
                              const std::string& value) {
  if (args_int.find(key) != args_int.end()) {
    args_int[key] = std::stoi(value.c_str());
  } else if (args_string.find(key) != args_string.end()) {
    args_string[key] = value;
  } else {
    Assert(false, "Unknown argument: " + key);
  }
}

void CommandLine::AddArg(const std::string& key, int default_value) {
  args_int[key] = default_value;
}

void CommandLine::AddArg(const std::string& key,
                         const std::string& default_value) {
  args_string[key] = default_value;
}

void CommandLine::Parse(int argc, char* const argv[]) {
  for (int index = 1; index + 1 < argc; index += 2) {
    std::string arg = argv[index];
    if (arg[0] == '-') {
      arg = arg.substr(1);
    }
    AddArgAutoI(arg, argv[index + 1]);
  }
}

int CommandLine::GetInt(const std::string& key) const {
  auto it = args_int.find(key);
  assert(it != args_int.end());
  return it->second;
}

std::string CommandLine::GetString(const std::string& key) const {
  auto it = args_string.find(key);
  assert(it != args_int.end());
  return it->second;
}

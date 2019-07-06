#pragma once

#include <string>
#include <unordered_map>

class CommandLine {
 protected:
  std::unordered_map<std::string, int> args_int;
  std::unordered_map<std::string, std::string> args_string;

  void AddArgAutoI(const std::string& key, const std::string& value);

 public:
  void AddArg(const std::string& key, int default_value);
  void AddArg(const std::string& key, const std::string& default_value);
  void Parse(int argc, char* const argv[]);

  int GetInt(const std::string& key) const;
  std::string GetString(const std::string& key) const;
};

extern CommandLine cmd;

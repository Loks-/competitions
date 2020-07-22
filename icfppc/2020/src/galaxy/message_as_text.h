#pragma once

#include <string>
#include <vector>

class MessageAsText {
 public:
  std::vector<std::string> m;

 public:
  MessageAsText();
  MessageAsText(const std::string& text_file_name);

  void ReadText(const std::string& text_file_name);
};

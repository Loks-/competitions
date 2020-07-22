#include "message_as_text.h"

#include "common/assert_exception.h"

#include <fstream>
#include <iostream>
#include <string>

MessageAsText::MessageAsText() {}

MessageAsText::MessageAsText(const std::string& text_file_name) {
  ReadText(text_file_name);
}

void MessageAsText::ReadText(const std::string& text_file_name) {
  std::ifstream f(text_file_name);
  Assert(f.is_open(), "File " + text_file_name + " is missed.");
  std::string s;
  while (getline(f, s)) {
    if (s.empty() || (s[0] == '#')) continue;
    m.push_back(s);
  }
  f.close();
}

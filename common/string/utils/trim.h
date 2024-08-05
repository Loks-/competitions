#include <algorithm>
#include <cctype>
#include <functional>
#include <locale>

// Trim from start (in place)
constexpr void TrimL(std::string &s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
          }));
}

// Trim from end (in place)
constexpr void TrimR(std::string &s) {
  s.erase(std::find_if(s.rbegin(), s.rend(),
                       [](unsigned char ch) { return !std::isspace(ch); })
              .base(),
          s.end());
}

// Trim from both ends (in place)
constexpr void Trim(std::string &s) {
  TrimR(s);
  TrimL(s);
}

// Trim from start (copying)
constexpr std::string TrimLCopy(std::string s) {
  TrimL(s);
  return s;
}

// Trim from end (copying)
constexpr std::string TrimRCopy(std::string s) {
  TrimR(s);
  return s;
}

// Trim from both ends (copying)
constexpr std::string TrimCopy(std::string s) {
  Trim(s);
  return s;
}

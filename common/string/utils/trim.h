#include <algorithm>
#include <cctype>
#include <functional>
#include <locale>

// Trim from start (in place)
static inline void TrimL(std::string &s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
          }));
}

// Trim from end (in place)
static inline void TrimR(std::string &s) {
  s.erase(std::find_if(s.rbegin(), s.rend(),
                       [](unsigned char ch) { return !std::isspace(ch); })
              .base(),
          s.end());
}

// Trim from both ends (in place)
static inline void Trim(std::string &s) {
  TrimR(s);
  TrimL(s);
}

// Trim from start (copying)
static inline std::string TrimLCopy(std::string s) {
  TrimL(s);
  return s;
}

// Trim from end (copying)
static inline std::string TrimRCopy(std::string s) {
  TrimR(s);
  return s;
}

// Trim from both ends (copying)
static inline std::string TrimCopy(std::string s) {
  Trim(s);
  return s;
}

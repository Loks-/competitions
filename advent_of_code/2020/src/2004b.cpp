#include "common/stl/base.h"
#include "common/string/utils/split.h"
#include "common/vector/read_lines.h"

int main_2004b() {
  auto IsDigits = [](const std::string& s, unsigned l) {
    if (l && (s.size() != l)) return false;
    for (auto c : s) {
      if (!isdigit(c)) return false;
    }
    return true;
  };
  auto IsHex = [](const std::string& s, unsigned l) {
    if (l && (s.size() != l)) return false;
    for (auto c : s) {
      if (!isdigit(c) && ((c < 'a') || (c > 'f'))) return false;
    }
    return true;
  };
  std::vector<std::string> eclrs{"amb", "blu", "brn", "gry",
                                 "grn", "hzl", "oth"};

  vector<string> vsi = nvector::ReadLines(), vs(1, "");
  for (auto& s : vsi) {
    if (s.empty())
      vs.push_back("");
    else
      vs.back() = vs.back() + " " + s;
  }
  unsigned r = 0;
  for (auto& s1 : vs) {
    auto ss = Split(s1, ' ');
    unsigned f = 0;
    for (auto& s2 : ss) {
      if (s2.empty()) continue;
      auto key = s2.substr(0, 3), value = s2.substr(4);
      if (key == "cid") {
        continue;
      } else if ((key == "byr") || (key == "iyr") || (key == "eyr")) {
        if (!IsDigits(value, 4)) break;
        int year = stoi(value);
        if ((year < (key == "byr" ? 1920 : key == "iyr" ? 2010 : 2020)) ||
            (year > (key == "byr" ? 2002 : key == "iyr" ? 2020 : 2030)))
          break;
      } else if (key == "hgt") {
        auto sfx = value.substr(value.size() - 2);
        if ((sfx != "cm") && (sfx != "in")) break;
        auto vadj = value.substr(0, value.size() - 2);
        if (!IsDigits(vadj, 0)) break;
        int h = stoi(vadj);
        if ((h < (sfx == "cm" ? 150 : 59)) || (h > (sfx == "cm" ? 193 : 76)))
          break;
      } else if (key == "hcl") {
        if ((value[0] != '#') || !IsHex(value.substr(1), 6)) break;
      } else if (key == "ecl") {
        if (find(eclrs.begin(), eclrs.end(), value) == eclrs.end()) break;
      } else if (key == "pid") {
        if (!IsDigits(value, 9)) break;
      }
      ++f;
    }
    if (f >= 7) ++r;
  }
  cout << r << endl;
  return 0;
}

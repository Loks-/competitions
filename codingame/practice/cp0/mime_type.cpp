// https://www.codingame.com/training/easy/mime-type

#include "common/stl/base.h"

#include <cctype>
#include <string>
#include <unordered_map>

int main_mime_type() {
  unsigned N, Q;
  cin >> N >> Q;
  std::unordered_map<string, string> ext_map;
  for (unsigned i = 0; i < N; ++i) {
    string ext, type;
    cin >> ext >> type;
    transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    ext_map[ext] = type;
  }
  cin.ignore();
  for (unsigned i = 0; i < Q; ++i) {
    string fname;
    getline(cin, fname);
    transform(fname.begin(), fname.end(), fname.begin(), ::tolower);
    auto n = fname.find_last_of('.');
    auto ext = fname.substr(n + 1);
    auto it = ext_map.find(ext);
    cout << (((n == string::npos) || (it == ext_map.end())) ? "UNKNOWN"
                                                            : it->second)
         << endl;
  }
  return 0;
}

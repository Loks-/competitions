#include "common/stl/base.h"
#include "common/vector/read_all.h"

int main_2101() {
  auto v = nvector::ReadAll<unsigned>();
  for (unsigned d : {1, 3}) {
    unsigned s = 0;
    for (unsigned i = d; i < v.size(); ++i) s += (v[i] > v[i - d]) ? 1 : 0;
    cout << s << endl;
  }
  return 0;
}

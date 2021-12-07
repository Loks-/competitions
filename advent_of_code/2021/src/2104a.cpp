#include "common/data_structures/mdv/min.h"
#include "common/data_structures/mdv/min_index.h"
#include "common/data_structures/mdvector.h"
#include "common/permutation/inverse_line.h"
#include "common/stl/base.h"
#include "common/vector/read_all.h"
#include "common/vector/read_from_line.h"

#include "common/vector/write.h"

int main_2104a() {
  auto vo = nvector::ReadFromLine<unsigned>(',');
  auto vi = permutation::Inverse(vo);
  auto vall = nvector::ReadAll<unsigned>();
  ds::MDVector<unsigned> mo(vall, {vall.size() / 25, 5, 5});
  auto ma = mo.TransformV([&](auto u) { return vi[u]; });
  auto mao = Min(ma.DMax(1).DMin(2), ma.DMax(2).DMin(1));
  auto index = MinIndex(mao);
  auto order = mao[index], value = vo[order];
  unsigned s = 0;
  for (size_t i = 0; i < 25; ++i) {
    size_t j = index * 25 + i;
    if (ma[j] > order) s += mo[j];
  }
  cout << s * value << endl;
  return 0;
}

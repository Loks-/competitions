// https://www.hackerrank.com/challenges/polar-angles

#include "common/geometry/d2/angle_int.h"
#include "common/geometry/d2/vector_io.h"
#include "common/stl/base.h"
#include "common/vector/read.h"

int main_polar_angles() {
  struct S {
   public:
    I2Vector v;
    I2Angle a;
    int64_t d;

    S(const I2Vector& _v) : v(_v), a(_v), d(_v.LengthSquared()) {}

    bool operator<(const S& r) const {
      return (a != r.a) ? a.Compare02Pi(r.a) : d < r.d;
    }
  };

  unsigned n;
  cin >> n;
  auto vv = nvector::Read<I2Vector>(n);
  std::vector<S> vs;
  for (auto& v : vv) vs.push_back(v);
  sort(vs.begin(), vs.end());
  for (auto& s : vs) cout << s.v << endl;
  return 0;
}

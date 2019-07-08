#include "base/core/world.h"

#include "base/action.h"
#include "base/action_type.h"
#include "base/decode.h"
#include "base/eval/map.h"
#include "base/ext/map.h"
#include "base/point.h"
#include "base/solution.h"
#include "base/worker.h"
#include "common/assert_exception.h"
#include "common/string/split.h"
#include <cassert>

namespace base {
namespace core {
template <class TMap>
void World<TMap>::Init(const std::string& problem) {
  map.Init(problem);
  workers.clear();

  auto vs = Split(problem, '#');
  Assert(vs.size() == 4);
  Point pworker = DecodePoint(vs[1]);
  workers.push_back(TWorker(DecodePoint(vs[1]), map));
  workers[0].Wrap();
}

template <class TMap>
TMap& World<TMap>::GetMap() {
  return map;
}

template <class TMap>
int World<TMap>::GetTime() const {
  return map.GetTime();
}

template <class TMap>
void World<TMap>::AdjustTime(int dt) {
  map.AdjustTime(dt);
}

template <class TMap>
void World<TMap>::Apply(unsigned worker_index, const Action& action) {
  assert(worker_index < workers.size());
  TWorker& w = workers[worker_index];
  if (action.type == ActionType::CLONE) {
    w.PickupItem();
    Assert(map.CheckCodeX(w.Location()),
           "Worker should be on CodeX point for Clone action.");
    TWorker wnew(w.Location(), map);
    wnew.Wrap();
    workers.push_back(wnew);
  } else {
    w(action);
  }
}

template <class TMap>
void World<TMap>::Apply(const Solution& solution) {
  unsigned max_workers = solution.actions.size();
  std::vector<unsigned> vindex(max_workers, 0);
  for (bool last = false; !last;) {
    AdjustTime(1);
    last = true;
    unsigned l = workers.size();
    for (unsigned i = 0; i < l; ++i) {
      if (vindex[i] < solution.actions[i].size()) {
        Apply(i, solution.actions[i][vindex[i]++]);
        last = false;
      }
    }
  }
  AdjustTime(-1);  // Last iteration was empty.
}

template <class TMap>
bool World<TMap>::Solved() const {
  return map.Wrapped();
}

template class World<eval::Map>;
template class World<ext::Map>;
}  // namespace core
}  // namespace base

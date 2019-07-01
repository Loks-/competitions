#include "base/world_core.h"

#include "base/action.h"
#include "base/action_type.h"
#include "base/boosters.h"
#include "base/decode.h"
#include "base/map.h"
#include "base/point.h"
#include "base/solution.h"
#include "base/worker.h"
#include "common/assert_exception.h"
#include "common/string/split.h"
#include <cassert>

namespace base {
template <class TMap>
void WorldCore<TMap>::Init(const std::string& problem) {
  boosters.Clear();
  map.Init(problem);
  time = 0;
  workers.clear();

  auto vs = Split(problem, '#');
  Assert(vs.size() == 4);
  Point pworker = DecodePoint(vs[1]);
  workers.push_back(TWorker(DecodePoint(vs[1]), *this));
  workers[0].Wrap();
}

template <class TMap>
Boosters& WorldCore<TMap>::GetBoosters() {
  return boosters;
}

template <class TMap>
TMap& WorldCore<TMap>::GetMap() {
  return map;
}

template <class TMap>
int WorldCore<TMap>::GetTime() const {
  return time;
}

template <class TMap>
void WorldCore<TMap>::Apply(unsigned worker_index, const Action& action) {
  assert(worker_index < workers.size());
  TWorker& w = workers[worker_index];
  if (action.type == ActionType::CLONE) {
    w.PickupItem();
    Assert(map.CheckCodeX(w.Location()),
           "Worker should be on CodeX point for Clone action.");
    TWorker wnew(w.Location(), *this);
    wnew.Wrap();
    workers.push_back(wnew);
  } else {
    w(action);
  }
}

template <class TMap>
void WorldCore<TMap>::Apply(const Solution& solution) {
  unsigned max_workers = solution.actions.size();
  std::vector<unsigned> vindex(max_workers, 0);
  for (bool last = false; !last;) {
    ++time;
    last = true;
    unsigned l = workers.size();
    for (unsigned i = 0; i < l; ++i) {
      if (vindex[i] < solution.actions[i].size()) {
        Apply(i, solution.actions[i][vindex[i]++]);
        last = false;
      }
    }
  }
  --time;  // Last iteration was empty.
}

template <class TMap>
bool WorldCore<TMap>::Solved() const {
  return map.Wrapped();
}

template class WorldCore<Map>;
}  // namespace base

#include "base/world.h"

#include "base/action.h"
#include "base/action_type.h"
#include "base/boosters.h"
#include "base/decode.h"
#include "base/map.h"
#include "base/point.h"
#include "base/solution.h"
#include "common/assert_exception.h"
#include "common/string/split.h"
#include <cassert>

namespace base {
void World::Init(const std::string& problem) {
  boosters.Clear();
  map.Init(problem);
  time = 0;
  workers.clear();

  auto vs = Split(problem, '#');
  Assert(vs.size() == 4);
  Point pworker = DecodePoint(vs[1]);
  workers.push_back(Worker(DecodePoint(vs[1]), *this));
  workers[0].Wrap();
}

Boosters& World::GetBoosters() { return boosters; }

Map& World::GetMap() { return map; }

int World::GetTime() const { return time; }

void World::Apply(unsigned worker_index, const Action& action) {
  assert(worker_index < workers.size());
  Worker& w = workers[worker_index];
  if (action.type == ActionType::CLONE) {
    w.PickupItem();
    Assert(map.CheckCodeX(w.Location()),
           "Worker should be on CodeX point for Clone action.");
    Worker wnew(w.Location(), *this);
    wnew.Wrap();
    workers.push_back(wnew);
  } else {
    w(action);
  }
}

void World::Apply(const Solution& solution) {
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
}

bool World::Solved() const { return map.Wrapped(); }
}  // namespace base

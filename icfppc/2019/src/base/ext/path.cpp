#include "base/ext/path.h"

#include "base/action.h"
#include "base/action_type.h"
#include "base/ext/map.h"
#include "common/data_structures/unsigned_set.h"
#include <queue>
#include <utility>

namespace base {
namespace ext {
Action Path::PathToTarget(const Map& m, unsigned from, TTarget target) {
  // (current, first_move)
  thread_local std::queue<std::pair<unsigned, unsigned>> q;
  for (; !q.empty();) q.pop();
  thread_local ds::UnsignedSet s;
  if (s.Size() < m.Size()) s.Resize(m.Size());
  s.Clear();

  if (target(from)) return ActionType::DO_NOTHING;
  for (unsigned next : m.GEdges(from)) {
    if (!s.HasKey(next)) {
      s.Insert(next);
      q.push({next, next});
    }
  }

  for (; !q.empty(); q.pop()) {
    unsigned current = q.front().first;
    if (target(current)) return m.Move(from, q.front().second);
    for (unsigned next : m.GEdges(current)) {
      if (!s.HasKey(next)) {
        s.Insert(next);
        q.push({next, q.front().second});
      }
    }
  }
  return ActionType::END;
}

Action Path::PathToTarget(const Map& m, unsigned from,
                          const ds::UnsignedSet& target) {
  auto f = [target](unsigned u) { return target.HasKey(u); };
  return PathToTarget(m, from, f);
}
}  // namespace ext
}  // namespace base

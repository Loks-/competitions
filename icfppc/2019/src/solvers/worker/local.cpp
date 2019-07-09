#include "solvers/worker/local.h"

#include "base/action.h"
#include "base/ext/event_type.h"
#include "base/ext/world.h"
#include "solvers/worker/simple.h"
#include <cassert>
#include <string>

namespace solvers {
namespace worker {
void Local::ResetTarget(const base::ext::World& world) {
  const auto& map = world.GetMap();
  const auto& unwrapped = map.Unwrapped();
  assert(!unwrapped.Empty());
  unsigned size = map.Size();
  unwrapped_ds.Init(size);
  for (unsigned i = 0; i < size; ++i) {
    if (unwrapped.HasKey(i)) {
      for (unsigned j : map.GEdges(i)) {
        if ((j > i) && unwrapped.HasKey(j)) unwrapped_ds.Union(i, j);
      }
    }
  }

  unsigned min_size = size;
  for (unsigned i : unwrapped.List()) {
    unsigned isize = unwrapped_ds.GetSize(i);
    if (min_size > isize) {
      min_size = isize;
    }
  }
  target.Clear();
  for (unsigned i : unwrapped.List()) {
    if (unwrapped_ds.GetSize(i) == min_size) target.Insert(i);
  }
}

void Local::Init(base::ext::World& world) {
  target.Resize(world.GetMap().Size());
  ResetTarget(world);
  events_index = 0;
}

void Local::Update(const base::ext::World& world) {
  auto& events = world.GetMap().Events();
  bool target_update_required = false;
  for (; events_index < events.size(); ++events_index) {
    if (events[events_index].type == base::ext::EventType::WRAP)
      target_update_required = true;
  }
  if (target_update_required) ResetTarget(world);
}

base::Action Local::NextMove(base::ext::World& world) {
  Update(world);
  return Simple::NextMove(world, target);
}

std::string Local::Name() const { return "local"; }
}  // namespace worker
}  // namespace solvers

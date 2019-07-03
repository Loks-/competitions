#include "base/worker.h"

#include "base/action.h"
#include "base/action_type.h"
#include "base/booster_type.h"
#include "base/boosters.h"
#include "base/direction.h"
#include "base/eval/map.h"
#include "base/ext/map.h"
#include "base/manipulator.h"
#include "base/point.h"
#include "base/rotation.h"
#include "common/assert_exception.h"
#include <algorithm>

namespace base {
template <class TMap>
Worker<TMap>::Worker(const Point& _p, TMap& _map) : p(_p), map(_map) {
  time_fast_wheels = -1;
  time_drill = -1;
}

template <class TMap>
const Point& Worker<TMap>::Location() const {
  return p;
}

template <class TMap>
void Worker<TMap>::Wrap() {
  manipulators.Wrap(p, map);
}

template <class TMap>
void Worker<TMap>::PickupItem() {
  map.PickupItem(p);
}

template <class TMap>
void Worker<TMap>::operator()(const Action& action, int time) {
  PickupItem();
  switch (action.type) {
    case ActionType::MOVE_UP:
    case ActionType::MOVE_DOWN:
    case ActionType::MOVE_LEFT:
    case ActionType::MOVE_RIGHT: {
      bool fast_wheels_enabled = time_fast_wheels > time;
      bool drill_enabled = time_drill > time;
      Direction d(action.type);
      p = d(p);
      Assert(map.Inside(p), "Moving worker outside of map.");
      if (drill_enabled) {
        if (map.Obstacle(p)) {
          map.Drill(p);
        }
      } else {
        Assert(!map.Obstacle(p), "Moving worker to obstacle.");
      }
      Wrap();
      if (fast_wheels_enabled) {
        Point pt = d(p);
        if (map.Inside(pt)) {
          if (drill_enabled || !map.Obstacle(pt)) {
            PickupItem();
            p = pt;
            if (drill_enabled && map.Obstacle(p)) {
              map.Drill(p);
            }
            Wrap();
          }
        }
      }
    } break;
    case ActionType::DO_NOTHING:
      break;
    case ActionType::ROTATE_CLOCKWISE:
    case ActionType::ROTATE_COUNTERCLOCKWISE: {
      Rotation r(action.type);
      manipulators.Rotate(r.GetType());
      Wrap();
    } break;
    case ActionType::ATTACH_MANIPULATOR:
      Assert(map.GetBoosters().Available(BoosterType::EXTENSION),
             "Booster EXTENSION not available for use.");
      map.GetBoosters().Remove(BoosterType::EXTENSION);
      manipulators.Add(Point{action.x, action.y});
      Wrap();
      break;
    case ActionType::ATTACH_FAST_WHEELS:
      Assert(map.GetBoosters().Available(BoosterType::FAST_WHEELS),
             "Booster FAST_WHEELS not available for use.");
      map.GetBoosters().Remove(BoosterType::FAST_WHEELS);
      time_fast_wheels = std::max(time_fast_wheels, time) + TIME_FAST_WHEELS;
      break;
    case ActionType::USING_DRILL:
      Assert(map.GetBoosters().Available(BoosterType::DRILL),
             "Booster DRILL not available for use.");
      map.GetBoosters().Remove(BoosterType::DRILL);
      time_drill = std::max(time_drill, time) + TIME_DRILL;
      break;
    case ActionType::SET_BEACON:
      Assert(map.GetBoosters().Available(BoosterType::TELEPORT),
             "Booster TELEPORT not available for use.");
      map.GetBoosters().Remove(BoosterType::TELEPORT);
      map.AddBeacon(p);
      break;
    case ActionType::SHIFT: {
      Point pt{action.x, action.y};
      Assert(map.CheckBeacon(pt), "Beacon is not exist on required location.");
      p = pt;
      Wrap();
    } break;
    default:
      Assert(false, "Unknown action.");
  }
}

template class Worker<eval::Map>;
template class Worker<ext::Map>;
}  // namespace base

#include "problem.h"

#include "settings.h"
#include "utils/json_point.h"

#include "common/files/json.h"
#include "common/geometry/d2/distance/distance_l2.h"
#include "common/geometry/d2/point.h"
#include "common/geometry/d2/polygon.h"

#include <string>
#include <vector>

bool Problem::LoadI(const std::string& _id, const std::string& filename) {
  id = _id;
  files::JSON json;
  if (!json.Load(filename)) return false;
  epsilon = json.GetInteger("epsilon");
  std::vector<I2Point> vp;
  auto& json_hole = json.GetValue("hole");
  vp.resize(json_hole.Size());
  for (unsigned i = 0; i < vp.size(); ++i)
    vp[i] = GetPoint(json_hole.GetValue(i));
  hole = I2Polygon(vp);
  auto& json_figure = json.GetValue("figure");
  auto& json_fpoints = json_figure.GetValue("vertices");
  auto& json_fedges = json_figure.GetValue("edges");
  vp.resize(json_fpoints.Size());
  for (unsigned i = 0; i < vp.size(); ++i)
    vp[i] = GetPoint(json_fpoints.GetValue(i));
  figure.Resize(vp.size());
  int64_t eps_scale = 1000000;
  for (unsigned i = 0; i < json_fedges.Size(); ++i) {
    auto u0 = json_fedges.GetValue(i).GetInteger(0),
         u1 = json_fedges.GetValue(i).GetInteger(1);
    auto d = SquaredDistanceL2(vp[u0], vp[u1]);
    auto dd = (d * epsilon) / eps_scale;
    auto dmin = (epsilon < eps_scale) ? d - dd : 0, dmax = d + dd;
    figure.AddEdge(u0, u1, {dmin, dmax});
  }
  if (json.HasKey("bonuses")) {
    auto& json_bonuses = json.GetValue("bonuses");
    bonuses.resize(json_bonuses.Size());
    for (unsigned i = 0; i < bonuses.size(); ++i)
      bonuses[i].LoadFromProblem(id, json_bonuses.GetValue(i));
  }
  return true;
}

std::string Problem::GetFileName(const std::string& _id) {
  return ProblemFilename(_id);
}

bool Problem::Load(const std::string& _id) {
  return LoadI(_id, GetFileName(_id));
}

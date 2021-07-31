#include "solution.h"

#include "settings.h"
#include "utils/json_point.h"

#include "common/files/json.h"
#include "common/geometry/d2/point.h"

#include <string>
#include <vector>

bool Solution::LoadI(const std::string& _id, const std::string& filename) {
  id = _id;
  files::JSON json;
  if (!json.Load(filename)) return false;
  auto& json_vertices = json.GetValue("vertices");
  points.resize(json_vertices.Size());
  for (unsigned i = 0; i < points.size(); ++i)
    points[i] = GetPoint(json_vertices.GetValue(i));
  if (json.HasKey("bonuses")) {
    auto& json_bonuses = json.GetValue("bonuses");
    assert(json_bonuses.Size() == 1);
    bonus_used.LoadFromSolution(id, json_bonuses.GetValue(0));
  }
  return true;
}

void Solution::SaveI(const std::string& filename) const {
  files::JSON json, json_vertices;
  json_vertices.SetArray();
  for (auto& p : points) json_vertices.Add(ToJSON(p));
  json.SetDictionary();
  json.Add("vertices", json_vertices);
  json.Save(filename);
}

std::string Solution::GetFileName(const std::string& _id,
                                  const std::string& solver_name) {
  return SolutionFilename(_id, solver_name);
}

bool Solution::Load(const std::string& _id, const std::string& solver_name) {
  return LoadI(_id, GetFileName(_id, solver_name));
}

void Solution::Save(const std::string& solver_name) {
  SaveI(GetFileName(id, solver_name));
}

#include "bonus.h"

#include "utils/json_point.h"

#include "common/data_structures/enum_string_map.h"
#include "common/files/json.h"

namespace {
class BonusTypeMap : public ds::EnumStringMap<Bonus::BonusType> {
 public:
  BonusTypeMap() {
    Add(Bonus::NONE, "NONE");
    Add(Bonus::GLOBALIST, "GLOBALIST");
    Add(Bonus::BREAK_A_LEG, "BREAK_A_LEG");
    Add(Bonus::WALLHACK, "WALLHACK");
    Add(Bonus::SUPERFLEX, "SUPERFLEX");
  }
};

BonusTypeMap bonus_type_map;
}  // namespace

void Bonus::LoadFromProblem(const std::string& problem_id,
                            const files::JSON& json) {
  type = bonus_type_map(json.GetString("bonus"));
  problem_get = problem_id.empty() ? 0 : std::stoi(problem_id);
  problem_use = json.GetInteger("problem");
  position = GetPoint(json.GetValue("position"));
}

void Bonus::LoadFromSolution(const std::string& solution_id,
                             const files::JSON& json) {
  type = bonus_type_map(json.GetString("bonus"));
  problem_get = json.GetInteger("problem");
  problem_use = solution_id.empty() ? 0 : std::stoi(solution_id);
}

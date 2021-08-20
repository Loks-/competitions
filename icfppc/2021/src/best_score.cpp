#include "best_score.h"

#include "settings.h"

#include "common/base.h"
#include "common/files/csv_reader.h"

#include <string>

BestScore::BestScore() {
  std::string filename = path_to_root + "problems/best_score.csv";
  files::CSVReader r(filename);
  for (; r.NextLine();) score[r("problem")] = r.Get<unsigned>("best_score");
}

unsigned BestScore::operator()(unsigned id) const {
  return operator()(std::to_string(id));
}

unsigned BestScore::operator()(const std::string& id) const {
  auto it = score.find(id);
  assert(it != score.end());
  return it->second;
}

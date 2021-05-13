#pragma once

#include "settings.h"

#include <cmath>

inline constexpr double ExplorationMult() { return 30; }

inline constexpr double ExplorationMultExt() {
  return ExplorationMult() * (UseExtScore() ? double(ExtScoreScale()) : 1.0);
}

inline double Log2Games(unsigned games) { return log2(games + 1); }

inline double MCScore(double score, double l2g, unsigned games) {
  return (games > 0) ? score + ExplorationMult() * sqrt(l2g / games)
                     : MCMaxScore();
}

inline double MCScoreExt(double score, double l2g, unsigned games) {
  return (games > 0) ? score + ExplorationMultExt() * sqrt(l2g / games)
                     : MCMaxScore();
}

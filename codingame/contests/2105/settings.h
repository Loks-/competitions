#pragma once

#include "common/base.h"

inline constexpr unsigned CellsSize() { return 37; }
inline constexpr unsigned InitialNutrient() { return 20; }
inline constexpr unsigned SeedBaseCost() { return 0; }
inline unsigned GrowBaseCost(unsigned new_size) { return (1u << new_size) - 1; }
inline constexpr unsigned CompleteCost() { return 4; }
inline unsigned CompletePointsFromRichness(uint8_t r) { return 2 * (r - 1); }
inline constexpr unsigned TotalDays() { return 24; }

inline int PMult(unsigned player) { return 2 * int(player) - 1; }

inline unsigned FinalScore(unsigned score, unsigned sun) {
  return score + sun / 3;
}

inline int64_t PDScore(uint64_t fscore0, uint64_t fscore1, unsigned player) {
  return PMult(player) * (int64_t(fscore1) - int64_t(fscore0));
}

inline constexpr bool UseExtScore() { return true; }
inline constexpr int64_t ExtScoreScale() { return (1ll << 14); }
inline constexpr int64_t MCMaxScore() { return 1000000000; }

inline constexpr int64_t MaxTimePerMove() { return 10; }

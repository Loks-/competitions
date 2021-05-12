#pragma once

inline unsigned CellsSize() { return 37; }
inline unsigned InitialNutrient() { return 20; }
inline unsigned SeedBaseCost() { return 0; }
inline unsigned GrowBaseCost(unsigned new_size) { return (1u << new_size) - 1; }
inline unsigned CompleteCost() { return 4; }
inline unsigned CompletePointsFromRichness(uint8_t r) { return 2 * (r - 1); }
inline unsigned TotalDays() { return 24; }

inline unsigned FinalScore(unsigned score, unsigned sun) {
  return score + sun / 3;
}

inline int64_t PDScore(unsigned fscore0, unsigned fscore1, unsigned player) {
  return (2 * int(player) - 1) * (int64_t(fscore1) - int64_t(fscore0));
}

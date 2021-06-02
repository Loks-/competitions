#pragma once

enum class Result {
  Unknown = 0,
  Loss = 1,
  LossOrDraw = 2,
  Draw = 3,
  WinOrDraw = 4,
  Win = 5,
  LossPlusWin = 6
};

inline Result Invert(Result a) {
  return Result(unsigned(Result::LossPlusWin) - unsigned(a));
}

inline bool Finalized(Result a) {
  static const unsigned mask = (1u << unsigned(Result::Loss)) |
                               (1u << unsigned(Result::Draw)) |
                               (1u << unsigned(Result::Win));
  return (1u << unsigned(a)) & mask;
}

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

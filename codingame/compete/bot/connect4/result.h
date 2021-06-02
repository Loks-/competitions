#pragma once

class Result {
 public:
  static const unsigned Unknown = 0;
  static const unsigned Loss = 1;
  static const unsigned LossOrDraw = 2;
  static const unsigned Draw = 3;
  static const unsigned WinOrDraw = 4;
  static const unsigned Win = 5;
  static const unsigned LossPlusWin = 6;

  static unsigned Invert(unsigned r) { return LossPlusWin - r; }

  static bool Finalized(unsigned r) {
    static const unsigned mask = (1u << Loss) | (1u << Draw) | (1u << Win);
    return r & mask;
  }
};

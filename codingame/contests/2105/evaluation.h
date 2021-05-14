#pragma once

#include "cells.h"
#include "game.h"

class Evaluation {
 protected:
  unsigned player;

 public:
  void Reset(const Cells&, unsigned _player) { player = _player; }

  int64_t ApplyC(const Game&) { return 0; }
  int64_t ApplyI(Game&) { return 0; }
};

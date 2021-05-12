#pragma once

#include "cells.h"
#include "game.h"

class Evaluation {
 protected:
  unsigned player;

 public:
  void Reset(const Cells&, unsigned _player) { player = _player; }

  int64_t Apply(const Game&) { return 0; }
};

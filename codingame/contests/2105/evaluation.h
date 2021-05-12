#pragma once

#include "game.h"
#include "settings.h"

class Evaluation {
 protected:
  unsigned player;

 public:
  void Reset(const Game&, unsigned _player) { player = _player; }

  int64_t Apply(const Game&) { return 0; }
};

#pragma once

#include "action.h"
#include "game.h"

#include <string>

class Strategy {
 public:
  unsigned player;

  void SetP(unsigned _player) { player = _player; }
  virtual std::string Name() const { return "Wait"; }
  virtual Action GetAction(const Game&) { return {}; }
};

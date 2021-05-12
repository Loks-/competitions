#pragma once

#include "action.h"
#include "cells.h"
#include "game.h"

#include <memory>
#include <string>

class Strategy {
 public:
  unsigned player;

  void SetP(unsigned _player) { player = _player; }

  virtual void Reset(const Cells&) {}
  virtual std::string Name() const { return "Wait"; }
  virtual Action GetAction(const Game&) { return {}; }

  static std::shared_ptr<Strategy> Make() {
    return std::make_shared<Strategy>();
  }
};

using PStrategy = std::shared_ptr<Strategy>;

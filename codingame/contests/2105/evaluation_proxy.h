#pragma once

#include "cells.h"
#include "evaluation.h"
#include "game.h"

#include "common/base.h"

#include <vector>

template <class TFStrategy0, class TFStrategy1 = TFStrategy0>
class EvaluationProxy : public Evaluation {
 public:
  using TBase = Evaluation;

 protected:
  Game game;

 public:
  void Reset(const Cells& cells, unsigned _player) {
    TBase::Reset(cells, _player);
    game.cells = cells;
  }

  int64_t Apply(const Game& _game) {
    game.pos = _game.pos;
    for (; game.pos.day < TotalDays();)
      game.ApplyActions(TFStrategy0::Get(game, 0), TFStrategy1::Get(game, 1));
    return game.PScoreExt(Evaluation::player);
  }
};

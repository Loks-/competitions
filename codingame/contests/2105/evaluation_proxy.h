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
  Game lgame;

 public:
  void Reset(const Cells& cells, unsigned _player) {
    TBase::Reset(cells, _player);
    lgame.cells = cells;
  }

  int64_t ApplyC(const Game& game) {
    lgame.pos = game.pos;
    return ApplyI(lgame);
  }

  int64_t ApplyI(Game& game) {
    for (; !game.Ended();)
      game.ApplyActions(TFStrategy0::Get(game, 0), TFStrategy1::Get(game, 1));
    return game.PScoreExt(Evaluation::player);
  }
};

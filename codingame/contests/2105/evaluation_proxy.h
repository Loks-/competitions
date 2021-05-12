#pragma once

#include "evaluation.h"
#include "game.h"

#include "common/base.h"

#include <vector>

template <class TStrategy0, class TStrategy1 = TStrategy0>
class EvaluationProxy : public Evaluation {
 public:
  using TBase = Evaluation;

 protected:
  Game game;

 public:
  void Reset(const Game& _game, unsigned _player) {
    TBase::Reset(_game, _player);
    game.cells = _game.cells;
  }

  int64_t Apply(const Game& _game) {
    game.pos = _game.pos;
    for (; game.pos.day < TotalDays();)
      game.ApplyActions(TStrategy0::Get(game, 0), TStrategy1::Get(game, 1));
    return game.PScore(Evaluation::player);
  }
};

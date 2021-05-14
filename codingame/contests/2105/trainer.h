#pragma once

#include "featurization.h"
#include "runner2.h"

#include "common/linear_algebra/vector.h"
#include "common/optimization/model/linear.h"
#include "common/optimization/model/trainer/datapoint.h"
#include "common/optimization/model/trainer/xtx.h"

#include <iostream>
#include <vector>

class Trainer {
 public:
  using TODataPoint = opt::model::trainer::DataPoint<DVector, double>;
  using TOModel = opt::model::Linear;
  using TOTrainer = opt::model::trainer::XTX;

 protected:
  Runner2 r;
  TOTrainer t;
  TOModel m;
  Featurization f;
  std::vector<double> vf;

 public:
  template <class TStrategy0, class TStrategy1>
  void SetTrainers() {
    r.SetStrategy0(TStrategy0::Make());
    r.SetStrategy1(TStrategy1::Make());
  }

  void AddRandomGame(bool both_players_view) {
    t.SetRidge(.1, .01);
    r.NewGame();
    r.RunGame();
    auto v = r.GetActions();
    int64_t s = r.game.PScore(1);
    r.RestartGame();
    for (auto p : v) {
      for (unsigned ip = 0; ip < 2; ++ip) {
        if (!ip && !both_players_view) continue;
        double ds = (ip ? s : -s);
        f.Apply(r.game, ip, vf);
        t.AddPoint(TODataPoint(vf, ds));
      }
      r.Apply(p);
    }
  }

  void AddNGames(unsigned ngames, bool both_players_view) {
    for (unsigned i = 0; i < ngames; ++i) AddRandomGame(both_players_view);
  }

  void Train() {
    t.Train();
    m = t.GetModel();
    std::cout << "R2 = " << t.R2() << std::endl;
    std::cout << "[";
    for (auto d : m.b) std::cout << d << " ";
    std::cout << "]" << std::endl;
  }
};

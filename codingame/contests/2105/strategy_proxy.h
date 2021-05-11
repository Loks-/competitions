#pragma once

#include "action.h"
#include "game.h"
#include "strategy.h"

#include <string>

template <class TFStrategy>
class StrategyProxy : public Strategy {
 public:
  using TSelf = StrategyProxy<TFStrategy>;

 protected:
  std::string name;

 public:
  StrategyProxy(const std::string& _name) : name(_name) {}

  std::string Name() const override { return name; }

  Action GetAction(const Game& game) override {
    return TFStrategy::Get(game, Strategy::player);
  }

  static PStrategy Make(const std::string& s = "Proxy") {
    return std::make_shared<TSelf>(s);
  }
};

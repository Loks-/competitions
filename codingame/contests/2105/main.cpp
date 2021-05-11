#include "fstrategy_random.h"
#include "runner.h"
#include "strategy.h"
#include "strategy_mcts.h"
#include "strategy_mcts2.h"
#include "strategy_proxy.h"
#include "strategy_wsgc_uts.h"

int main() {
  Runner r;
  // r.SetStrategy0(Strategy::Make());
  // r.SetStrategy0(StrategyMCTS2::Make());
  r.SetStrategy0(StrategyWSGCUTS::Make());
  // r.SetStrategy1(Strategy::Make());
  // r.SetStrategy1(StrategyProxy<FStrategyRandom>::Make("Random"));
  r.SetStrategy1(StrategyMCTS2::Make());
  // r.SetStrategy1(StrategyWSGCUTS::Make());
  r.RunGame();
  return 0;
}

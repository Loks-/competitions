#include "fstrategy_builder.h"
#include "fstrategy_complete.h"
#include "fstrategy_random.h"
#include "fstrategy_wait_and_complete.h"
#include "runner.h"
#include "strategy.h"
#include "strategy_mcts.h"
#include "strategy_mcts2.h"
#include "strategy_mcts3.h"
#include "strategy_proxy.h"
#include "strategy_proxy.h"
#include "strategy_wsgc_mcts.h"
#include "strategy_wsgc_uts.h"
#include "strategy_wsgc_uts2.h"

int main() {
  Runner r;
  // Runner r(2230846503);
  // r.SetStrategy0(Strategy::Make());
  r.SetStrategy0(StrategyProxy<FStrategyBuilder<8, 2>>::Make());
  // r.SetStrategy0(StrategyMCTS2::Make());
  // r.SetStrategy0(StrategyWSGCUTS::Make());
  // r.SetStrategy0(StrategyWSGCUTS2<FStrategyWaitAndComplete>::Make());
  // r.SetStrategy0(StrategyWSGCMCTS<FStrategyWaitAndComplete>::Make());
  // r.SetStrategy1(Strategy::Make());
  // r.SetStrategy1(StrategyProxy<FStrategyRandom>::Make("Random"));
  // r.SetStrategy1(StrategyMCTS2::Make());
  r.SetStrategy1(StrategyMCTS3<FStrategyWaitAndComplete>::Make());
  // r.SetStrategy1(
  //     StrategyMCTS3<FStrategyComplete, FStrategyWaitAndComplete>::Make());
  // r.SetStrategy1(StrategyWSGCUTS::Make());
  r.RunGame();
  return 0;
}

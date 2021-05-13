#include "fstrategy_builder.h"
#include "fstrategy_complete.h"
#include "fstrategy_wait_and_complete.h"
#include "runner.h"
#include "strategy.h"
#include "strategy_mcts.h"
#include "strategy_mcts2.h"
#include "strategy_mcts3.h"
#include "strategy_mcts4.h"
#include "strategy_proxy.h"
#include "strategy_proxy.h"
#include "strategy_wsgc_mcts.h"
#include "strategy_wsgc_uts.h"
#include "strategy_wsgc_uts2.h"

#include <string>

int main(int nargs, char **pargs) {
  Runner r;
  // Runner r(2230846503);
  // r.SetStrategy0(Strategy::Make());
  // r.SetStrategy0(StrategyProxy<FStrategyBuilder<1, 1>>::Make("Builder 1/1"));
  // r.SetStrategy0(StrategyMCTS2::Make());
  r.SetStrategy0(StrategyMCTS3<FStrategyBuilder<1, 1>>::Make());
  // r.SetStrategy0(StrategyWSGCUTS::Make());
  // r.SetStrategy0(StrategyWSGCUTS2<FStrategyWaitAndComplete>::Make());
  // r.SetStrategy0(StrategyWSGCMCTS<FStrategyBuilder<1, 1>>::Make());
  // r.SetStrategy1(Strategy::Make());
  // r.SetStrategy1(StrategyProxy<FStrategyBuilder<1, 1>>::Make("Builder 1/1"));
  // r.SetStrategy1(StrategyMCTS2::Make());
  r.SetStrategy1(StrategyMCTS4<FStrategyBuilder<1, 1>>::Make());
  // r.SetStrategy1(StrategyWSGCMCTS<FStrategyBuilder<1, 1>>::Make());
  if (nargs > 1) {
    unsigned ngames = std::stoul(pargs[1]);
    r.RunNGames(ngames);
  } else {
    r.RunGame();
  }
  return 0;
}

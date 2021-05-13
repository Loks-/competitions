#include "cell.h"
#include "fstrategy_builder.h"
#include "fstrategy_complete.h"
#include "fstrategy_wait_and_complete.h"
#include "game.h"
#include "strategy_proxy.h"
// #include "strategy_mcts3.h"
// #include "strategy_wsgc_mcts.h"
// #include "strategy_wsgc_uts2.h"

int main_submit() {
  auto s = StrategyProxy<FStrategyBuilder<>>::Make();
  // auto s = StrategyMCTS3<FStrategyWaitAndComplete>::Make();
  // auto s = StrategyWSGCMCTS<FStrategyWaitAndComplete>::Make();
  // auto s = StrategyWSGCUTS2<FStrategyWaitAndComplete>::Make();
  s->SetP(1);
  Game g;
  g.ReadCells();
  s->Reset(g.cells);

  // game loop
  while (1) {
    g.ReadPosition();
    auto a = s->GetAction(g);
    a.Write();
  }
  return 0;
}

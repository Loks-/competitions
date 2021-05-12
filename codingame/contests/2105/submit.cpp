#include "cell.h"
#include "game.h"
#include "strategy_wsgc_uts.h"

int main_submit() {
  auto s = StrategyWSGCUTS::Make();
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

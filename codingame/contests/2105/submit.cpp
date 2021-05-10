#include "cell.h"
#include "game.h"
#include "strategy_mcts.h"

int main_submit() {
  auto s = StrategyMCTS::Make();
  s->SetP(1);
  Game g;
  g.ReadCells();

  // game loop
  while (1) {
    g.ReadPosition();
    auto a = s->GetAction(g);
    a.Write();
  }
  return 0;
}

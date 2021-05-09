#include "cell.h"
#include "game_turn.h"

#include "common/base.h"
#include "common/stl/base.h"

int main() {
  auto cells = Cell::ReadCells();
  GameTurn gturn;

  // game loop
  while (1) {
    gturn.Read();

    unsigned d = gturn.day, rs = ((d == 0) || (d == 1))   ? 1
                                 : ((d == 2) || (d == 4)) ? 2
                                                          : 3;
    unsigned best_cell = cells.size(), richness = 0;
    if (gturn.Me().sun >= ((rs == 1) ? 3 : (rs == 2) ? 7 : 4)) {
      for (auto& tree : gturn.trees) {
        if (tree.player && (tree.size == rs)) {
          if (cells[tree.cell].richness > richness) {
            richness = cells[tree.cell].richness;
            best_cell = tree.cell;
          }
        }
      }
    }
    if (best_cell < cells.size()) {
      cout << ((rs < 3) ? "GROW " : "COMPLETE ") << best_cell << endl;
    } else {
      cout << "WAIT" << endl;
    }
  }
  return 0;
}

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

    unsigned best_cell = cells.size(), richness = 0;
    if (gturn.Me().sun >= 4) {
      for (auto& tree : gturn.trees) {
        if (tree.player) {
          if (cells[tree.cell].richness > richness) {
            richness = cells[tree.cell].richness;
            best_cell = tree.cell;
          }
        }
      }
    }
    if (best_cell < cells.size()) {
      cout << "COMPLETE " << best_cell << endl;
    } else {
      cout << "WAIT" << endl;
    }
  }
  return 0;
}

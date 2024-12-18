

#include <vector>
#include "simulation.h"

void life(std::vector<std::vector<int>>& board, int ROW, int COL) {
    std::vector<std::vector<int>> tempBoard(ROW, std::vector<int>(COL, 0));
    tempBoard = board;

    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            int current_field = tempBoard[i][j];
            int neighbour_count = 0;
            //Bierzemy każdą komórkę po kolei
            for (int z = -1; z <= 1; z++) {
                for (int y = -1; y <= 1; y++) {
                    if (z == 0 && y == 0) continue;
                    if ((i + z >= 0 && i + z < ROW) && (j + y >= 0 && j + y < COL)) {
                        //Jeżeli sąsiad jest "żywy" to dodajemy 1 do liczby żywych sąsiadów.
                        if (int neighbour = tempBoard[i + z][j + y]; neighbour == 1) {
                            neighbour_count += 1;
                        }
                    }
                }
            }

            //Jeżeli komórka ma mniej niż 2 lub więcej niż dwóch żywych sąsiadów, sama umiera
            if (current_field == 1) {
                if (neighbour_count < 2 || neighbour_count > 3) {
                    board[i][j] = 0;
                }
            } else
                //Jeżeli komórka ma dokładnie 3 żywych sąsiadów to sama też ożywa.
            {
                if (neighbour_count == 3) {
                    board[i][j] = 1;
                }
            }
        }
    }
}
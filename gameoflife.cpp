#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <random>
void clearConsole() {

}



const int COL = 30;
const int ROW = 30;

void create_board(int board[ROW][COL]) {
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            board[i][j] = 0;
        }
    }
}

void create_player(int board[ROW][COL]) {
    // std::default_random_engine generator;
    // std::uniform_int_distribution<int> distribution(1,ROW);
    // for (int i = 0; i < 25; i++) {
    //
    //     int dice_roll = distribution(generator);
    //     board[dice_roll][(dice_roll)] = 1;
    // }
    //W
    board[1][1] = 1;
    board[2][1] = 1;
    board[3][1] = 1;
    board[4][1] = 1;
    board[5][1] = 1;

    board[5][2] = 1;

    board[4][3] = 1;
    board[3][3] = 1;

    board[5][4] = 1;

    board[1][5] = 1;
    board[2][5] = 1;
    board[3][5] = 1;
    board[4][5] = 1;
    board[5][5] = 1;
    //I
    board[1][7] = 1;
    board[2][7] = 1;
    board[3][7] = 1;
    board[4][7] = 1;
    board[5][7] = 1;

    //E

    board[1][9] = 1;
    board[2][9] = 1;
    board[3][9] = 1;
    board[4][9] = 1;
    board[5][9] = 1;

    board[1][10] = 1;
    board[1][11] = 1;

    board[3][10] = 1;
    board[3][11] = 1;

    board[5][10] = 1;
    board[5][11] = 1;
    //Z

    board[1][13] = 1;
    board[1][14] = 1;
    board[1][15] = 1;
    board[1][16] = 1;

    board[2][16] = 1;
    board[3][15] = 1;

    board[3][14] = 1;

    board[4][13] = 1;

    board[5][13] = 1;
    board[5][14] = 1;
    board[5][15] = 1;
    board[5][16] = 1;

    //A

    board[1][19] = 1;
    board[1][20] = 1;
    board[2][18] = 1;
    board[2][21] = 1;
    board[3][18] = 1;
    board[3][21] = 1;
    board[3][19] = 1;
    board[3][20] = 1;
    board[4][18] = 1;
    board[4][21] = 1;
    board[5][18] = 1;
    board[5][21] = 1;

    //M

    board[8][3] = 1;
    board[9][3] = 1;
    board[10][3] = 1;
    board[11][3] = 1;
    board[12][3] = 1;

    board[8][4] = 1;

    board[9][5] = 1;
    board[10][5] = 1;

    board[8][6] = 1;

    board[8][7] = 1;
    board[9][7] = 1;
    board[10][7] = 1;
    board[11][7] = 1;
    board[12][7] = 1;

    //A

    board[8][9] = 1;
    board[8][10] = 1;
    board[9][8] = 1;
    board[9][11] = 1;
    board[10][8] = 1;
    board[10][11] = 1;
    board[10][9] = 1;
    board[10][10] = 1;
    board[11][8] = 1;
    board[11][11] = 1;
    board[12][8] = 1;
    board[12][11] = 1;

    //G

    board[9][12] = 1;
    board[10][12] = 1;
    board[11][12] = 1;


    board[8][13] = 1;
    board[8][14] = 1;
    board[8][15] = 1;

    board[12][13] = 1;
    board[12][14] = 1;
    board[11][15] = 1;
    board[12][15] = 1;

    board[10][15] = 1;
    board[10][14] = 1;

    //O

    board[8][17] = 1;
    board[8][18] = 1;
    board[9][16] = 1;
    board[10][16] = 1;
    board[11][16] = 1;
    board[12][17] = 1;
    board[12][18] = 1;
    board[9][19] = 1;
    board[10][19] = 1;
    board[11][19] = 1;

    //W

    board[8][20] = 1;
    board[9][20] = 1;
    board[10][20] = 1;
    board[11][20] = 1;
    board[12][20] = 1;

    board[12][21] = 1;
    board[11][22] = 1;
    board[10][22] = 1;

    board[12][23] = 1;

    board[8][24] = 1;
    board[9][24] = 1;
    board[10][24] = 1;
    board[11][24] = 1;
    board[12][24] = 1;

    board[29][13] = 1;
    board[28][13] = 1;
    board[27][13] = 1;
    board[26][13] = 1;
    board[25][13] = 1;
    board[24][13] = 1;
    board[23][13] = 1;
    board[22][13] = 1;
    board[21][13] = 1;

    board[20][12] = 1;
    board[19][12] = 1;
    board[18][12] = 1;
    board[18][13] = 1;
    board[19][14] = 1;
    board[18][15] = 1;
    board[18][16] = 1;

    board[23][15] = 1;
    board[23][16] = 1;
    board[24][15] = 1;
    board[24][16] = 1;

    board[19][17] = 1;

    board[18][18] = 1;
    board[18][19] = 1;
    board[19][19] = 1;
    board[20][19] = 1;

    board[29][18] = 1;
    board[28][18] = 1;
    board[27][18] = 1;
    board[26][18] = 1;
    board[25][18] = 1;
    board[24][18] = 1;
    board[23][18] = 1;
    board[22][18] = 1;
    board[21][18] = 1;

    board[17][12] = 1;
    board[17][13] = 1;
    board[17][18] = 1;
    board[17][19] = 1;

    board[28][24] = 1;
    board[27][24] = 1;
    board[26][24] = 1;
    board[25][24] = 1;
    board[24][23] = 1;
    board[24][22] = 1;
    board[29][22] = 1;
    board[24][21] = 1;
    board[25][21] = 1;
    board[26][21] = 1;
    board[27][21] = 1;
    board[28][21] = 1;
    board[29][21] = 1;
    board[29][23] = 1;

    board[28][29] = 1;
    board[29][28] = 1;
    board[29][27] = 1;
    board[28][26] = 1;
    board[27][28] = 1;
    board[26][27] = 1;
    board[25][26] = 1;
    board[24][27] = 1;
    board[24][28] = 1;
    board[25][29] = 1;

}

void print_board(int board[ROW][COL]) {

    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            if (board[i][j] == 1) {
                std::cout << " \u2588 ";
            } else {
                std::cout << " . ";
            }
        }
        std::cout << std::endl;
    }

}

void life(int board[ROW][COL]) {
    int tempBoard[ROW][COL]; // Stworzenie tymczasowej planszy do obliczeń

    // Skopiowanie bieżącej planszy
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            tempBoard[i][j] = board[i][j];
        }
    }

    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            int klocek = tempBoard[i][j];

            int sdzi = 0;
            for (int z = -1; z <= 1; z++) {
                for (int y = -1; y <= 1; y++) {
                    if (z == 0 && y == 0) continue; // Pomiń samą komórkę
                    if (i + z >= 0 && i + z < ROW && j + y >= 0 && j + y < COL) { // Sprawdź granice
                        int neighbour = tempBoard[i + z][j + y];
                        if (neighbour == 1) {
                            sdzi += 1;
                        }
                    }
                }
            }
            // Reguły gry
            if (klocek == 1) {
                if (sdzi < 2 || sdzi > 3) {
                    board[i][j] = 0; // Umiera
                }
            } else {
                if (sdzi == 3) {
                    board[i][j] = 1; // Rodzi się
                }
            }
        }
    }
}

int main() {
    system("cls");
    int plansza[ROW][COL];
    create_board(plansza);
    create_player(plansza);
    print_board(plansza);
    std::this_thread::sleep_for(std::chrono::milliseconds(4000)); // Czekaj 300 ms;
    while (true) {

        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Czekaj 300 ms;
        system("cls");
        print_board(plansza);
        life(plansza);
    }

    return 0;
}

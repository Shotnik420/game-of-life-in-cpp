#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <random>
#include <windows.h>
#include <mutex>
#include <semaphore>

const int COL = 30;
const int ROW = 30;
std::mutex mtx;
std::binary_semaphore semlg(1),sempg(0), semld(0), sempd(0), updown(2);


void create_board(int board[ROW][COL]) {
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            board[i][j] = 0;
        }
    }
}

void create_player(int board[ROW][COL]) {
    board[2][0] = 1;
    board[2][1] = 1;
    board[2][2] = 1;

    board[1][2] = 1;
    board[0][1] = 1;

}



void life(int board[ROW][COL]) {
    int tempBoard[ROW][COL];


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
                    if (z == 0 && y == 0) continue;
                    if (i + z >= 0 && i + z < ROW && j + y >= 0 && j + y < COL) { // Sprawdź granice
                        int neighbour = tempBoard[i + z][j + y];
                        if (neighbour == 1) {
                            sdzi += 1;
                        }
                    }
                }
            }
            if (klocek == 1) {
                if (sdzi < 2 || sdzi > 3) {
                    board[i][j] = 0;
                }
            } else {
                if (sdzi == 3) {
                    board[i][j] = 1;
                }
            }
        }
    }
}



void thread_print(int id,int board[ROW][COL],int startRow, int endRow, int startCol, int endCol,  std::binary_semaphore& mysem, std::binary_semaphore& nextsem) {

        for (int i = startRow; i < endRow; i++) {

            mysem.acquire();
            // std::cout<<std::to_string(id)+" ok i go ";
            std::string lineSegment = std::to_string(id)+ ":";
            for (int j = startCol; j < endCol; j++) {
                if (board[i][j] == 1) {
                    lineSegment += " # ";
                } else {
                    lineSegment += " . ";
                }
            }
            std::cout << lineSegment;
            if(endCol==COL) {
                std::cout << std::endl;
            }
            if((i==14 && endCol == COL)||(i==29 && endCol == COL)) {

            }else {
                nextsem.release();
            }

            // std::cout<<std::to_string(id) + " says: i give you pass, "
        }
        std::cout<<"im done " + std::to_string(id);
        updown.release();

}



void print_board(int board[ROW][COL]) {

    std::thread topLeft(thread_print, 1,board,0,15,0,15, std::ref(semlg), std::ref(sempg));
    std::thread topRight(thread_print,2, board,0,15,15,30, std::ref(sempg), std::ref(semlg));
    updown.acquire();
    updown.acquire();
    updown.acquire();
    updown.acquire();
    semld.release();
    std::thread bottomLeft(thread_print, 3,board,15,30,0,15, std::ref(semld), std::ref(sempd));
    std::thread bottomRight(thread_print,4,board,15,30,15,30, std::ref(sempd), std::ref(semld));
    topLeft.join();
    topRight.join();
    bottomLeft.join();
    bottomRight.join();
}

int update_board(int board[ROW][COL]) {
    semlg.release();
    print_board(board);

}

int main() {
    system("cls");
    int plansza[ROW][COL];
    create_board(plansza);
    create_player(plansza);
    print_board(plansza);
    Sleep(4000);
    while (true) {
        Sleep(500);
        system("cls");
        update_board(plansza);
        life(plansza);
    }

    return 0;
}

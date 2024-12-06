#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <random>
#include <windows.h>
#include <semaphore>
#include <fstream>
#include <vector>
#include <string>

const int COL = 30;
const int ROW = 30;

std::binary_semaphore semlg(1), sempg(0), semld(0), sempd(0), updown(2);


std::vector<std::vector<int>> board(ROW, std::vector<int>(COL, 0));

void import_board(const std::string& filename, std::vector<std::vector<int>>& board) {
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }

    // Read the file to determine the number of rows and columns
    std::vector<std::string> lines;
    while (getline(file, line)) {
        lines.push_back(line);
    }
    file.close();

    // Resize the board
    board.resize(ROW, std::vector<int>(COL, 0));

    // Populate the board with the values from the file
    for (int rowIdx = 0; rowIdx < ROW; ++rowIdx) {
        for (int colIdx = 0; colIdx < lines[rowIdx].size() && colIdx < COL; ++colIdx) {
            if (lines[rowIdx][colIdx] == '0' || lines[rowIdx][colIdx] == '1') {
                board[rowIdx][colIdx] = lines[rowIdx][colIdx] - '0';
            }
        }
    }
}

void life(std::vector<std::vector<int>>& board) {
    std::vector<std::vector<int>> tempBoard = board;

    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            int klocek = tempBoard[i][j];
            int sdzi = 0;
            for (int z = -1; z <= 1; z++) {
                for (int y = -1; y <= 1; y++) {
                    if (z == 0 && y == 0) continue;
                    if (i + z >= 0 && i + z < ROW && j + y >= 0 && j + y < COL) {
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

void thread_print(int id, std::vector<std::vector<int>>& board, int startRow, int endRow, int startCol, int endCol, std::binary_semaphore& mysem, std::binary_semaphore& nextsem) {
    for (int i = startRow; i < endRow; i++) {
        std::string lineSegment;
        for (int j = startCol; j < endCol; j++) {
            if (board[i][j] == 1) {
                lineSegment += "|#|";
            } else {
                lineSegment += "|_|";
            }
        }
        if (endCol == COL) {

            int dlug = lineSegment.size();
            lineSegment += std::to_string(dlug);
            lineSegment += "\n";
        }
        mysem.acquire();
        if ((i == (ROW / 2) && endCol == COL) || (i == ROW && endCol == COL)) {
        } else {
            std::cout << lineSegment;
            nextsem.release();
        }
    }
    updown.release();
}
void thread_print2(int id, std::vector<std::vector<int>>& board, int startRow, int endRow, int startCol, int endCol, std::binary_semaphore& mysem, std::binary_semaphore& nextsem) {
    for (int i = startRow; i < endRow; i++) {
        std::string lineSegment;
        for (int j = startCol; j < endCol; j++) {
            if (board[i][j] == 1) {
                lineSegment += "|#|";
            } else {
                lineSegment += "|_|";
            }
        }
        if (endCol == COL) {

            int dlug = lineSegment.size();
            lineSegment += std::to_string(dlug);
            lineSegment += "\n";
        }
        mysem.acquire();
        if ((i == (ROW / 2)-1 && endCol == COL) || (i == ROW && endCol == COL)) {
        } else {
            std::cout << lineSegment;
            nextsem.release();
        }
    }
    updown.release();
}
void print_board(std::vector<std::vector<int>>& board) {
    std::thread topLeft(thread_print, 1, std::ref(board), 0, 15, 0, 15, std::ref(semlg), std::ref(sempg));
    std::thread topRight(thread_print, 2, std::ref(board), 0, 15, 15, 30, std::ref(sempg), std::ref(semlg));
    updown.acquire();
    updown.acquire();
    updown.acquire();
    updown.acquire();
    semld.release();
    std::thread bottomLeft(thread_print2, 3, std::ref(board), 15, 30, 0, 15, std::ref(semld), std::ref(sempd));
    std::thread bottomRight(thread_print2, 4, std::ref(board), 15, 30, 15, 30, std::ref(sempd), std::ref(semld));
    topLeft.join();
    topRight.join();
    bottomLeft.join();
    bottomRight.join();
}

int main() {
    system("cls");
    system("color 86");
    import_board("pattern.txt", board);
    print_board(board);
    Sleep(2000);
    while (true) {
        Sleep(1000);
        system("cls");
        semlg.release();
        print_board(board);
        life(board);
    }

    return 0;
}

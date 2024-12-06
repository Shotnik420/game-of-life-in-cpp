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

int COL = 30;
int ROW = 30;

std:: binary_semaphore semlg(1), sempg(0), semld(0), sempd(0);
std::binary_semaphore updown(2);
std::binary_semaphore next_screen_sem(0);

std::vector<std::vector<int>> board(ROW, std::vector<int>(COL, 0));

void import_board(const std::string& filename, std::vector<std::vector<int>>& board) {
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }

    //
    std::vector<std::string> lines;
    while (getline(file, line)) {
        lines.push_back(line);
    }

    file.close();
    ROW = lines.size();

    //
    for (const auto& l : lines) {
        if (l.size() > COL) {
            COL = l.size();
        }
    }

    //
    board.resize(ROW, std::vector<int>(COL, 0));

    //
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
                lineSegment += " # ";
            } else {
                lineSegment += "   ";
            }
        }
        int length = lineSegment.length();
        if (endCol == COL) {
            lineSegment += "\n";
            length -=1;
        }


        mysem.acquire();

        std::cout << lineSegment;
        if ((i == ROW-1 && endCol == 30)||(i==(ROW/2)-1 && endCol == 30)){
        } else {
            nextsem.release();
        }
    }
    updown.release();
}


void print_board(std::vector<std::vector<int>>& board) {
    std::thread topLeft(thread_print, 1, std::ref(board), 0, ROW/2, 0, COL/2, std::ref(semlg), std::ref(sempg));
    std::thread topRight(thread_print, 2, std::ref(board), 0, ROW/2, COL/2,COL, std::ref(sempg), std::ref(semlg));
    updown.acquire();
    updown.acquire();
    updown.acquire();
    updown.acquire();
    semld.release();
    std::thread bottomLeft(thread_print, 3, std::ref(board), ROW/2, ROW, 0, COL/2, std::ref(semld), std::ref(sempd));
    std::thread bottomRight(thread_print, 4, std::ref(board), ROW/2, ROW, COL/2, COL, std::ref(sempd), std::ref(semld));
    topLeft.join();
    topRight.join();
    bottomLeft.join();
    bottomRight.join();
    next_screen_sem.release();
}


std::string menu() {
    std::cout << "   _____                               __   _      _  __                  \n";
    std::cout << "  / ____|                             / _| | |    (_)/ _|      _     _   \n";
    std::cout << " | |  __  __ _ _ __ ___   ___    ___ | |_  | |     _| |_ ___ _| |_ _| |_ \n";
    std::cout << " | | |_ |/ _` | '_ ` _ \\ / _ \\  / _ \\|  _| | |    | |  _/ _ \\_   _|_   _|\n";
    std::cout << " | |__| | (_| | | | | | |  __/ | (_) | |   | |____| | ||  __/ |_|   |_|  \n";
    std::cout << "  \\_____|\\__,_|_| |_| |_|\\___|  \\___/|_|   |______|_|_| \\___|             \n";
    std::cout << "                                                                          \n";
    std::cout << "                  Symulator Conway’s Game of Life w C++                  \n\n";
    std::cout << "                                      Stworzony przez Daniela Szote      \n\n\n\n";
    std::cout << "------ Menu ------\n";
    std::cout << "Wybierz jeden z przygotowanych wzorow, lub stworz wlasny edutujac plik twoj_pattern.txt\n";
    std::cout << "1. Pokaz 'Rozrost'\n";
    std::cout << "2. Pokaz 'Statki'\n";
    std::cout << "3. Pokaz 'Serce'\n";
    std::cout << "4. Twoj pattern \n";
    std::cout << "Wybierz opcje (1-4): ";
    int wybor;
    std::cin >> wybor;

    switch (wybor) {
        case 1:
            return "./patterns/rozrost.txt";
        case 2:
            return "./patterns/statki.txt";
        case 3:
            return "./patterns/serce.txt";
        case 4:
            return "./twoj_pattern.txt";
    }
}

int main() {
    system("color 86");
    std::string choice = menu();
    system("cls");
    import_board(choice, board);


    print_board(board);
    Sleep(2000);
    while (true) {
        next_screen_sem.acquire();
        system("cls");
        semlg.release();
        print_board(board);
        life(board);
        Sleep(100);
    }

    return 0;
}

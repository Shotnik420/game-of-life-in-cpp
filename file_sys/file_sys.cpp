#include<fstream>
#include <iostream>
#include <string>
#include <vector>
#include "filesys.h"

//Import planszy z pliku
void import_board(const std::string& filename, std::vector<std::vector<int>>& board, int ROW, int COL) {
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }

    //Zbieramy liczbę linijek
    std::vector<std::string> lines;
    while (getline(file, line)) {
        lines.push_back(line);
    }
    ROW = lines.size();
    file.close();


    //Zbieramy liczbe kolumn
    for (const auto& l : lines) {
        if (l.size() > COL) {
            COL = l.size();
        }
    }

    //Zmieniamy rozmiar tabeli na rozmiar pliku
    board.resize(ROW, std::vector<int>(COL, 0));

    //Wypełniamy tabele zerami i jedynkami
    for (int rowIdx = 0; rowIdx < ROW; ++rowIdx) {
        for (int colIdx = 0; colIdx < lines[rowIdx].size() && colIdx < COL; ++colIdx) {
            if (lines[rowIdx][colIdx] == '0' || lines[rowIdx][colIdx] == '1') {
                board[rowIdx][colIdx] = lines[rowIdx][colIdx] - '0';
            }
        }
    }
}

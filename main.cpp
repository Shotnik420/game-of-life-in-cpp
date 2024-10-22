#include <iostream>
#include <chrono>
#include <thread>
#include <windows.h>
#include <random>
void clearConsole() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = {0, 0}; // Ustawienie kursora na (0,0)
    DWORD written; // Liczba zapisanych znaków
    CONSOLE_SCREEN_BUFFER_INFO bufferInfo;

    GetConsoleScreenBufferInfo(hConsole, &bufferInfo);
    DWORD consoleSize = bufferInfo.dwSize.X * bufferInfo.dwSize.Y; // Rozmiar konsoli

    // Czyszczenie bufora
    FillConsoleOutputCharacter(hConsole, ' ', consoleSize, coord, &written);
    GetConsoleScreenBufferInfo(hConsole, &bufferInfo);
    // Przywrócenie pozycji kursora
    SetConsoleCursorPosition(hConsole, coord);
}



const int COL = 10;
const int ROW = 10;

void create_board(int board[ROW][COL]) {
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            board[i][j] = 0;
        }
    }
}

void create_player(int board[ROW][COL]) {
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(1,10);
    for (int i = 0; i < 15; i++) {

        int dice_roll = distribution(generator);
        board[dice_roll][(dice_roll)] = 1;
    }
}

void print_board(int board[ROW][COL]) {
    std::cout << "----------------------" << std::endl;
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            if (board[i][j] == 1) {
                std::cout << "OO";
            } else {
                std::cout << "[]";
            }
        }
        std::cout << std::endl;
    }
    std::cout << "----------------------" << std::endl;
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

    int plansza[ROW][COL];
    create_board(plansza);
    create_player(plansza);

    while (true) {
        print_board(plansza);
        std::this_thread::sleep_for(std::chrono::milliseconds(300)); // Czekaj 300 ms
        clearConsole();
        life(plansza);
    }

    return 0;
}

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

//Definicja rozmiaru tabeli
int COL = 30;
int ROW = 30;

//Definicja semaforów
std:: binary_semaphore semlg(1), sempg(0), semld(0), sempd(0);
std::binary_semaphore updown(2);
std::binary_semaphore next_screen_sem(0);

//Definicja planszy
std::vector<std::vector<int>> board(ROW, std::vector<int>(COL, 0));


//Import planszy z pliku
void import_board(const std::string& filename, std::vector<std::vector<int>>& board) {
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


//Symulacja życia
void life(std::vector<std::vector<int>>& board) {
    std::vector<std::vector<int>> tempBoard = board;

    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            int klocek = tempBoard[i][j];
            int sdzi = 0;
            //Bierzemy każdą komórkę po kolei
            for (int z = -1; z <= 1; z++) {
                for (int y = -1; y <= 1; y++) {
                    if (z == 0 && y == 0) continue;
                    if (i + z >= 0 && i + z < ROW && j + y >= 0 && j + y < COL) {
                        //Sprawdzamy każdego jej sąsiada
                        int neighbour = tempBoard[i + z][j + y];
                        //Jeżeli sąsiad jest "żywy" to dodajemy 1 do liczby żywych sąsiadów.
                        if (neighbour == 1) {
                            sdzi += 1;
                        }
                    }
                }
            }

            //Jeżeli komórka ma mniej niż 2 lub więcej niż dwóch żywych sąsiadów, sama umiera
            if (klocek == 1) {
                if (sdzi < 2 || sdzi > 3) {
                    board[i][j] = 0;
                }
            } else
                //Jeżeli komórka ma dokładnie 3 żywych sąsiadów to sama też ożywa.
                {
                if (sdzi == 3) {
                    board[i][j] = 1;
                }
            }
        }
    }
}

//Print dla wątków
void thread_print(int id, std::vector<std::vector<int>>& board, int startRow, int endRow, int startCol, int endCol, std::binary_semaphore& mysem, std::binary_semaphore& nextsem) {
    //Dla każdej komórki w wektorze board reprezentujemy zawartość za pomocą # lub pustką.
    for (int i = startRow; i < endRow; i++) {
        //Definiujemy segment linii do którego będziemy dodawać znaki.
        std::string lineSegment;
        for (int j = startCol; j < endCol; j++) {
            if (board[i][j] == 1) {
                lineSegment += " # ";
            } else {
                lineSegment += "   ";
            }
        }
        //Jeżeli jesteśmy na prawej krawędzi planszy, to przehodzimy do niższej linijki.
        if (endCol == COL) {
            lineSegment += "\n";
        }

        //Nasz segment lini jest gotowy, czekamy aż będziemy mogli go wypisać
        mysem.acquire();
        std::cout << lineSegment;
        if ((i == ROW-1 && endCol == 30)||(i==(ROW/2)-1 && endCol == 30)){
        } else {
            //Jeżeli linia którą wyprintowaliśmy nie kończy kratek górnych lub dolnych to wyzwalamy kolejny semafor. W innym przypadku nie wyzwalamy żadnego semafora
            //tutaj gdyż już dzieje się to w pętli main()
            nextsem.release();
        }
    }

        //Wyzwalamy semafor służący do przechodzenia z segmentu górnego na segment dolny i vice versa.
        updown.release();
}

//Główny "Manager" printowania
void print_board(std::vector<std::vector<int>>& board) {
    //Segment górny, dwa semafory updown, zostaną tu wydzielone.
    std::thread topLeft(thread_print, 1, std::ref(board), 0, ROW/2, 0, COL/2, std::ref(semlg), std::ref(sempg));
    std::thread topRight(thread_print, 2, std::ref(board), 0, ROW/2, COL/2,COL, std::ref(sempg), std::ref(semlg));

    updown.acquire();
    updown.acquire();
    updown.acquire();
    updown.acquire();

    semld.release();
    std::thread bottomLeft(thread_print, 3, std::ref(board), ROW/2, ROW, 0, COL/2, std::ref(semld), std::ref(sempd));
    std::thread bottomRight(thread_print, 4, std::ref(board), ROW/2, ROW, COL/2, COL, std::ref(sempd), std::ref(semld));

    //Czekamy aż wszystkie kratki zakończą działanie
    topLeft.join();
    topRight.join();
    bottomLeft.join();
    bottomRight.join();
    //Można kolejny raz printować!
    next_screen_sem.release();
}


//Definicja menu startowego
std::string menu(){
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
    //Fajny kolor na początek pętli
    system("color 86");
    //Wywołujemy menu i dostajemy ścieżkę do pliku wzoru
    std::string choice = menu();
    //czyścimy konsolę
    system("cls");
    //Wczytujemy plik
    import_board(choice, board);

    //Wypisujemy tabelę.
    print_board(board);
    //Czekamy 2 sekundy, zanim zaczniemy symulować
    Sleep(2000);
    while (true) {
        //Czekamy aż semafor kolejnego printu zostanie wyzwolony
        next_screen_sem.acquire();
        //Czyścimy konsolę
        system("cls");
        //Wyzwalamy semafor lewej górnej kratki
        semlg.release();
        //Printujemy planszę
        print_board(board);
        //Symulujemy życie
        life(board);
        //Czekamy jeszcze 0.1 sekundy, na wszelki wypadek
        Sleep(100);
    }

    return 0;
}

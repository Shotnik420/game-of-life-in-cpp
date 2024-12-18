/**
* @file main.cpp
* @brief Główna funkcja programu implementującego grę w życie.
*
* Program dzieli planszę na segmenty i używa semaforów do zarządzania wątkami,
* które wyświetlają fragmenty planszy. Program symuluje grę w życie i
* aktualizuje planszę w każdej iteracji.
**/
#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <random>
#include <windows.h>
#include <semaphore>
#include <vector>
#include <string>
#include "./file_sys/filesys.h"
#include "menu/menu.h"
#include "simulation.h"
//Definicja rozmiaru tabeli
int COL = 30;
int ROW = 30;

//Definicja semaforów
std:: binary_semaphore semLeftUp(1), semRightUp(0), semLeftDown(0), semRightDown(0);
std::binary_semaphore updown(2);
std::binary_semaphore next_screen_sem(0);

//Definicja planszy
std::vector<std::vector<int>> board(ROW, std::vector<int>(COL, 0));




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
    std::thread topLeft(thread_print, 1, std::ref(board), 0, ROW/2, 0, COL/2, std::ref(semLeftUp), std::ref(semRightUp));
    std::thread topRight(thread_print, 2, std::ref(board), 0, ROW/2, COL/2,COL, std::ref(semRightUp), std::ref(semLeftUp));

    updown.acquire();
    updown.acquire();
    updown.acquire();
    updown.acquire();

    semLeftDown.release();
    std::thread bottomLeft(thread_print, 3, std::ref(board), ROW/2, ROW, 0, COL/2, std::ref(semLeftDown), std::ref(semRightDown));
    std::thread bottomRight(thread_print, 4, std::ref(board), ROW/2, ROW, COL/2, COL, std::ref(semRightDown), std::ref(semLeftDown));

    //Czekamy aż wszystkie kratki zakończą działanie
    topLeft.join();
    topRight.join();
    bottomLeft.join();
    bottomRight.join();
    //Można kolejny raz printować!
    next_screen_sem.release();
}

int main() {
    //Fajny kolor na początek pętli
    system("color 86");
    //Wywołujemy menu i dostajemy ścieżkę do pliku wzoru
    std::string choice = menu();
    //czyścimy konsolę
    system("cls");
    //Wczytujemy plik
    import_board(choice, board, ROW, COL);

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
        semLeftUp.release();
        //Printujemy planszę
        print_board(board);
        //Symulujemy życie
        life(board, ROW, COL);
        //Czekamy jeszcze 0.1 sekundy, na wszelki wypadek
        Sleep(100);
    }

    return 0;
}

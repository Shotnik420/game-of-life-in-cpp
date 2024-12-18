#include <iostream>
#include "menu.h"
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
    std::cout << "Wybierz jeden z przygotowanych wzorow\n";
    std::cout << "1. Pokaz 'Rozrost'\n";
    std::cout << "2. Pokaz 'Statki'\n";
    std::cout << "3. Pokaz 'Serce'\n";
    std::cout << ", lub wpisz sciezke do pliku,(domyslnie './patterns/nazwa_pliku.txt');\n";
    std::cout << "Wybierz opcje (1-3) lub sciezka: ";
    std::string choose_str;
    std::cin >> choose_str;

    if (choose_str == "1") {
        return "./patterns/rozrost.txt";
    }
    else if (choose_str == "2") {
        return "./patterns/statki.txt";
    }
    else if (choose_str == "3") {
        return "./patterns/serce.txt";
    }
    else if (choose_str[0] == '.') {  // Sprawdzenie, czy zaczyna się od kropki
        return choose_str;  // Zakładamy, że wpisuje całą ścieżkę
    }
    else {
        std::cerr << "Niepoprawny wybor!" << std::endl;
        return "";  // Zwracamy pusty string dla błędnego wyboru
    }
}


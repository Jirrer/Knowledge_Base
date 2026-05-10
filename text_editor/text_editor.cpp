#include <iostream>
#include <vector>
#include <conio.h> 
#include <windows.h>
#include <tuple>
#include "text_editor.h"

struct Letter {
    char value;
    Letter* prev;
    Letter* next;
    bool deleted;

    Letter(char val, Letter* previous) : value(val), prev(previous), next(nullptr), deleted(false) {}
};

std::vector<Letter> content;
int currIndex = 0;

Letter firstLetter('~', nullptr);

void openEditor() {
    int charInput;
    Letter* lastLetter = &firstLetter;

    printToScreen();

    while ((charInput = readKeyPress()) >= 0) {
        if (charInput == 8) { // Delete
            if (!lastLetter->prev) { continue;}
            
            lastLetter->deleted = true; 
            lastLetter = lastLetter->prev;  
        } else {
            Letter* newLetter = new Letter(charInput, lastLetter);
            
            lastLetter -> next = newLetter;
            lastLetter = newLetter;
        }

        printToScreen();
    }
}

char readKeyPress() {
    char keyPress = _getch();

    switch (keyPress) {
        case 3: 
            return -1; // Ctrl+C

        case 8: // Backspace
            return 8;

        case 10:
            return 10;

        case 13:
            return 10;

    }

    return keyPress;
}

// std::tuple<int,int> getTerminalSize_windows() { // fix swappend x and y
//     CONSOLE_SCREEN_BUFFER_INFO csbi;

//     if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
//         int columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
//         int rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
        
//         return std::make_tuple(rows, columns);
//     }

//     return std::make_tuple(-1, -1);
// }

void refreshScreen() {
    system("cls");
}

void printToScreen() {
    // refreshScreen();
    std::cout << std::endl;

    Letter* l = &firstLetter;
    
    while (l -> next) {
        if (!l->deleted) { std::cout << l->value; }
        
        l = l -> next;  
    }

    if (!l->deleted) { std::cout << l->value; }
}
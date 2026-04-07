#include <iostream>
#include <vector>
#include <conio.h> 
#include <windows.h>
#include <tuple>
#include "text_editor.h"

struct Letter {
    int index;
    char value;
};

std::vector<Letter> content;
int currIndex = 0;

void openEditor() {
    drawRows();

    int c;

    while ((c = readKeyPress()) >= 0) {
        if (c == 0) { continue; }

        if (c == 8) {
            if (currIndex > 0) {
                content.erase(content.begin() + (currIndex - 1));
                currIndex--;
                drawRows();
            }
            continue;
        }

        if (c == 9) {
            for (int x = 0; x < 5; x++) {
                Letter l;
                l.index = currIndex;
                l.value = ' ';

                content.push_back(l);
                currIndex++;
            }

            continue;
        }

        processKeyPress(c);
    }
}

int readKeyPress() {
    int keyPress = _getch();

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

void processKeyPress(char keyPress) {
    Letter letter;
    letter.value = keyPress;
    letter.index = currIndex + 1;

    content.insert(content.begin() + currIndex, letter);
    currIndex++;

    drawRows();

}

std::tuple<int,int> getTerminalSize_windows() { // fix swappend x and y
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        int columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        int rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
        
        return std::make_tuple(rows, columns);
    }

    return std::make_tuple(-1, -1);
}

void refreshScreen_windows() {
    system("cls");
}

void drawRows() {
    refreshScreen_windows();
    std::cout << "~ ";

    std::tuple<int, int> screenSize = getTerminalSize_windows();
    int screenLength = std::get<0>(screenSize);
    int screenHeight = std::get<1>(screenSize);

    int printedRows = 0;
    int lineIndex = 0;
    size_t contentIndex = 0;
    while (printedRows < screenHeight){
        if (lineIndex == screenLength - 5) { 
            std::cout << "\n~ "; 
            printedRows++;
            lineIndex = 0;
        }

        lineIndex++;
        
        if (contentIndex < content.size()) {
            if (content[contentIndex].value == 10) {
                printedRows++;
                lineIndex = 0;
            } else {
                std::cout << content[contentIndex].value;
            }

        }

        contentIndex++;


    }
}
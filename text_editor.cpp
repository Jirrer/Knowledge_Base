#include <iostream>
#include <vector>
#include <conio.h> 
#include "text_editor.h"

std::vector<std::string> openEditor() {
    Letter* head = nullptr;
    Letter* tail = nullptr;

    while (true) {
        char charInput = _getch();

        if (charInput == 27) { break; }


        Letter* newLetter = new Letter(charInput);

        if (!head) {
            head = newLetter;
            tail = newLetter;
        } else {
            tail->next = newLetter;
            tail = newLetter;
        }

        printConsole(head);
    }


    std::vector<std::string> output;
    return output;
}

void printConsole(Letter* head) {
    system("cls"); // To-Do: add linux option

    Letter* current = head;
    while (current) {
        std::cout << static_cast<char>(current->data);
        current = current->next;
    }

}
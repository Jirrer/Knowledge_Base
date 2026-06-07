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

// To-Do: deleting a letter keeps it in the struct the cursorLocation is buggy

std::vector<Letter> content;
int currIndex = 0;

Letter firstLetter('~', nullptr);
Letter* cursorLocation = &firstLetter;

void openEditor() {
    int charInput;

    printToScreen();

    while ((charInput = _getch()) != 3) {
        if (charInput == 224 || charInput == 0) {  // Escape sequence
            charInput = _getch();  // Get the actual arrow key code
        }
        
        switch (charInput) {
            case 8: // Backspace
                if (!cursorLocation->prev) { continue;}
                
                cursorLocation->deleted = true; 
                cursorLocation = cursorLocation->prev; 
                
                break;

            case 72: // Up Arrow
                

                break;

            case 80: // Down Arrow


                break;
            
            case 75: // Left Arrow
                if (cursorLocation->prev && !cursorLocation->prev->deleted) {
                    cursorLocation = cursorLocation->prev;
                }

                break;


            case 77: // Right Arrow
                if (cursorLocation->next && !cursorLocation->prev->deleted) {
                    cursorLocation = cursorLocation->next;
                }

                break;
        
            default:
                Letter* newLetter = new Letter(charInput, cursorLocation);

                newLetter->next = cursorLocation->next;
                        
                cursorLocation -> next = newLetter;
                cursorLocation = newLetter;

                break;
        }
        
        printToScreen();
    }
}


void refreshScreen() {
    system("cls");
}

void printToScreen() {
    refreshScreen();

    Letter* l = &firstLetter;
    
    while (l -> next) {
        if (l == cursorLocation) {

            std::cout << l->value << ']';
            
        }


        else if (!l->deleted) { std::cout << l->value; }
        
        l = l -> next;  
    }

    if (!l->deleted) { 
        if (l == cursorLocation) { std::cout << l->value << ']'; } 
        else { std::cout << l->value; }
    }
}
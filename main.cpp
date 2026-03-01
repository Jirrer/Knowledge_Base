#include "functions.h"
#include <iostream>

using namespace std;

enum Choice {
    exit_program,
    search_base,
};

int main() {
    enum Choice choice;
    
    while (choice != exit_program) {
        cout << "Ran loop";

        choice = exit_program;

        switch (choice) {
            case exit_program: exit;
            case search_base: showSearchResults(searchKnowledgeBase("")); 
        }
    }

    return 0;
}

void showSearchResults(string searchOutput) {
    cout << searchOutput;
}

string searchKnowledgeBase(string searchInput) {


    return "test";
}


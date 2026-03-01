#include <iostream>
#include <conio.h>
#include <vector>

using namespace std;

enum Choice {
    exit_program,
    search_base,
};

void clearTerminal();
Choice getChoice(string choiceInput);
void searchKnowledgeBase(string searchInput);
void showSelectedOutput(string resultsID);

void clearTerminal() {
    // To-Do: accept linux as an option

    system("cls");
}

int main() {
    enum Choice choice;
    string userInput;
    
    while (choice != exit_program) {
        cout << ":";

        getline(cin, userInput);

        choice = getChoice(userInput);

        clearTerminal();

        switch (choice) {
            case exit_program: exit;
            case search_base: searchKnowledgeBase(userInput); 
        }
    }

    return 0;
}

Choice getChoice(string choiceInput) {
    if (choiceInput == "leave") {
        return Choice::exit_program;

    } else {
        return Choice::search_base;
    }
}

void searchKnowledgeBase(string searchInput) {
    // To-Do: fuzzy search knowledge base and show
    // results (might move to another file)
    vector<string> searchResults = {
        "copying working directory in windows",
        "traversing a binary tree",
        "copying working directory in windows"
    };

    clearTerminal();

    cout << "*** Search Results ***" << endl;

    for (string result : searchResults) {
        cout << result << endl;
    }

    cout << "\033[2;1H";

    int lineIndex = 0;
    while (true) {
        int keySelection = _getch();

        if (keySelection == 13) { break; }

        // To-Do: put cursor at the end of the text
        
        if (keySelection == 0 || keySelection == 224) {
            int arrowDirection = _getch();
            
            if (arrowDirection == 72 and lineIndex > 0) { 
                lineIndex--; 
                cout << "\033[1A";
            }
            else if (arrowDirection == 80 and lineIndex < searchResults.size() - 1) { 
                lineIndex++; 
                cout << "\033[1B";
            } 
        }
    }

    showSelectedOutput(searchResults.at(lineIndex));
}



// Move cursor up 1 line
// std::cout << "\033[1A";
// Move cursor down 1 line
// std::cout << "\033[1B";
// Move cursor right 1 position
// std::cout << "\033[1C";


void showSelectedOutput(string resultsID) {
    clearTerminal();

    cout << resultsID << endl;
    cout << "example data" << endl;
}



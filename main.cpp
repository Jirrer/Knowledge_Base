#include <iostream>
#include <conio.h>
#include <vector>
#include "query.h"
#include "database.h"

using namespace std;

enum User_Choice {
    exit_program,
    search_base,
    add,
    search_by_category,
};

enum Direction_Choice {
    in,
    out,
    leave,   
};

// To-Do: add a quite hotkey (crl+c) for any and all input
// To-Do: add search algorithm
// To-Do: change the name for 'add'
// To-Do: add 'esc' as a back button
// To-Do: make it so adding text (or editing in the future) acts a text editior

void clearTerminal();
User_Choice getChoice(string choiceInput);
void searchKnowledgeBase(vector<string> searchResults);
void showSelectedOutput(string domainName, vector<string> oldSearchResults);
void addToKnowledgeBase();
bool pushChanges(string name, string category, vector<string> text);
Direction_Choice getDirectionChoice();

void clearTerminal() {
    // To-Do: accept linux as an option

    system("cls");
}

int main() {
    enum User_Choice choice;
    string userInput;
    
    while (choice != exit_program) {
        clearTerminal();
        
        cout << " *** Search *** " << endl << ": ";

        getline(cin, userInput);

        choice = getChoice(userInput);

        switch (choice) {
            case exit_program: exit(1); break;
            case add: addToKnowledgeBase(); break;
            case search_by_category: searchKnowledgeBase(queryKnowledgeBaseByCategory(userInput)); break;
            case search_base: searchKnowledgeBase(queryKnowledgeBase(userInput)); break;
        }
    }

    return 0;
}

User_Choice getChoice(string choiceInput) {
    if (choiceInput == "leave") {
        return User_Choice::exit_program;
    }

    else if (choiceInput == "add") {
        return User_Choice::add;
    
    } 

    else if (choiceInput[0] == '_' && choiceInput[1] == '_') {
        return User_Choice::search_by_category;
    }
    
    else {
        return User_Choice::search_base;
    }
}

void searchKnowledgeBase(vector<string> searchResults) {
    clearTerminal();

    cout << "*** Search Results ***" << endl;

    if (searchResults.size() == 0) {
        cout << "No Results Found" << endl;

        cout << "<-- Return";

        Direction_Choice choice = getDirectionChoice();

        if (choice == Direction_Choice::in || choice == Direction_Choice::out) {
            return;
        }
    }

    for (string result : searchResults) {
        cout << result << endl;
    }

    cout << "\033[2;1H";

    int lineIndex = 0;
    while (true) {
        int keySelection = _getch();
        
        if (keySelection == 3) { clearTerminal(); exit(0); }
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

    if (searchResults.size() > 0) { showSelectedOutput(searchResults.at(lineIndex), searchResults); }
    else {showSelectedOutput("", searchResults); }

}

void showSelectedOutput(string domainName, vector<string> oldSearchResults) {
    clearTerminal();

    cout << domainName << endl << endl;

    cout << pullContentFromKeys(domainName) << endl << endl;

    cout << "-- New Search --";

    Direction_Choice choice = getDirectionChoice();

    if (choice == Direction_Choice::in) { return; }
    if (choice == Direction_Choice::out) { searchKnowledgeBase(oldSearchResults); }
}

void addToKnowledgeBase() {
    string name;
    string category;
    vector<string> text;

    cout << "Enter Name: ";
    getline(cin, name);

    cout << "Enter Category: ";
    getline(cin, category);

    cout << "Enter Text content (enter '.' to end) -->" << endl;
    
    string line;
    while (getline(cin, line) && line != ".") {
        text.push_back(line);
    }
 
    if (pushChanges(name, category, text)) { cout << "Successfully Added" << endl; }
    else { cout << "Error Trying to Add" << endl; }

    Direction_Choice choice = getDirectionChoice();

    if (choice == Direction_Choice::in || choice == Direction_Choice::out) {
        return;
    }
}

bool pushChanges(string name, string category, vector<string> text) {
    if (name.empty()) { return false; }

    string content;

    if (text.empty()) {
        content = "No Content to Show";
    } else {
        content = text[0];
        for (size_t i = 1; i < text.size(); i++) {
            content += "\n" + text[i];
        }
    }

    tuple<string, string, string> payload(name, category, content);

    return insertIntoKeys(payload);
}

Direction_Choice getDirectionChoice() {
    while (true) {
        char input = _getch();

        if (input == '\r') {
            return Direction_Choice::in;
        } 

        else if (input == 27) {
            return Direction_Choice::out;
        }

        else if (input == 3) {
            exit(0); // To-Do: maybe clear terminal here
        }
    }
}
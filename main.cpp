#include <iostream>
#include <conio.h>
#include <vector>
#include <fstream>
#include "query.h"
#include "database.h"
#include <cstdlib>

using namespace std;

enum User_Choice {
    exit_program,
    search_base,
    add,
    search_by_category,
    help,
};

enum Direction_Choice {
    in,
    out,
    leave,   
};

// To-Do: add search algorithm
// To-Do: make it so adding text (or editing in the future) acts a text editior
// To-Do: add delete entry

// Helper Methods
void clearTerminal();
User_Choice getChoice(string choiceInput);
Direction_Choice getDirectionChoice();
void loadDotEnv(const std::string& filename);

// Methods After Main
void searchKnowledgeBase(vector<string> searchResults);
void showSelectedOutput(string domainName, vector<string> oldSearchResults);
void addToKnowledgeBase();
bool pushChanges(string name, string category, vector<string> text);
void printHelpLibrary();

void clearTerminal() {
    // To-Do: accept linux as an option

    system("cls");
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

    else if (choiceInput.size() == 1 && choiceInput[0] == '?') {
        return User_Choice::help;
    }
    
    else {
        return User_Choice::search_base;
    }
}

Direction_Choice getDirectionChoice() {
    while (true) {
        char input = _getch();

        if (input == '\r') { // enter
            return Direction_Choice::in;
        } 

        else if (input == 27) { // esc
            return Direction_Choice::out;
        }

        else if (input == 3) { // ctrl+c
            exit(0); // To-Do: maybe clear terminal here
        }
    }
}

void loadDotEnv(const std::string& filename = ".env") {
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
    if (line.empty() || line[0] == '#') continue;
    auto eq = line.find('=');
    if (eq == std::string::npos) continue;

    string key = line.substr(0, eq);
    string value = line.substr(eq + 1);

    #ifdef _WIN32
    _putenv_s(key.c_str(), value.c_str());
    #else
    setenv(key.c_str(), value.c_str(), 1);
    #endif
    }
}

int main() {
    enum User_Choice choice = search_base;
    string userInput;

    loadDotEnv();
    
    while (choice != exit_program) {
        clearTerminal();
        
        cout << " *** Search *** " << endl << ": ";

        getline(cin, userInput);

        choice = getChoice(userInput);

        switch (choice) {
            case exit_program: exit(1); break;
            case add: addToKnowledgeBase(); break;
            case search_by_category: searchKnowledgeBase(queryKnowledgeBaseByCategory(userInput)); break;
            case help: printHelpLibrary(); break;
            case search_base: searchKnowledgeBase(queryKnowledgeBase(userInput)); break;
        }
    }

    return 0;
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

    cout << "\033[2;1H" << "\033[" << searchResults[0].size() << "C";

    int lineIndex = 0;
    while (true) {
        int keySelection = _getch();
        
        if (keySelection == 27) { return; } // esc
        if (keySelection == 3) { clearTerminal(); exit(0); } // ctrl+c
        if (keySelection == 13) { break; } // enter
        
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
            
            cout << "\r";
            cout << "\033[" << searchResults[lineIndex].size() << "C";
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


    if (getDirectionChoice()) {
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

void printHelpLibrary() { // To-Do: add a specific help (like help python)
    clearTerminal(); 

    cout << "*** Help Library ***" << endl << endl;
    cout << "(?) - Show Help Library" << endl;
    cout << "(Example Search) - Search Knowledge Base" << endl;
    cout << "(__exampleCategory) - Search by category" << endl;
    cout << "(crtl+c) - End Program" << endl;
    cout << "(add) - Add New Entry" << endl;

    if (getDirectionChoice()) {
        return;
    }
}
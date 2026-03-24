#include <iostream>
#include <conio.h>
#include <vector>
#include "query.h"
#include "sqlite3.h"

using namespace std;

enum Choice {
    exit_program,
    search_base,
    add,
};

// To-Do: add a quite hotkey (crl+c) for any and all input

void clearTerminal();
Choice getChoice(string choiceInput);
void searchKnowledgeBase(string searchInput);
void showSelectedOutput(string resultsID);
void addToKnowledgeBase();
bool pushChanges(string name, string category, vector<string> text);

void clearTerminal() {
    // To-Do: accept linux as an option

    system("cls");
}

int main() {
    enum Choice choice;
    string userInput;
    
    while (choice != exit_program) {
        clearTerminal();
        
        cout << " *** Search *** " << endl << ": ";

        getline(cin, userInput);

        choice = getChoice(userInput);

        switch (choice) {
            case exit_program: exit(1);
            case add: addToKnowledgeBase();
            case search_base: searchKnowledgeBase(userInput); 
        }
    }

    return 0;
}

Choice getChoice(string choiceInput) {
    if (choiceInput == "leave") {
        return Choice::exit_program;
    }

    else if (choiceInput == "add") {
        return Choice::add;
    
    } 
    
    else {
        return Choice::search_base;
    }
}

void searchKnowledgeBase(string searchInput) {
    vector<string> searchResults = queryKnowledgeBase(searchInput);

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

    if (searchResults.size() > 0) { showSelectedOutput(searchResults.at(lineIndex)); }
    else {showSelectedOutput(""); }

}

void showSelectedOutput(string resultsID) {
    clearTerminal();

    // To-Do: access content corrisponding to the key

    cout << resultsID << endl;
    cout << "example data" << endl;

    cout << "<-- Return";

    while (true) {
        char input = _getch();

        if (input == '\r') {
            return;
        }
    }
}

void addToKnowledgeBase() {
    string name;
    string category;
    vector<string> text;

    cout << "Enter Name: ";
    getline(cin, name);

    cout << "Enter Category: ";
    getline(cin, category);

    cout << "Enter Text Contnet (enter '.' to end) -->" << endl;
    
    string line;
    while (getline(cin, line) && line != ".") {
        text.push_back(line);
    }

    cout << name << endl;
    cout << category << endl;
 
    if (pushChanges(name, category, text)) { cout << "Successfully Added" << endl; }
    else { cout << "Error Trying to Add" << endl; }

    while (true) {
        char input = _getch();

        if (input == '\r') {
            return;
        }
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

    sqlite3* db;

    int rc = sqlite3_open("Knowledge_Base.db", &db);

    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    } 

    sqlite3_stmt* stmt;

    const char* sql = 
        "INSERT INTO keys (domain, category, content) VALUES (?, ?, ?);";

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            std::cerr << "Prepare failed: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_close(db);
            return false;
        }

    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, category.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, content.c_str(), -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Insert failed: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return rc == SQLITE_DONE;
}
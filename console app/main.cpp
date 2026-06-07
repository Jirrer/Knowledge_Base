#include <iostream>
#include <conio.h>
#include <vector>
#include <fstream>
#include <string>
#include <tuple>
#include "query.h"
#include "database.h"
#include <cstdlib>
// #include "text_editor.h"

enum User_Choice {
    exit_program,
    search_base,
    add,
    search_by_category,
    help,
    del,
};

enum Direction_Choice {
    in,
    out,
    leave,   
};

// To-Do: add delete entry
// To-DO: normilize text before generating methephone codes

void clearTerminal();
User_Choice getChoice(std::string choiceInput);
Direction_Choice getDirectionChoice();
void loadDotEnv(const std::string& filename);
void searchKnowledgeBase(std::vector<std::string> searchResults);
void showSelectedOutput(std::string domainName, std::vector<std::string> oldSearchResults);
void addToKnowledgeBase();
bool pushChanges(std::string name, std::string category, std::vector<std::string> text);
void printHelpLibrary();
void removeFromKnowledgeBase();
bool checkValidTitleAndCategory(std::string title, std::string category);

void clearTerminal() {
    // To-Do: accept linux as an option

    std::system("cls");
}

User_Choice getChoice(std::string choiceInput) {
    if (choiceInput == "leave") {
        return User_Choice::exit_program;
    }

    else if (choiceInput == "add") {
        return User_Choice::add;
    } 

    else if (choiceInput == "del") {
        return User_Choice::del;
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
            std::exit(0); // To-Do: maybe clear terminal here
        }
    }
}

void loadDotEnv(const std::string& filename = ".env") {
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        auto eq = line.find('=');
        if (eq == std::string::npos) continue;

        std::string key = line.substr(0, eq);
        std::string value = line.substr(eq + 1);

        #ifdef _WIN32
        _putenv_s(key.c_str(), value.c_str());
        #else
        setenv(key.c_str(), value.c_str(), 1);
        #endif
    }
}

int main() {
    enum User_Choice choice = search_base;
    std::string userInput;

    loadDotEnv();
    
    while (choice != exit_program) {
        clearTerminal();
        
        std::cout << " *** Search *** " << std::endl << ": ";

        std::getline(std::cin, userInput);

        choice = getChoice(userInput);

        switch (choice) {
            case exit_program: std::exit(1); break;
            case add: addToKnowledgeBase(); break;
            case search_by_category: searchKnowledgeBase(queryKnowledgeBaseByCategory(userInput)); break;
            case help: printHelpLibrary(); break;
            case search_base: searchKnowledgeBase(queryKnowledgeBase(userInput)); break;
            case del: removeFromKnowledgeBase(); break; 
        }
    }

    return 0;
}

void searchKnowledgeBase(std::vector<std::string> searchResults) { // To-Do: have it print the category as - title (category)
    clearTerminal();

    std::cout << "*** Search Results ***" << std::endl;

    if (searchResults.size() == 0) {
        std::cout << "No Results Found" << std::endl;

        std::cout << "<-- Return";

        Direction_Choice choice = getDirectionChoice();

        if (choice == Direction_Choice::in || choice == Direction_Choice::out) {
            return;
        }
    }

    for (std::string result : searchResults) {
        std::cout << result << std::endl;
    }

    std::cout << "\033[2;1H" << "\033[" << searchResults[0].size() << "C";

    int lineIndex = 0;
    while (true) {
        int keySelection = _getch();
        
        if (keySelection == 27) { return; } // esc
        if (keySelection == 3) { clearTerminal(); std::exit(0); } // ctrl+c
        if (keySelection == 13) { break; } // enter
        
        if (keySelection == 0 || keySelection == 224) {
            int arrowDirection = _getch();
            
            if (arrowDirection == 72 && lineIndex > 0) { 
                lineIndex--; 
                std::cout << "\033[1A";
            }
            else if (arrowDirection == 80 && lineIndex < searchResults.size() - 1) { 
                lineIndex++; 
                std::cout << "\033[1B";
            } 
            
            std::cout << "\r";
            std::cout << "\033[" << searchResults[lineIndex].size() << "C";
        }
    }

    if (searchResults.size() > 0) { showSelectedOutput(searchResults.at(lineIndex), searchResults); }
    else {showSelectedOutput("", searchResults); }

}

void showSelectedOutput(std::string domainName, std::vector<std::string> oldSearchResults) {
    clearTerminal();

    std::cout << domainName << std::endl << std::endl;

    std::cout << pullContentFromKeys(domainName) << std::endl << std::endl;

    std::cout << "-- New Search --";

    Direction_Choice choice = getDirectionChoice();

    if (choice == Direction_Choice::in) { return; }
    if (choice == Direction_Choice::out) { searchKnowledgeBase(oldSearchResults); }
}

void addToKnowledgeBase() {
    std::string name;
    std::string category;
    std::vector<std::string> text;

    std::cout << "Enter Title: ";
    std::getline(std::cin, name);

    std::cout << "Enter Category: ";
    std::getline(std::cin, category);

    std::cout << "Enter Text content (enter '.' to end) -->" << std::endl;
    
    std::string line;
    while (std::getline(std::cin, line) && line != ".") {
        text.push_back(line);
    }
 
    if (pushChanges(name, category, text)) { std::cout << "Successfully Added" << std::endl; }
    else { std::cout << "Error Trying to Add" << std::endl; }


    if (getDirectionChoice()) { return; }
}

bool pushChanges(std::string name, std::string category, std::vector<std::string> text) {
    if (name.empty()) { return false; }

    std::string content;

    if (text.empty()) {
        content = "No Content to Show";
    } else {
        content = text[0];
        for (size_t i = 1; i < text.size(); i++) {
            content += "\n" + text[i];
        }
    }

    std::tuple<std::string, std::string, std::string> payload(name, category, content);

    return insertIntoKeys(payload);
}

void removeFromKnowledgeBase() {
    clearTerminal();

    std::string title;
    std::string category;

    std::cout << "Enter Title: ";
    std::getline(std::cin, title);

    std::cout << "Verify Category: ";
    std::getline(std::cin, category);

    if (checkValidTitleAndCategory(title, category) == false) {
        std::cout << "Category does not match Title";
        return;
    }

    if (removeFromDatabase(title, category)) { std::cout << "Sucessfully removed " << title << " (" << category << ")"; }
    else {std::cout << "Error removing " << title << " (" << category << ")"; }

    if (getDirectionChoice()) { return; }
}


bool checkValidTitleAndCategory(std::string title, std::string category) {


    return true;
}

void printHelpLibrary() { // To-Do: add a specific help (like help python)
    clearTerminal(); 

    std::cout << "*** Help Library ***" << std::endl << std::endl;
    std::cout << "(?) - Show Help Library" << std::endl;
    std::cout << "(Example Search) - Search Knowledge Base" << std::endl;
    std::cout << "(__exampleCategory) - Search by category" << std::endl;
    std::cout << "(crtl+c) - End Program" << std::endl;
    std::cout << "(add) - Add New Entry" << std::endl;

    if (getDirectionChoice()) {
        return;
    }
}
#include <iostream>

using namespace std;

enum Choice {
    exit_program
};

int main() {
    enum Choice choice;
    
    while (choice != exit_program) {
        cout << "Ran loop";

        choice = exit_program;
    }

    return 0;
}


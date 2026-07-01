#include <string>
#include <vector>
#include <iostream>
#include "text_editor.h"

int main() {
    // Command to open Microsoft Edit with a specific file
    std::string command = "edit temp_submission.txt";

    // Execute the command
    int result = std::system(command.c_str());

    if (result == 0) {
        std::cout << "Microsoft Edit closed normally." << std::endl;
    } else {
        std::cout << "Failed to open Microsoft Edit. Ensure it is installed." << std::endl;
    }

    return 1;
}
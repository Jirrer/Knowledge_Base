#pragma once
#include <string>
#include <vector>

struct Letter {
    int data;
    Letter* next;

    Letter(int val) : data(val), next(nullptr) {}
};

std::vector<std::string> openEditor();
void printConsole(Letter* head);
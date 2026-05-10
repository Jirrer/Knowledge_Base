#pragma once
#include <string>
#include <vector>
#include <tuple>

void openEditor();
char readKeyPress();
void processKeyPress(char keyPress);
std::tuple<int,int> getTerminalSize_windows();
void printToScreen();

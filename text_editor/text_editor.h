#pragma once
#include <string>
#include <vector>
#include <tuple>

void openEditor();
int readKeyPress();
void processKeyPress(char keyPress);
std::tuple<int,int> getTerminalSize_windows();
void drawRows();
void refreshScreen();
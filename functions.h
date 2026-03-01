#include <string>

using namespace std;

enum Choice{};

#ifndef MATH_UTILS_H 
#define MATH_UTILS_H

void showSearchResults(string searchOutput);
string searchKnowledgeBase(string searchInput);
Choice getChoice();

#endif
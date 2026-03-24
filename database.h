#pragma once
#include <string>
#include <vector>

using namespace std;

bool insertIntoKeys(const tuple<string, string, string>& payload);
vector<tuple<string, string>> pullDomainAndCategories();
vector<tuple<string, string>> pullDomainAndCategoriesByCategory(string category);
string pullContentFromKeys(string domainKey);
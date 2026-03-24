#pragma once
#include <string>
#include <vector>

using namespace std;

bool insertIntoKeys(const tuple<string, string, string>& payload);
vector<tuple<string, string>> pullDomainAndCategories();
string pullContentFromKeys(string domainKey);
#include <vector>
#include <iostream>
#include <tuple>
#include <algorithm> 
#include <cctype>
#include <iterator>
#include "database.h"
#include "double_metaphone.h"
#include <sstream>

using namespace std;

struct Domain {
    string name;
    string category;
    string firstCode;
    string secondCode;
    string content;
};

// Helper Methods
tuple<string, string> getMetephoneCode(const string& text);

// Private Methods
vector<Domain> pullAllDomains();
vector<Domain> pullDomainsByCategory(string category);
vector<string> tokenize(const string& text);

tuple<string, string> getMetephoneCode(const string& text) {
    vector<string> codes;

    DoubleMetaphone(text, &codes);

    return make_tuple(codes[0], codes[1]);
}

vector<string> tokenize(const string& text) {
    vector<string> tokens;
    stringstream ss(text);
    string word;

    while (ss >> word) {
        tokens.push_back(word);
    }

    return tokens;
}


vector<Domain> pullAllDomains() { // To-Do: refactor domain struct and this method
    vector<tuple<string, string>> domainAndCategories = pullDomainAndCategories();

    vector<Domain> output;

    for (tuple<string, string> domainAndCategory : domainAndCategories) {
        Domain procressedDomain;

        procressedDomain.name = get<0>(domainAndCategory);

        tuple<string, string> metephoneCodesPair = getMetephoneCode(procressedDomain.name);
        
        procressedDomain.category = get<1>(domainAndCategory);
        procressedDomain.firstCode = get<0>(metephoneCodesPair);
        procressedDomain.secondCode= get<1>(metephoneCodesPair);

        output.push_back(procressedDomain);
    }

    return output;
    
}

vector<Domain> pullDomainsByCategory(string category) { // To-Do: refactor domain struct and this method
    vector<tuple<string, string>> domainAndCategories = pullDomainAndCategoriesByCategory(category);

    vector<Domain> output;

    for (tuple<string, string> domainAndCategory : domainAndCategories) {
        Domain procressedDomain;

        tuple<string, string> metephoneCodesPair = getMetephoneCode("test");
        
        procressedDomain.name = get<0>(domainAndCategory);
        procressedDomain.category = get<1>(domainAndCategory);
        procressedDomain.firstCode = get<0>(metephoneCodesPair);
        procressedDomain.secondCode= get<1>(metephoneCodesPair);

        output.push_back(procressedDomain);
    }

    return output;
    
}

vector<string> queryKnowledgeBase(const string& searchInput) {
    vector<Domain> allDomains = pullAllDomains();
    vector<string> searchWords = tokenize(searchInput);

    vector<tuple<string, int>> foundMatches;

    for (const Domain& domain : allDomains) {
        vector<string> domainWords = tokenize(domain.name);

        int matchCount = 0;

        vector<tuple<string, string>> domainCodes;
        for (const string& word : domainWords) {
            domainCodes.push_back(getMetephoneCode(word));
        }

        for (const string& searchWord : searchWords) {
            auto searchCodes = getMetephoneCode(searchWord);

            for (const auto& dCodes : domainCodes) {
                if (
                    get<0>(searchCodes) == get<0>(dCodes) ||
                    get<0>(searchCodes) == get<1>(dCodes) ||
                    get<1>(searchCodes) == get<0>(dCodes) ||
                    get<1>(searchCodes) == get<1>(dCodes)
                ) {
                    matchCount++;
                    break;
                }
            }
        }

        if (matchCount >= searchWords.size() / 2.0) {
            foundMatches.emplace_back(domain.name, matchCount);
        }
    }

    sort(foundMatches.begin(), foundMatches.end(),
        [](const auto& a, const auto& b) {
            return get<1>(a) > get<1>(b);
        });

    vector<string> output;
    output.reserve(foundMatches.size());

    for (const auto& [str, num] : foundMatches) {
        output.push_back(str);
    }

    return output;
}

vector<string> queryKnowledgeBaseByCategory(const string& searchInput) {
    string processedSearchInput = searchInput.substr(2); 

    vector<Domain> domainsByCategory = pullDomainsByCategory(processedSearchInput);

    

    vector<string> output;
    for (Domain domain : domainsByCategory) { // To-Do: fix the loop
        output.push_back(domain.name);
    }

    return output;
}

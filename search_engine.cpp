#include <vector>
#include <iostream>
#include <tuple>
#include <algorithm> 
#include <cctype>
#include <iterator>
#include "database.h"

using namespace std;

struct Domain {
    string name;
    string category;
    string firstCode;
    string secondCode;
    string content;
};

// Helper Methods
tuple<string, string> getMetephoneCode(string text);

// Private Methods
vector<Domain> pullAllDomains();
vector<Domain> pullDomainsByCategory(string category);

// To-Do: a lot of refactoring and abstracting

tuple<string, string> getMetephoneCode(string text) {
    // vector<char> vowels = {'A', 'E', 'I', 'O', 'U', 'W', 'Y'};




    // string firstCode[4];
    // string secondCode[4];

    // transform(text.begin(), text.end(), text.begin(),
    //         [](unsigned char c){ return tolower(c); });

    // for (int i = 0; i < text.length(); i++) {
    //     if ((i == 0) && (find(vowels.begin(), vowels.end(), toupper(text[i])) != vowels.end())) {
    //         firstCode[0] = 'A';
    //     }
    // }

    // string firstCodeStr = firstCode;

    tuple<string, string> output = make_tuple("1111", "1111");
    
    return output;

}

vector<Domain> pullAllDomains() { // To-Do: refactor domain struct and this method
    vector<tuple<string, string>> domainAndCategories = pullDomainAndCategories();

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

    tuple<string, string> searchMetephoneCodes = getMetephoneCode(searchInput);

    string searchFirstCode = get<0>(getMetephoneCode(searchInput));
    string searchSecondCode = get<1>(getMetephoneCode(searchInput));

    vector<string> output;
    for (Domain domain : allDomains) { // To-Do: fix the loop
        if ((domain.firstCode.size() > 0) && domain.firstCode == searchFirstCode) {
            output.push_back(domain.name);

        } else if ((domain.firstCode != "") && domain.firstCode == searchSecondCode) {
            output.push_back(domain.name);
        }

        else if ((domain.secondCode.size() > 0 && domain.secondCode.size() > 0) && domain.secondCode == searchSecondCode) {
            output.push_back(domain.name);
        }
    }
    
    return output;
}

vector<string> queryKnowledgeBaseByCategory(const string& searchInput) {
    string processedSearchInput = searchInput.substr(2); 

    vector<Domain> domainsByCategory = pullDomainsByCategory(processedSearchInput);

    tuple<string, string> searchMetephoneCodes = getMetephoneCode(processedSearchInput);

    string searchFirstCode = get<0>(getMetephoneCode(processedSearchInput));
    string searchSecondCode = get<1>(getMetephoneCode(processedSearchInput));

    vector<string> output;
    for (Domain domain : domainsByCategory) { // To-Do: fix the loop
        if ((domain.firstCode.size() > 0) && domain.firstCode == searchFirstCode) {
            output.push_back(domain.name);

        } else if ((domain.firstCode != "") && domain.firstCode == searchSecondCode) {
            output.push_back(domain.name);
        }

        else if ((domain.secondCode.size() > 0 && domain.secondCode.size() > 0) && domain.secondCode == searchSecondCode) {
            output.push_back(domain.name);
        }
    }
    
    return output;
}

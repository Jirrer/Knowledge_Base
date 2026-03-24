#include <vector>
#include <iostream>
#include <tuple>
#include <algorithm> 
#include <cctype>
#include <iterator>
// #include <sqlite3.h> // change to its own file later

using namespace std;

struct Domain {
    string name;
    string category;
    string firstCode;
    string secondCode;
};

vector<Domain> pullAllDomains();
tuple<string, string> getMetephoneCode(string text);

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

vector<Domain> pullAllDomains() {
    // dev data
    vector<string> searchResults = {
        "CoPpying working directory in windows",
        "trAversing a binary tree",
        "copying working directory in windows",
        "arranging in python"
    }; 

    vector<Domain> output;

    for (string domain : searchResults) {
        Domain procressedDomain;

        tuple<string, string> metephoneCodesPair = getMetephoneCode(domain);
        
        procressedDomain.name = domain;
        procressedDomain.firstCode = get<0>(metephoneCodesPair);
        procressedDomain.secondCode= get<1>(metephoneCodesPair);

        output.push_back(procressedDomain);
    }

    return output;
    
}

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
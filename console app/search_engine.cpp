#include <vector>
#include <iostream>
#include <tuple>
#include <algorithm> 
#include <cctype>
#include <iterator>
#include "database.h"
#include "double_metaphone.h"
#include <sstream>

struct Domain {
    std::string name;
    std::string category;
    std::string firstCode;
    std::string secondCode;
    std::string content;
};

std::tuple<std::string, std::string> getMetephoneCode(const std::string& text);
std::vector<std::string> tokenize(const std::string& text);
std::vector<Domain> getAllDomains();
std::vector<Domain> getDomainsByCategory(std::string category);

std::tuple<std::string, std::string> getMetephoneCode(const std::string& text) {
    std::vector<std::string> codes;

    DoubleMetaphone(text, &codes);

    return std::make_tuple(codes[0], codes[1]);
}

std::vector<std::string> tokenize(const std::string& text) {
    std::vector<std::string> tokens;
    std::stringstream ss(text);
    std::string word;

    while (ss >> word) {
        tokens.push_back(word);
    }

    return tokens;
}

std::vector<Domain> getAllDomains() {
    std::vector<std::tuple<std::string, std::string>> domainAndCategories = pullDomainAndCategories();

    std::vector<Domain> output;

    for (std::tuple<std::string, std::string> domainAndCategory : domainAndCategories) {
        Domain procressedDomain;

        procressedDomain.name = std::get<0>(domainAndCategory);

        std::tuple<std::string, std::string> metephoneCodesPair = getMetephoneCode(procressedDomain.name);
        
        procressedDomain.category = std::get<1>(domainAndCategory);
        procressedDomain.firstCode = std::get<0>(metephoneCodesPair);
        procressedDomain.secondCode= std::get<1>(metephoneCodesPair);

        output.push_back(procressedDomain);
    }

    return output;
    
}

std::vector<Domain> getDomainsByCategory(std::string category) { // To-Do: refactor domain struct and this method
    std::vector<std::tuple<std::string, std::string>> domainAndCategories = pullDomainAndCategoriesByCategory(category);

    std::vector<Domain> output;

    for (std::tuple<std::string, std::string> domainAndCategory : domainAndCategories) {
        Domain procressedDomain;

        std::tuple<std::string, std::string> metephoneCodesPair = getMetephoneCode("test");
        
        procressedDomain.name = std::get<0>(domainAndCategory);
        procressedDomain.category = std::get<1>(domainAndCategory);
        procressedDomain.firstCode = std::get<0>(metephoneCodesPair);
        procressedDomain.secondCode= std::get<1>(metephoneCodesPair);

        output.push_back(procressedDomain);
    }

    return output;
    
}

std::vector<std::string> queryKnowledgeBase(const std::string& searchInput) {
    std::vector<Domain> allDomains = getAllDomains();
    std::vector<std::string> searchWords = tokenize(searchInput);

    std::vector<std::tuple<std::string, int>> foundMatches;

    for (const Domain& domain : allDomains) {
        std::vector<std::string> domainWords = tokenize(domain.name);

        int matchCount = 0;

        std::vector<std::tuple<std::string, std::string>> domainCodes;
        for (const std::string& word : domainWords) {
            domainCodes.push_back(getMetephoneCode(word));
        }

        for (const std::string& searchWord : searchWords) {
            auto searchCodes = getMetephoneCode(searchWord);

            for (const auto& dCodes : domainCodes) {
                if (
                    std::get<0>(searchCodes) == std::get<0>(dCodes) ||
                    std::get<0>(searchCodes) == std::get<1>(dCodes) ||
                    std::get<1>(searchCodes) == std::get<0>(dCodes) ||
                    std::get<1>(searchCodes) == std::get<1>(dCodes)
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

    std::sort(foundMatches.begin(), foundMatches.end(),
        [](const auto& a, const auto& b) {
            return std::get<1>(a) > std::get<1>(b);
        });

    std::vector<std::string> output;
    output.reserve(foundMatches.size());

    for (const auto& [str, num] : foundMatches) {
        output.push_back(str);
    }

    return output;
}

std::vector<std::string> queryKnowledgeBaseByCategory(const std::string& searchInput) {
    std::string processedSearchInput = searchInput.substr(2); 

    std::vector<Domain> domainsByCategory = getDomainsByCategory(processedSearchInput);


    std::vector<std::string> output;
    for (Domain domain : domainsByCategory) { // To-Do: fix the loop
        output.push_back(domain.name);
    }

    return output;
}

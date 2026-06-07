#pragma once

#include <string>
#include <vector>
#include <optional>

struct Domain {
    std::string name;
    std::string category;
    std::string firstCode;
    std::string secondCode;
    std::string content;
};

enum class DomainQueryType {
    ALL,
    CATEGORY,
};

std::vector<Domain> pullDomains(DomainQueryType domainQueryType, std::optional<std::string> categoryInput = std::nullopt);
bool removeFromDatabase(std::string title, std::string category);
std::string pullContentFromKeys(std::string domainKey);
bool insertIntoKeys(const std::tuple<std::string, std::string, std::string>& payload);
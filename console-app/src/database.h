#pragma once

bool insertIntoKeys(const std::tuple<std::string, std::string, std::string>& payload);
std::vector<std::tuple<std::string, std::string>> pullDomainAndCategories();
std::vector<std::tuple<std::string, std::string>> pullDomainAndCategoriesByCategory(std::string category);
std::string pullContentFromKeys(std::string domainKey);
bool removeFromDatabase(std::string title, std::string category);
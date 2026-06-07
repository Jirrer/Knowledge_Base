#pragma once

enum class Query_Type {
    DEFAULT,
    CATEGORY,
};

std::vector<std::string> queryKnowledgeBase(const std::string& searchInput, Query_Type queryType);
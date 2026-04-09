#include <iostream>
#include "sqlite3.h"
#include <vector>
#include <tuple>
#include <string>
#include <cstdlib>
#include <fstream>

const char* getDbPath() {
    const char* p = std::getenv("SQLITE_DB_PATH");
    return (p && *p) ? p : nullptr;
}

bool openDatabase(sqlite3** db) { // To-Do: add loop that waits for user to acknowledge the error
    const char* dbPath = getDbPath();

    if (!dbPath) {
        std::cerr << "Missing SQLITE_DB_PATH. Set it in your .env file.\n";
        return false;
    }

    std::ifstream dbFile(dbPath);
    if (!dbFile.good()) {
        std::cerr << "Database file not found: " << dbPath << "\n";
        return false;
    }
    dbFile.close();

    int rc = sqlite3_open(dbPath, db);
    if (rc != SQLITE_OK) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(*db) << std::endl;
        sqlite3_close(*db);
        *db = nullptr;
        return false;
    }

    return true;
}

bool insertIntoKeys(const std::tuple<std::string, std::string, std::string>& payload) {
    sqlite3* db;

    if (!openDatabase(&db)) {
        return false;
    }

    int rc;
    
    sqlite3_stmt* stmt;

    const char* sql = 
        "INSERT INTO keys (domain, category, content) VALUES (?, ?, ?);";

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            std::cerr << "Prepare failed: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_close(db);
            return false;
        }

    sqlite3_bind_text(stmt, 1, std::get<0>(payload).c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, std::get<1>(payload).c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, std::get<2>(payload).c_str(), -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Insert into keys failed: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return rc == SQLITE_DONE;
}

std::vector<std::tuple<std::string, std::string>> pullDomainAndCategories() {
    sqlite3* db;
    if (!openDatabase(&db)) {
        return {};
    }

    const char* sql = "SELECT domain, category, content FROM keys;";

    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    std::vector<std::tuple<std::string, std::string>> output;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string domain = sqlite3_column_text(stmt, 0)
        ? (const char*)sqlite3_column_text(stmt, 0)
        : "";

        std::string category = sqlite3_column_text(stmt, 1)
        ? (const char*)sqlite3_column_text(stmt, 1)
        : "";

        output.push_back({domain, category});
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return output;
}

std::vector<std::tuple<std::string, std::string>> pullDomainAndCategoriesByCategory(std::string categoryInput) {
    sqlite3* db;
    sqlite3_stmt* stmt;

    std::vector<std::tuple<std::string, std::string>> output;

    if (!openDatabase(&db)) {
        return output;
    }

    const char* sql = 
        "SELECT domain, category FROM keys WHERE category = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        sqlite3_close(db);
        return output;
    }

    sqlite3_bind_text(stmt, 1, categoryInput.c_str(), -1, SQLITE_TRANSIENT);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string domain = sqlite3_column_text(stmt, 0)
            ? (const char*)sqlite3_column_text(stmt, 0)
            : "";

        std::string category = sqlite3_column_text(stmt, 1)
            ? (const char*)sqlite3_column_text(stmt, 1)
            : "";

        output.emplace_back(domain, category);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return output;
}

std::string pullContentFromKeys(std::string domainKey) {
    sqlite3* db;
    sqlite3_stmt* stmt;
    std::string result = "";

    if (!openDatabase(&db)) {
        return "";
    }

    const char* sql = "SELECT content FROM keys WHERE domain = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Prepare failed\n";
        sqlite3_close(db);
        return "";
    }

    sqlite3_bind_text(stmt, 1, domainKey.c_str(), -1, SQLITE_TRANSIENT);

    int rc = sqlite3_step(stmt);

    if (rc == SQLITE_ROW) {
        const unsigned char* text = sqlite3_column_text(stmt, 0);

        if (text) {
            result = (const char*)text;
        } else {
            result = "";  
        }
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return result;
}

#include <iostream>
#include "sqlite3.h"
#include <vector>
#include <tuple>
#include <string>

using namespace std;

bool insertIntoKeys(const tuple<string, string, string>& payload) {
    sqlite3* db;
    
    int rc = sqlite3_open("Knowledge_Base.db", &db);
    
    if (rc) {
            cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
            return false;
        } 
    
    sqlite3_stmt* stmt;

    const char* sql = 
        "INSERT INTO keys (domain, category, content) VALUES (?, ?, ?);";

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            cerr << "Prepare failed: " << sqlite3_errmsg(db) << endl;
            sqlite3_close(db);
            return false;
        }

    sqlite3_bind_text(stmt, 1, get<0>(payload).c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, get<1>(payload).c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, get<2>(payload).c_str(), -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        cerr << "Insert into keys failed: " << sqlite3_errmsg(db) << endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return rc == SQLITE_DONE;
}

vector<tuple<string, string>> pullDomainAndCategories() {
    sqlite3* db;
    sqlite3_open("Knowledge_Base.db", &db);

    const char* sql = "SELECT domain, category, content FROM keys;";

    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    vector<tuple<string, string>> output;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        string domain = sqlite3_column_text(stmt, 0)
        ? (const char*)sqlite3_column_text(stmt, 0)
        : "";

        string category = sqlite3_column_text(stmt, 1)
        ? (const char*)sqlite3_column_text(stmt, 1)
        : "";

        output.push_back({domain, category});
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return output;
}

vector<tuple<string, string>> pullDomainAndCategoriesByCategory(string categoryInput) {
    sqlite3* db;
    sqlite3_stmt* stmt;

    vector<tuple<string, string>> output;

    if (sqlite3_open("Knowledge_Base.db", &db)) {
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
        string domain = sqlite3_column_text(stmt, 0)
            ? (const char*)sqlite3_column_text(stmt, 0)
            : "";

        string category = sqlite3_column_text(stmt, 1)
            ? (const char*)sqlite3_column_text(stmt, 1)
            : "";

        output.emplace_back(domain, category);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return output;
}

string pullContentFromKeys(string domainKey) {
    sqlite3* db;
    sqlite3_stmt* stmt;
    string result = "";

    if (sqlite3_open("Knowledge_Base.db", &db)) {
        cerr << "Can't open database\n";
        return "";
    }

    const char* sql = "SELECT content FROM keys WHERE domain = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Prepare failed\n";
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

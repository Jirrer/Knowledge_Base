#include <iostream>
#include "sqlite3.h"

bool insertIntoKeys(const std::tuple<std::string, std::string, std::string>& payload) {
    sqlite3* db;
    
    int rc = sqlite3_open("Knowledge_Base.db", &db);
    
    if (rc) {
            std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
            return false;
        } 
    
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

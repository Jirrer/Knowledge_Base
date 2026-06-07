#include "pullingData.h"
#include "config.h"
#include "sqlite3.h"
#include <iostream>
#include <fstream>
#include <cstdlib>

std::vector<Domain> queryDatabase(std::string query);
std::vector<Domain> readLocalData();
void verifyLocalData(std::vector<Domain>* localDataPointer);
void updateLocalData(std::vector<Domain>* localDataPointer);
bool openDatabase(sqlite3** db);

std::vector<Domain> readLocalData() {


    std::vector<Domain> temp;

    return temp;
}

void verifyLocalData(std::vector<Domain>* localDataPointer) {
    


}

void updateLocalData(std::vector<Domain>* localDataPointer) {

}



std::vector<Domain> pullDomains(DomainQueryType domainQueryType, std::optional<std::string> categoryInput) {
    std::vector<Domain> localData = readLocalData();

    if (isOnline) {
        verifyLocalData(&localData); 
    }

    std::string query;
    
    
    switch (domainQueryType) {
        case DomainQueryType::ALL:
            query = "SELECT domain, category, content FROM keys;";

            break;

        case DomainQueryType::CATEGORY:
            query = "SELECT domain, category FROM keys WHERE category = " + categoryInput.value() + ";";

            break;
    }


    return queryDatabase(query);
}

std::vector<Domain> queryDatabase(std::string query) {
    sqlite3* db;

    if (!openDatabase(&db)) {
        return {};
    }

    const char* sql = "SELECT domain, category, content FROM keys;";

    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    std::vector<Domain> output;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Domain d;

        d.name = sqlite3_column_text(stmt, 0)
            ? (const char*)sqlite3_column_text(stmt, 0)
            : "";

        d.category = sqlite3_column_text(stmt, 1)
            ? (const char*)sqlite3_column_text(stmt, 1)
            : "";

        d.content = sqlite3_column_text(stmt, 2)
            ? (const char*)sqlite3_column_text(stmt, 2)
            : "";

        output.push_back(d);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return output;
}


bool openDatabase(sqlite3** db) {
    const char* localPath = "internal_storage.db";

    std::ifstream dbFile(localPath);
    if (!dbFile.good()) {
        std::cerr << "Database file not found: " << localPath << "\n";
        return false;
    }
    dbFile.close();

    int rc = sqlite3_open(localPath, db);
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

bool removeFromDatabase(std::string title, std::string category) {

    return true;
}
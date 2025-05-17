#include "DatabaseManager.h"
#include <iostream>
#include <sstream>

DatabaseManager::DatabaseManager() : db(nullptr) {}

DatabaseManager::~DatabaseManager() {
    if (db) {
        sqlite3_close(db);
    }
}

bool DatabaseManager::initialize(const std::string& dbPath) {
    int rc = sqlite3_open(dbPath.c_str(), &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    return createTablesIfNotExist();
}

bool DatabaseManager::createTablesIfNotExist() {
    const char* createTableSQL = R"(
        CREATE TABLE IF NOT EXISTS files (
            imageid INTEGER PRIMARY KEY AUTOINCREMENT,
            imagename TEXT NOT NULL,
            imagepath TEXT NOT NULL,
            imagesize BIGINT,
            image_file_type TEXT,
            image_creation_time DATETIME,
            image_modified_time DATETIME,
            image_entry_time DATETIME DEFAULT CURRENT_TIMESTAMP
        )
    )";

    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, createTableSQL, nullptr, nullptr, &errMsg);
    
    if (rc != SQLITE_OK) {
        std::cerr << "Can't create table: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

bool DatabaseManager::deleteImage(int imageId) {
    std::string sql = "DELETE FROM files WHERE imageid = " + std::to_string(imageId);
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL delete error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

bool DatabaseManager::deleteImages(const std::vector<int>& imageIds) {
    if (imageIds.empty()) {
        return true; // Nothing to delete
    }

    std::stringstream sql;
    sql << "DELETE FROM files WHERE imageid IN (";
    for (size_t i = 0; i < imageIds.size(); ++i) {
        sql << imageIds[i];
        if (i < imageIds.size() - 1) {
            sql << ",";
        }
    }
    sql << ")";

    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.str().c_str(), nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL delete error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

std::vector<ImageRecord> DatabaseManager::getImages(const std::string& whereClause) {
    std::vector<ImageRecord> images;
    std::string sql = "SELECT imageid, imagename, imagepath, imagesize, image_file_type, image_creation_time, image_modified_time, image_entry_time FROM files";
    if (!whereClause.empty()) {
        sql += " WHERE " + whereClause;
    }

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << std::endl;
        return images; // Return empty vector on error
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        ImageRecord record;
        record.id = sqlite3_column_int(stmt, 0);
        record.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        record.path = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        record.size = sqlite3_column_int64(stmt, 3); // Use int64 for large sizes
        record.fileType = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        record.creationTime = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
        record.modifiedTime = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
        record.entryTime = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7));
        images.push_back(record);
    }

    sqlite3_finalize(stmt);
    return images;
}

bool DatabaseManager::insertImageMetadata(const std::string& filename,
                                        const std::string& fullPath,
                                        std::uintmax_t fileSize,
                                        const std::string& fileType,
                                        const std::string& creationTime,
                                        const std::string& modificationTime) {

     // Check if the image path already exists in the database
    const char* checkExistenceSQL = "SELECT COUNT(*) FROM files WHERE imagepath = ?";
    sqlite3_stmt* checkStmt;
    int rc_check = sqlite3_prepare_v2(db, checkExistenceSQL, -1, &checkStmt, nullptr); 
    if (rc_check != SQLITE_OK) { 
        std::cerr << "Prepare statement for existence check failed: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_text(checkStmt, 1, fullPath.c_str(), -1, SQLITE_TRANSIENT);
    rc_check = sqlite3_step(checkStmt);
    if (rc_check == SQLITE_ROW) {
        int count = sqlite3_column_int(checkStmt, 0);
        if (count > 0) {
            sqlite3_finalize(checkStmt);
            std::cerr << "Image with path '" << fullPath << "' already exists in the database." << std::endl;
            return false; // Image already exists
        }
    }
    sqlite3_finalize(checkStmt);

    std::string insertSQL = "INSERT INTO files (imagename, imagepath, imagesize, image_file_type, "
                           "image_creation_time, image_modified_time) VALUES ('" +
                           filename + "', '" + fullPath + "', " + std::to_string(fileSize) + ", '" +
                           fileType + "', '" + creationTime + "', '" + modificationTime + "')";

    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, insertSQL.c_str(), nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL insert error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

int DatabaseManager::recordCallback(void* NotUsed, int argc, char** argv, char** azColName) {
    for (int i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

bool DatabaseManager::executeQuery(const std::string& query, int (*callback)(void*, int, char**, char**), void* data) {
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, query.c_str(), callback, data, &errMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL query error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}
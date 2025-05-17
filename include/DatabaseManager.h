#pragma once
#include <sqlite3.h>
#include <string>
#include <filesystem>
#include <vector>

struct ImageRecord {
    int id;
    std::string name;
    std::string path;
    std::uintmax_t size;
    std::string fileType;
    std::string creationTime;
    std::string modifiedTime;
    std::string entryTime;
};

class DatabaseManager {
public:
    DatabaseManager();
    ~DatabaseManager();
    
    // Core database operations
    bool initialize(const std::string& dbPath);
    bool executeQuery(const std::string& query, int (*callback)(void*, int, char**, char**) = nullptr, void* data = nullptr);
    
    // Basic CRUD operations
    bool insertImageMetadata(const std::string& filename,
        const std::string& fullPath,
        std::uintmax_t fileSize,
        const std::string& fileType,
        const std::string& creationTime,
        const std::string& modificationTime);    
    bool deleteImage(int imageId);
    bool deleteImages(const std::vector<int>& imageIds);
    std::vector<ImageRecord> getImages(const std::string& whereClause = "");

private:
    sqlite3* db;
    bool createTablesIfNotExist();
    static int recordCallback(void* data, int argc, char** argv, char** azColName);
};
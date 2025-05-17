#pragma once

#include "DatabaseManager.h"
#include <string>
#include <vector>
#include <optional>

// Data structures for search and results
struct SearchCriteria {
    std::optional<std::string> name;
    std::optional<std::string> fileType;
    std::optional<std::string> dateFrom;
    std::optional<std::string> dateTo;
    std::optional<size_t> minSize;
    std::optional<size_t> maxSize;
};

struct ImageResult {
    std::string path;
    std::string name;
    std::string fileType;
    size_t size;
    std::string creationTime;
    std::string modifiedTime;
};

class DatabaseFunctions {
public:
    explicit DatabaseFunctions(DatabaseManager& dbManager);

    // UI Handlers
    void handleAddImages();
    void handleSearch();
    void handleRemoveImages();
    void handleListImages();
    void displayResults(const std::vector<ImageResult>& results);

    // Image Addition Operations
    bool addSingleImage(const std::string& imagePath);
    bool addImagesFromDirectory(const std::string& dirPath, bool recursive);
    
    // Search Operations
    std::vector<ImageRecord> searchImages(const SearchCriteria& criteria);
    std::vector<ImageRecord> getAllImages();
    
    // Remove Operations
    bool removeSingleImage(const std::string& path);
    bool removeImagesByCriteria(const SearchCriteria& criteria);
    bool removeAllImages();
    bool removeImageById(int id);
    bool removeImagesByIds(const std::vector<int>& ids);
    bool removeImageByPath(const std::string& path);

    // List Operations
    std::vector<ImageRecord> listAllImages();
    std::vector<ImageRecord> listImagesByType(const std::string& fileType);
    std::vector<ImageRecord> listImagesByDateRange(const std::string& fromDate, 
                                                 const std::string& toDate);

private:
    DatabaseManager& dbManager;
    std::string buildSearchQuery(const SearchCriteria& criteria);
    bool validateImagePath(const std::string& path);
};
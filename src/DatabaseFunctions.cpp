// src/DatabaseFunctions.cpp
#include "DatabaseFunctions.h"
#include "ImageProcessor.h"
#include "TimeUtils.h"
#include <filesystem>
#include <iostream>
#include <sstream>

DatabaseFunctions::DatabaseFunctions(DatabaseManager& dbManager) : dbManager(dbManager) {}

void DatabaseFunctions::handleAddImages() {
    std::cout << "\nAdd Images:\n";
    std::cout << "1. Add single image\n";
    std::cout << "2. Add from directory\n";
    std::cout << "Enter choice: ";
    
    int choice;
    std::cin >> choice;
    std::cin.ignore();

    switch(choice) {
        case 1: {
            std::string path;
            std::cout << "Enter image path: ";
            std::getline(std::cin, path);
            if(addSingleImage(path)) {
                std::cout << "Image added successfully\n";
            }
            break;
        }
        case 2: {
            std::cout << "Recursive (y/n)? ";
            char recursiveChar;
            std::cin >> recursiveChar;
            std::cin.ignore();
            bool recursive = (recursiveChar == 'y' || recursiveChar == 'Y');

            std::string path;
            std::cout << "Enter directory path: ";
            std::getline(std::cin, path);

            if(addImagesFromDirectory(path, recursive)) {
                std::cout << "Directory processed successfully\n";
            }
            break;
        }
        default:
            std::cout << "Invalid choice\n";
            break;
    }
}

void DatabaseFunctions::handleSearch() {
    SearchCriteria criteria;
    std::string input;

    std::cout << "Enter search criteria (press Enter to skip):\n";
    
    std::cout << "Filename contains: ";
    std::getline(std::cin, input);
    if (!input.empty()) criteria.name = input;

    std::cout << "File type: ";
    std::getline(std::cin, input);
    if (!input.empty()) criteria.fileType = input;

    auto records = searchImages(criteria);
    std::vector<ImageResult> results;
    
    for (const auto& record : records) {
        results.push_back(ImageResult{
            record.path,
            record.name,
            record.fileType,
            record.size,
            record.creationTime,
            record.modifiedTime
        });
    }
    
    displayResults(results);
}

void DatabaseFunctions::handleRemoveImages() {
    std::cout << "\nRemove Images:\n";
    std::cout << "1. Remove single image\n";
    std::cout << "2. Remove by criteria\n";
    std::cout << "3. Remove all images\n";
    std::cout << "Enter choice: ";
    
    int choice;
    std::cin >> choice;
    std::cin.ignore();

    switch(choice) {
        case 1: {
            std::string path;
            std::cout << "Enter image path to remove: ";
            std::getline(std::cin, path);
            if(removeSingleImage(path)) {
                std::cout << "Image removed successfully\n";
            } else {
                std::cout << "Failed to remove image\n";
            }
            break;
        }
        case 2: {
            SearchCriteria criteria;
            std::string input;
            
            std::cout << "Enter criteria for removal (press Enter to skip):\n";
            std::cout << "Filename contains: ";
            std::getline(std::cin, input);
            if (!input.empty()) criteria.name = input;
            
            std::cout << "File type: ";
            std::getline(std::cin, input);
            if (!input.empty()) criteria.fileType = input;
            
            if(removeImagesByCriteria(criteria)) {
                std::cout << "Images removed successfully\n";
            } else {
                std::cout << "Failed to remove images or no matching images found\n";
            }
            break;
        }
        case 3: {
            if(removeAllImages()){
                std::cout << "All images removed successfully\n";
            } else{
                std::cout << "Failed to remove all images\n";
            }
            break;
        }
        default:
            std::cout << "Invalid choice\n";
            break;
    }
}

void DatabaseFunctions::handleListImages() {
    auto records = getAllImages();
    std::vector<ImageResult> results;
    
    for (const auto& record : records) {
        results.push_back(ImageResult{
            record.path,
            record.name,
            record.fileType,
            record.size,
            record.creationTime,
            record.modifiedTime
        });
    }
    
    displayResults(results);
}

void DatabaseFunctions::displayResults(const std::vector<ImageResult>& results) {
    if (results.empty()) {
        std::cout << "No results found.\n";
        return;
    }

    std::cout << "\nFound " << results.size() << " images:\n";
    std::cout << "-----------------\n";
    for (const auto& result : results) {
        std::cout << "Name: " << result.name << "\n";
        std::cout << "Path: " << result.path << "\n";
        std::cout << "Type: " << result.fileType << "\n";
        std::cout << "Size: " << result.size << " bytes\n";
        std::cout << "Created: " << result.creationTime << "\n";
        std::cout << "Modified: " << result.modifiedTime << "\n";
        std::cout << "-----------------\n";
    }
}

bool DatabaseFunctions::addSingleImage(const std::string& imagePath) {
    if (!validateImagePath(imagePath)) {
        std::cerr << "Invalid image path or file: " << imagePath << std::endl;
        return false;
    }

    std::filesystem::path path(imagePath);
    ImageRecord record;
    record.name = path.filename().string();
    record.path = imagePath;
    record.size = std::filesystem::file_size(path);
    record.fileType = path.extension().string().substr(1); // Remove the dot from extension
    record.creationTime = ImageProcessor::getCreationTimeFromExif(imagePath);
    record.modifiedTime = TimeUtils::fileTimeToSQLiteString(
        std::filesystem::last_write_time(path));

    return dbManager.insertImageMetadata(
        record.name,
        record.path,
        record.size,
        record.fileType,
        record.creationTime,
        record.modifiedTime);
}

bool DatabaseFunctions::addImagesFromDirectory(const std::string& dirPath, bool recursive) {
    if (!std::filesystem::exists(dirPath)) {
        std::cerr << "Directory does not exist: " << dirPath << std::endl;
        return false;
    }

    bool success = true;

    if (recursive) {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(dirPath)) {
            if (entry.is_regular_file()) {
                std::string filePath = entry.path().string();
                if (ImageProcessor::isImageFile(filePath)) {
                    if (!addSingleImage(filePath)) {
                        success = false;
                        std::cerr << "Failed to add image: " << filePath << std::endl;
                    }
                }
            }
        }
    } else {
        for (const auto& entry : std::filesystem::directory_iterator(dirPath)) {
            if (entry.is_regular_file()) {
                std::string filePath = entry.path().string();
                if (ImageProcessor::isImageFile(filePath)) {
                    if (!addSingleImage(filePath)) {
                        success = false;
                        std::cerr << "Failed to add image: " << filePath << std::endl;
                    }
                }
            }
        }
    }

    return success;
}

std::vector<ImageRecord> DatabaseFunctions::searchImages(const SearchCriteria& criteria) {
    std::string query = buildSearchQuery(criteria);
    return dbManager.getImages(query);
}

bool DatabaseFunctions::removeSingleImage(const std::string& path) {
    return removeImageByPath(path);
}

bool DatabaseFunctions::removeImagesByCriteria(const SearchCriteria& criteria) {
    std::string query = buildSearchQuery(criteria);
    auto images = dbManager.getImages(query);
    std::vector<int> ids;
    for (const auto& image : images) {
        ids.push_back(image.id);
    }
    return removeImagesByIds(ids);
}

bool DatabaseFunctions::removeAllImages()
{
    auto images = dbManager.getImages();
    std::vector<int> ids;
    for(const auto& image : images){
        ids.push_back(image.id);
    }
    return removeImagesByIds(ids);
}

std::vector<ImageRecord> DatabaseFunctions::getAllImages() {
    return listAllImages();
}

std::string DatabaseFunctions::buildSearchQuery(const SearchCriteria& criteria) {
    std::vector<std::string> conditions;

    // Helper function to escape SQL strings
    auto escapeString = [](const std::string& input) -> std::string {
        std::string escaped;
        for (char c : input) {
            if (c == '\'' || c == '"' || c == '\\') {
                escaped += '\\';
            }
            escaped += c;
        }
        return escaped;
    };

    // Add conditions with proper escaping
    if (criteria.name) {
        std::string escapedName = escapeString(*criteria.name);
        conditions.push_back("imagename LIKE '%" + escapedName + "%'");
    }
    if (criteria.fileType) {
        std::string escapedFileType = escapeString(*criteria.fileType);
        conditions.push_back("image_file_type = '" + escapedFileType + "'");
    }
    if (criteria.dateFrom) {
        std::string escapedDateFrom = escapeString(*criteria.dateFrom);
        conditions.push_back("image_creation_time >= '" + escapedDateFrom + "'");
    }
    if (criteria.dateTo) {
        std::string escapedDateTo = escapeString(*criteria.dateTo);
        conditions.push_back("image_creation_time <= '" + escapedDateTo + "'");
    }
    if (criteria.minSize) {
        // No need to escape numeric values
        conditions.push_back("imagesize >= " + std::to_string(*criteria.minSize));
    }
    if (criteria.maxSize) {
        // No need to escape numeric values
        conditions.push_back("imagesize <= " + std::to_string(*criteria.maxSize));
    }

    // If no conditions, return empty string
    if (conditions.empty()) {
        return "";
    }

    // Build WHERE clause
    std::stringstream whereClause;
    whereClause << "WHERE ";
    for (size_t i = 0; i < conditions.size(); ++i) {
        if (i > 0) {
            whereClause << " AND ";
        }
        whereClause << conditions[i];
    }

    return whereClause.str();
}

bool DatabaseFunctions::removeImageById(int id) {
    return dbManager.deleteImage(id);
}

bool DatabaseFunctions::removeImagesByIds(const std::vector<int>& ids) {
    return dbManager.deleteImages(ids);
}

bool DatabaseFunctions::removeImageByPath(const std::string& path) {
    // First, find the image by path
    std::string whereClause = "imagepath = '" + path + "'";
    auto images = dbManager.getImages(whereClause);
    
    if (images.empty()) {
        std::cerr << "No image found with path: " << path << std::endl;
        return false;
    }
    
    // Delete the image using its ID
    return dbManager.deleteImage(images[0].id);
}

std::vector<ImageRecord> DatabaseFunctions::listAllImages() {
    return dbManager.getImages();
}

std::vector<ImageRecord> DatabaseFunctions::listImagesByType(const std::string& fileType) {
    std::string whereClause = "WHERE image_file_type = '" + fileType + "'";
    return dbManager.getImages(whereClause);
}

std::vector<ImageRecord> DatabaseFunctions::listImagesByDateRange(
    const std::string& fromDate, const std::string& toDate) {
    std::string whereClause = "WHERE image_creation_time BETWEEN '" + 
                             fromDate + "' AND '" + toDate + "'";
    return dbManager.getImages(whereClause);
}

bool DatabaseFunctions::validateImagePath(const std::string& path) {
    if (!std::filesystem::exists(path)) {
        return false;
    }
    if (!std::filesystem::is_regular_file(path)) {
        return false;
    }
    return ImageProcessor::isImageFile(path);
}
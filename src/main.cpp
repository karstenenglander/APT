#include "DatabaseManager.h"
#include "ImageProcessor.h"
#include "TimeUtils.h"
#include "DatabaseFunctions.h"
#include <iostream>
#include <filesystem>
#include <string>
#include <limits>

void displayMenu() {
    std::cout << "\nImage Catalog Menu:\n";
    std::cout << "1. Add image(s)\n";
    std::cout << "2. Search images\n";
    std::cout << "3. Remove image(s)\n";
    std::cout << "4. List images\n";
    std::cout << "5. Exit\n";
    std::cout << "Enter your choice: ";
}

int main() {
    DatabaseManager dbManager;
    if (!dbManager.initialize("metadata.db")) {
        return 1;
    }

    DatabaseFunctions dbFunctions(dbManager);

    while (true) {
        displayMenu();
        
        int choice;
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            case 1:
                dbFunctions.handleAddImages();
                break;
            case 2:
                dbFunctions.handleSearch();
                break;
            case 3:
                dbFunctions.handleRemoveImages();
                break;
            case 4:
                dbFunctions.handleListImages();
                break;
            case 5:
                std::cout << "Exiting program...\n";
                return 0;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}
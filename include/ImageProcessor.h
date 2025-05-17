#pragma once
#include <string>
#include <filesystem>
#include <exiv2/exiv2.hpp>
#include <iostream>

class ImageProcessor {
public:
    static bool isImageFile(const std::string& filename);
    static std::string getCreationTimeFromExif(const std::string& filepath);
};
#include "ImageProcessor.h"

bool ImageProcessor::isImageFile(const std::string& filename) {
    try {
        auto image = Exiv2::ImageFactory::open(filename);
        image->readMetadata();
        std::string extension = filename.substr(filename.find_last_of(".") + 1);
        if(extension == "xmp"){
            std::cout << "Skipping .xmp file: " << filename << "\n";
            return false;
        } else{
            return true;
        }
    } catch (const Exiv2::Error& e) {
        return false;
    }
}

std::string ImageProcessor::getCreationTimeFromExif(const std::string& filepath) {
    try {
        auto image = Exiv2::ImageFactory::open(filepath);
        image->readMetadata();
        auto exifData = image->exifData();
        if (exifData.findKey(Exiv2::ExifKey("Exif.Photo.DateTimeOriginal")) != exifData.end()) {
            return exifData["Exif.Photo.DateTimeOriginal"].toString();
        }
    } catch (...) {}
    return "N/A";
}
cmake_minimum_required(VERSION 3.10)

project(APT CXX)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Release)


set(CMAKE_TOOLCHAIN_FILE "C:/Users/karst/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
set(CMAKE_PREFIX_PATH "C:/Users/karst/vcpkg/installed/x64-windows/share" ${CMAKE_PREFIX_PATH})



find_package(TIFF REQUIRED)
find_package(wxWidgets CONFIG REQUIRED)
find_package(exiv2 CONFIG REQUIRED)
find_package(OpenCV REQUIRED COMPONENTS core imgproc imgcodecs highgui)
find_package(libraw CONFIG REQUIRED)
find_package(unofficial-sqlite3 CONFIG REQUIRED)
find_package(protobuf CONFIG REQUIRED)

set(SOURCES
    src/main.cpp
    src/DatabaseManager.cpp
    src/ImageProcessor.cpp
    src/TimeUtils.cpp
)

include_directories(${PROJECT_SOURCE_DIR}/include ${OpenCV_INCLUDE_DIRS} ${LIBRAW_INCLUDE_DIRS})
link_directories(${OpenCV_LIBRARY_DIRS})

# Add the executable
add_executable(APT 
    src/main.cpp 
    src/DatabaseManager.cpp 
    src/ImageProcessor.cpp 
    src/TimeUtils.cpp
    src/DatabaseFunctions.cpp
)

# Add include directories
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${OpenCV_INCLUDE_DIRS}
    ${LibRaw_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE 
    ${OpenCV_LIBS}
    libraw::raw_r  # Changed from raw::raw to LibRaw::LibRaw
    TIFF::TIFF
    protobuf::libprotobuf
    unofficial::sqlite3::sqlite3
    Exiv2::exiv2lib
    wx::core wx::base
)

# Optional: Set output directories for executables
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
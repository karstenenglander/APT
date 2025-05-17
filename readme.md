# Advanced Photo Tagger (APT)

A C++ application designed for organizing photographs by cataloging their metadata. APT extracts information from image files and stores it in an SQLite database, allowing users to add, search, and manage their photo collections through a command-line interface. The ultimate goal is to support comprehensive tagging for fine-grained categorization.

## Current Features

*   **Metadata Extraction & Storage**:
    *   Extracts metadata including filename, full path, file size, file type, EXIF creation time, and filesystem modification time.
    *   Stores extracted metadata in an SQLite database.
    *   Automatically records the entry time for each image.
*   **Image Management**:
    *   Add individual image files.
    *   Add all images from a specified directory, with an option for recursive scanning.
    *   Prevents duplicate entries based on unique image paths.
    *   Filters out non-image files during scanning (using Exiv2 to validate image integrity).
*   **Database Operations**:
    *   Remove images from the database:
        *   Individually by path.
        *   By search criteria (currently filename and file type).
        *   Remove all indexed images.
    *   Search for images based on:
        *   Partial filename.
        *   File type.
    *   List all images currently in the database.
*   **User Interface**:
    *   Interactive command-line interface (CLI) for all major functions.

## Database Schema

The application uses a single table named `files` with the following structure:

*   `imageid` (INTEGER, PRIMARY KEY, AUTOINCREMENT): Unique identifier for the image record.
*   `imagename` (TEXT, NOT NULL): The filename of the image.
*   `imagepath` (TEXT, NOT NULL, UNIQUE): The full filesystem path to the image.
*   `imagesize` (BIGINT): The size of the image file in bytes.
*   `image_file_type` (TEXT): The file extension/type of the image (e.g., "jpg", "png").
*   `image_creation_time` (DATETIME): The creation timestamp, preferably from EXIF data (e.g., "YYYY-MM-DD HH:MM:SS").
*   `image_modified_time` (DATETIME): The last modified timestamp from the filesystem.
*   `image_entry_time` (DATETIME, DEFAULT CURRENT_TIMESTAMP): The timestamp when the image record was added to the database.

## Dependencies

*   **C++17 Compiler** (e.g., MSVC, GCC, Clang)
*   **CMake** (version 3.10 or higher)
*   **vcpkg** (for C++ library management)
*   **Libraries managed via vcpkg**:
    *   `unofficial-sqlite3`: For SQLite database interaction.
    *   `exiv2`: For reading image metadata (especially EXIF).
    *   `libraw`: For potential RAW image file support (linked).
    *   `opencv`: (linked, primarily for image processing capabilities, current use minimal for image type validation via Exiv2).
    *   `tiff`: Dependency for image format support.
    *   `wxwidgets`: (linked, intended for a future GUI).
    *   `protobuf`: (linked, for potential future structured data serialization).

## Building the Project

1.  **Prerequisites**:
    *   Ensure you have a C++17 compliant compiler, CMake, and Git installed.
    *   Install [vcpkg](https://github.com/microsoft/vcpkg) and integrate it with your build system if you haven't already (`vcpkg integrate install`).
    *   Install the required dependencies using vcpkg:
        ```bash
        vcpkg install tiff wxwidgets exiv2 opencv4 libraw unofficial-sqlite3 protobuf
        ```
        *(Note: Adjust package names if necessary for your vcpkg version or target triplet. The `CMakeLists.txt` expects these to be found.)*

2.  **Clone the Repository**:
    ```bash
    git clone <your-repository-url>
    cd <repository-name>
    ```

3.  **Configure and Build with CMake**:
    *   The `CMakeLists.txt` specifies a path to the vcpkg toolchain file: `C:/Users/karst/vcpkg/scripts/buildsystems/vcpkg.cmake`. **You will likely need to update this path** to match your vcpkg installation location.
    *   Alternatively, you can remove this line from `CMakeLists.txt` if vcpkg is globally integrated or pass it directly in the CMake command.

    ```bash
    # Replace <path-to-your-vcpkg-toolchain-file> with the actual path
    # e.g., C:\vcpkg\scripts\buildsystems\vcpkg.cmake or ~/vcpkg/scripts/buildsystems/vcpkg.cmake

    # For Visual Studio:
    cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=<path-to-your-vcpkg-toolchain-file> -G "Visual Studio 17 2022" -A x64
    cd build
    cmake --build . --config Release

    # For other generators (e.g., Ninja, Makefiles):
    # cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=<path-to-your-vcpkg-toolchain-file> -DCMAKE_BUILD_TYPE=Release
    # cd build
    # cmake --build . --config Release
    ```

4.  **Run the Application**:
    The executable `APT.exe` (or `APT` on Linux/macOS) will be located in the `build/bin/Release` (or `build/bin` if not MSVC) directory.

## Usage

Run the compiled executable. You will be presented with a menu:

```	Image Catalog Menu:
1. Add image(s)
2. Search images
3. Remove image(s)
4. List images
5. Exit
	Enter your choice:
```


*   **1. Add image(s)**:
    *   Choose to add a single image file by providing its full path.
    *   Choose to add all images from a directory. You'll be asked if the search should be recursive.
*   **2. Search images**:
    *   Enter parts of a filename to search for.
    *   Enter a file type (e.g., "jpg", "png") to filter by.
*   **3. Remove image(s)**:
    *   Remove a single image by its full path.
    *   Remove images matching search criteria (filename, file type).
    *   Remove all images from the database.
*   **4. List images**:
    *   Displays all images currently cataloged in the database with their details.
*   **5. Exit**:
    *   Closes the application.

## Program Flow (Internal for Adding Images)

1.  **User Input**: Path to image or directory is provided.
2.  **Directory Traversal**: If a directory is given, it's scanned (recursively or non-recursively).
3.  **Filter Non-Image Files**: Each file is checked using Exiv2 to ensure it's a valid image format. `.xmp` files are explicitly skipped.
4.  **Metadata Extraction**: For valid image files, metadata is extracted:
    *   `imagename`: File's own name.
    *   `imagepath`: Full absolute path.
    *   `imagesize`: File size in bytes.
    *   `image_file_type`: File extension.
    *   `image_creation_time`: From EXIF "DateTimeOriginal" if available, otherwise "N/A".
    *   `image_modified_time`: From filesystem last write time.
5.  **Database Insertion**:
    *   A check is performed to see if an image with the same `imagepath` already exists.
    *   If not, the extracted metadata is inserted into the `files` table in the SQLite database.

## Future Enhancements

*   **Full Tagging System**: Implement the core "tagging" feature, allowing users to add, remove, and search by custom tags. This will likely involve new database tables.
*   **Advanced Search Filters**:
    *   Search/filter by date ranges (creation, modification, entry).
    *   Search/filter by image size range.
    *   Search/filter by tags.
*   **Graphical User Interface (GUI)**: Develop a GUI (potentially using the linked wxWidgets library) for a more user-friendly experience.
*   **Image Previews**: Ability to show thumbnails or previews of images.
*   **Batch Operations**: More sophisticated batch editing of metadata and tags.
*   **Expanded Metadata Support**: Extract and store more types of metadata (e.g., camera model, GPS info, ratings).
*   **Configuration File**: For settings like database path, default recursion, etc.
*   **Improved Error Handling and Logging**: More robust and user-friendly error messages and a dedicated logging mechanism.

## Contributing

Contributions are welcome! Please feel free to fork the repository, make changes, and submit pull requests. For major changes, please open an issue first to discuss what you would like to change.

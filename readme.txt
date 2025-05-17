# Advanced Photo Tagger (APT)

A C++ project for the organization of photographs based upon tags that categorize the images.

# Features

Metadata database (table)
	- imageid
	- imagename
	- imagepath
	- imagesize
	- image_file_type
	- image_creation_time
	- image_modified_time
	- image_entry_time

# Program Flow
1. User Input
	a. Select function
		- Add image(path)
		- Add image folder(path)
		- Remove image(path)
		- Remove image folder(path)
		- Search directory
			- Filter results(date, size, path)
2. Directory traversal
	- Traverse to given directory
	- Scan entire path's directory with recursion
3. Filter out non-image files
	- Use libmagic or OpenCV or extensions
4. Metadata Extraction
	- Extract all metadata inputs for database entry
		- imagename
		- imagepath
		- imagesize
		- image_file_type
		- image_creation_time
		- image_modified_time
5. Database insertion
	- Insert each extracted metadata value into spot in the database table



	cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=C:\Users\karst\vcpkg\scripts\buildsystems\vcpkg.cmake -G "Visual Studio 17 2022" -A x64
	cd build
	cmake --build . --config Release

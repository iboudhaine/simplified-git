#include "../../include/File.h"
#include <iostream>

void testFileOperations() {
    // Create a File object with a test file
    File testFile("test_file.txt");

    // Test getting file name
    std::cout << "File Name: " << testFile.getName() << std::endl;

    // Test getting file content
    std::cout << "File Content: " << testFile.getContent() << std::endl;

    // Test getting file hash
    std::cout << "File Hash: " << testFile.getHash().value() << std::endl;
}

int main() {
    testFileOperations();

    return 0;
}

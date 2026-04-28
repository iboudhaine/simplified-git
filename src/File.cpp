#include "includes/File.h"
#include <iostream>
#include <fstream>
#include <string>
#include <functional>

/**
 * @brief Constructs a File object with the given name, reads its content, and calculates the hash.
 * @param name The name of the file.
 */
File::File(const std::string& name) : name(name) {
    readContent();
    calculateHash();
}

/**
 * @brief Gets the name of the file.
 * @return The name of the file.
 */
const std::string& File::getName() const {
    return name;
}

/**
 * @brief Gets the content of the file.
 * @return The content of the file.
 */
const std::string& File::getContent() const {
    return content;
}

/**
 * @brief Gets the hash value of the file content.
 * @return The hash value of the file content, or std::nullopt if not calculated.
 */
std::optional<size_t> File::getHash() const {
    return hash;
}

/**
 * @brief Reads the content of the file from disk.
 */
void File::readContent() {
    std::ifstream file(name);

    if (file.is_open()) {
        content.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();
    } else {
        std::cerr << "Error opening file: " << name << std::endl;
    }
}

/**
 * @brief Calculates and sets the hash value of the file content.
 */
void File::calculateHash() {
    std::hash<std::string> hasher;
    hash = hasher(content);
}

#ifndef FILE_H
#define FILE_H

#include <string>
#include <optional>

/**
 * @brief The File class represents a file, storing its name, content, and an optional hash value.
 */
class File {
public:
    /**
     * @brief Constructs a File object with the given name.
     * @param name The name of the file.
     */
    File(const std::string& name);

    /**
     * @brief Gets the name of the file.
     * @return The name of the file.
     */
    const std::string& getName() const;

    /**
     * @brief Gets the content of the file.
     * @return The content of the file.
     */
    const std::string& getContent() const;

    /**
     * @brief Gets the hash value of the file content.
     * @return The hash value of the file content, or std::nullopt if not calculated.
     */
    std::optional<size_t> getHash() const;

    /**
     * @brief Reads the content of the file from disk.
     */
    void readContent();

    /**
     * @brief Calculates the hash value of the file content.
     */
    void calculateHash();

private:
    std::string name;                    ///< The name of the file.
    std::string content;                 ///< The content of the file.
    std::optional<size_t> hash;          ///< The hash value of the file content.
};

#endif // FILE_H

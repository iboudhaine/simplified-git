#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <string>
#include <map>
#include <optional>
#include <vector>

enum class Operation {
    Init,
    Add,
    Commit,
    Status
};


/**
 * @brief The Repository class manages a version-controlled repository, providing operations like init, add, commit, and status.
 */
class Repository {
public:
    /**
     * @brief Constructs a Repository object with the specified repository path.
     * @param repoPath The path to the repository.
     */
    Repository(const std::string& repoPath);


    /**
     * @brief Initializes the repository.
     * @return A message indicating the initialization status.
     */
    std::string init();

    /**
     * @brief Adds a file to the repository.
     * @param fileName The name of the file to add.
     * @return A message indicating the add status.
     */
    std::string add(const std::string& fileName);

    /**
     * @brief Commits changes to a file in the repository.
     * @param fileName The name of the file to commit.
     * @param message The commit message.
     * @return A message indicating the commit status.
     */
    std::string commit(const std::string& fileName, const std::string& message);

    /**
     * @brief Logs an operation with a message.
     * @param operation The type of operation (Init, Add, Commit, etc.).
     * @param message The log message.
     */
    void log(Operation operation, const std::string& message) const;

    /**
     * @brief Gets the status of the repository.
     * @return A vector of strings indicating the status of files in the repository.
     */
    std::vector<std::string> status() const;

    std::map<std::string, std::optional<size_t>> committedFiles; ///< Map of committed files and their hash values.
    std::string repoPath; ///< The path to the repository.



private:
    std::string gitFolderPath; ///< The path to the repository's .git folder.
    std::string logFilePath; ///< The path to the log file.
    std::string filesPath; ///< The path to the file tracking committed files.

    /**
     * @brief Loads information about committed files from the filesPath.
     */
    void loadCommittedFiles();

    /**
     * @brief Saves committed files information to the filesPath and logs a commit message.
     * @param message The commit message.
     */
    void saveCommittedFiles(const std::string& message) const;

    /**
     * @brief Overwrites the filesPath file with committed files information.
     */
    void overwriteFilesFile() const;
};

#endif // REPOSITORY_H

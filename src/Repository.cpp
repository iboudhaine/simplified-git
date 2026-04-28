#include "includes/Repository.h"
#include "includes/File.h"
#include <fstream>
#include <iostream>
#include <optional>
#include <experimental/filesystem>
#include <vector>
#include <chrono>
#include <ctime>

namespace fs = std::experimental::filesystem;


/**
 * @brief Constructs a Repository object with the specified repository path.
 * @param repoPath The path to the repository.
 */
Repository::Repository(const std::string& repoPath) : repoPath(repoPath) {
    gitFolderPath = repoPath + "/git";
    logFilePath = gitFolderPath + "/log";
    filesPath = gitFolderPath + "/files";

    loadCommittedFiles();
}

/**
 * @brief Adds a file to the list of committed files.
 * @param fileName The name of the file to add.
 * @return A message indicating the add status.
 */
std::string Repository::add(const std::string& fileName) {
    if (std::ifstream(repoPath + "/" + fileName)) {
        auto it = committedFiles.find(fileName);

        if (it == committedFiles.end()) {
            committedFiles[fileName] = std::nullopt;
            log(Operation::Add, "File added successfully");
            std::cout << "Added: " << fileName << std::endl;
            return "Added: " + fileName;
        } else {
            log(Operation::Add, "File already added to the repository");
            std::cout << "File already added to the repository: " << fileName << std::endl;
            return "File already added to the repository: " + fileName;
        }
    } else {
        log(Operation::Add, "File does not exist in the repository");
        std::cout << "File does not exist in the repository. Cannot add." << std::endl;
        return "File does not exist in the repository. Cannot add.";
    }
}


/**
 * @brief Commits changes to a file in the repository.
 * @param fileName The name of the file to commit.
 * @param message The commit message.
 * @return A message indicating the commit status.
 */
std::string Repository::commit(const std::string& fileName, const std::string& message) {
    if (std::ifstream(repoPath + "/" + fileName)) {
        auto it = committedFiles.find(fileName);

        if (it != committedFiles.end()) {
            File file(repoPath + "/" + fileName);
            file.calculateHash();
            std::optional<size_t> currentHash = file.getHash();

            if (!it->second.has_value() || currentHash != it->second.value()) {
                committedFiles[fileName] = currentHash;
                saveCommittedFiles(message);
                std::cout << "Committed changes for file: " << fileName << std::endl;
                return "Committed changes for file: " + fileName;
            } else {
                log(Operation::Commit, "No changes to commit for file");
                std::cout << "No changes to commit for file: " << fileName << std::endl;
                return "No changes to commit for file: " + fileName;
            }
        } else {
            log(Operation::Commit, "File not found in committed files");
            std::cout << "File not found in committed files. Add the file first." << std::endl;
            return "File not found in committed files. Add the file first.";
        }
    } else {
        log(Operation::Commit, "File does not exist in the repository");
        std::cout << "File does not exist in the repository. Cannot commit." << std::endl;
        return "File does not exist in the repository. Cannot commit.";
    }
}

/**
 * @brief Logs an operation with a message.
 * @param operation The type of operation (Init, Add, Commit, etc.).
 * @param message The log message.
 */
void Repository::log(Operation operation, const std::string& message) const {
    std::ofstream logFile(logFilePath, std::ios::app);

    if (!logFile.is_open()) {
        std::cerr << "Error opening log file: " << logFilePath << std::endl;
        return;
    }

    std::string operationStr;
    switch (operation) {
        case Operation::Add:
            operationStr = "Add";
            break;
        case Operation::Commit:
            operationStr = "Commit";
            break;
    }

    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    std::tm* now_tm = std::localtime(&now_c);
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", now_tm);


    logFile << std::string(buffer) << " " << operationStr << ": " << message << std::endl;
    logFile << "--------------------------------" << std::endl;

    logFile.close();
}


/**
 * @brief Loads information about committed files from the filesPath.
 */
void Repository::loadCommittedFiles() {
    std::ifstream filesFile(filesPath);
    std::string fileName;
    size_t hash;

    while (filesFile >> fileName >> hash) {
        committedFiles[fileName] = hash;
    }
}


/**
 * @brief Overwrites the filesPath file with committed files information.
 */
void Repository::overwriteFilesFile() const {
    std::ofstream filesFile(filesPath, std::ios::trunc);

    if (!filesFile.is_open()) {
        std::cerr << "Error opening file for overwriting: " << filesPath << std::endl;
        return;
    }

    for (const auto& entry : committedFiles) {
        if (entry.second.has_value()) {
            filesFile << entry.first << " " << entry.second.value() << std::endl;
        }
    }

    filesFile.close();
}


/**
 * @brief Saves committed files information to the filesPath and logs a commit message.
 * @param message The commit message.
 */
void Repository::saveCommittedFiles(const std::string& message) const {
    overwriteFilesFile();
    log(Operation::Commit, message);
}


/**
 * @brief Gets the status of the repository.
 * @return A vector of strings indicating the status of files in the repository.
 */
std::vector<std::string> Repository::status() const {

    std::vector<std::string> v;

    std::cout << "Repository Status:" << std::endl;

    for (const auto& file : fs::directory_iterator(repoPath)) {
        std::string fileName = file.path().filename().string();
        if (fileName != "git") {
            auto it = committedFiles.find(fileName);

            if (it == committedFiles.end()) {
                v.push_back(fileName + " is untracked.");
                std::cout << fileName << " is untracked." << std::endl;
            } else {
                File file(repoPath + "/" + fileName);
                auto currentHash = file.getHash();

                if (!it->second.has_value()) {
                    v.push_back(fileName + " added but not committed.");
                    std::cout << fileName << " added but not committed." << std::endl;
                } else if (currentHash != it->second.value()) {
                    v.push_back(fileName + " has been modified.");
                    std::cout << fileName << " has been modified." << std::endl;
                } else {
                    v.push_back(fileName + " is up to date.");
                    std::cout << fileName << " is up to date." << std::endl;
                }
            }
        }
    }

    std::cout << "-------------------" << std::endl;

    return v;
}


/**
 * @brief Initializes the repository.
 * @return A message indicating the initialization status.
 */
std::string Repository::init() {
    std::string gitFolderPath = repoPath + "/git";

    if (fs::exists(gitFolderPath)) {
        std::cout << "Repository already initialized at " << repoPath << std::endl;
        return "Opened repository at: " + repoPath;
    } else {
        fs::create_directories(gitFolderPath);

        std::ofstream logFile(gitFolderPath + "/log");
        logFile.close();

        std::ofstream filesFile(gitFolderPath + "/Files");
        filesFile.close();

        std::cout << "Initialized empty repository in " << repoPath << std::endl;
        return "Initialized empty repository in " + repoPath;
    }
}


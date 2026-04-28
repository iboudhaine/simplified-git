#include "../../include/Repository.h"
#include <iostream>
#include <fstream>

void createTestFile(const std::string& filePath) {
    std::ofstream testFile(filePath);
    testFile << "Initial content.";
    testFile.close();
}

void clearFiles(const std::string& filePath1, const std::string& filePath2) {
    std::ofstream file1(filePath1, std::ios::trunc);
    file1.close();
    std::ofstream file2(filePath2, std::ios::trunc);
    file2.close();
}

void testRepositoryOperations() {
    // Provide the path to the existing repository folder
    std::string repoPath = "repositoryTest";
    Repository repo(repoPath);
    std::string msg = "Sample commit message";

    // Create the test file
    createTestFile(repoPath + "/myTestFile.txt");

    // Test adding a file
    repo.add("myTestFile.txt");
    repo.status();

    // Test committing changes
    repo.commit("myTestFile.txt", msg);
    repo.status();

    // Test adding the same file again
    repo.add("myTestFile.txt");
    repo.status();

    // Test committing again
    repo.commit("myTestFile.txt", msg);
    repo.status();

    // Modify the content of the test file
    std::ofstream testFile(repoPath + "/myTestFile.txt", std::ios::app);
    testFile << "\nAdditional content.";
    testFile.close();

    // Commit changes
    repo.commit("myTestFile.txt", msg);
    repo.status();

    // Delete the test file from the repository
    if (remove((repoPath + "/myTestFile.txt").c_str()) == 0) {
        std::cout << "File deleted successfully." << std::endl;
    } else {
        std::cout << "Error deleting file." << std::endl;
    }

    // Try to adding after deleting the file
    repo.add("myTestFile.txt");
    repo.status();

    // Try to commit after deleting the file
    repo.commit("myTestFile.txt", msg);
    repo.status();

    // Clear the log and files files
    clearFiles(repoPath + "/git/log", repoPath + "/git/files");
}


int main() {
    testRepositoryOperations();

    return 0;
}


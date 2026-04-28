#include "../../include/Repository.h"
#include <iostream>
#include <fstream>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

void testInitFunction() {
    std::string repoPath = "Repository";

    // First initialization
    Repository repo(repoPath);
    repo.init();

    // Check if .git, log, and FilesFile were created
    /*if (fs::exists(repoPath + "/.git") && 
        fs::exists(repoPath + "/.git/log") && 
        fs::exists(repoPath + "/.git/FilesFile")) {
        std::cout << "Repository initialized correctly." << std::endl;
    } else {
        std::cerr << "Repository initialization failed." << std::endl;
    }*/

    // Second initialization (should recognize as already initialized)
    repo.init();

    // fs::remove_all(repoPath);
}

int main() {
    testInitFunction();
    
    return 0;
}
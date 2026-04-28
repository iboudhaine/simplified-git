#include "Git.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    std::string repoPath;
    if (argc >= 2) {
        repoPath = argv[1];
    } else {
        std::cout << "Repository path: ";
        std::getline(std::cin, repoPath);
    }

    Git git(repoPath);
    git.repl();
    return 0;
}

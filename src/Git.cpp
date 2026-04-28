#include "includes/Git.h"
#include "includes/File.h"


/**
 * @brief Constructs a Git object with the specified repository path.
 * @param repoPath The path to the repository.
 */
Git::Git(const std::string& repoPath) : repository(repoPath) {}


/**
 * @brief Enters the Git command-line interface (REPL).
 */
void Git::repl() {
    std::string input;
    Command command;

    while (true) {
        std::cout << "git> ";
        std::getline(std::cin, input);

        command = parseCommand(input);

        switch (command) {
        case Command::Init:
            repository.init();
            break;
        case Command::Add:
            handleAdd();
            break;
        case Command::Commit:
            handleCommit();
            break;
        case Command::Status:
            repository.status();
            break;
        case Command::Exit:
            return;
        case Command::Unknown:
            std::cout << "Unknown command." << std::endl;
            break;
        }
    }
}


/**
 * @brief Parses a user input command.
 * @param input The user input string.
 * @return The parsed Git command.
 */
Git::Command Git::parseCommand(const std::string& input) {
    if (input == "init") {
        return Command::Init;
    } else if (input.find("add") == 0) {
        // Assuming the format is "add [filename]"
        return Command::Add;
    } else if (input.find("commit") == 0) {
        // Assuming the format is "commit [commit message]"
        return Command::Commit;
    } else if (input == "status") {
        return Command::Status;
    } else if (input == "exit") {
        return Command::Exit;
    } else {
        return Command::Unknown;
    }
}


/**
 * @brief Handles the "add" command.
 */
void Git::handleAdd() {
    std::string fileName;
    std::cout << "Enter file name to add: ";
    std::getline(std::cin, fileName);
    repository.add(fileName);
}


/**
 * @brief Handles the "commit" command.
 */
void Git::handleCommit() {
    std::string message;
    std::cout << "Enter commit message: ";
    std::getline(std::cin, message);

    for (auto it: repository.committedFiles) {
        File file(repository.repoPath + "/" + it.first);
        std::optional<size_t> currentHash = file.getHash();
        if (! it.second.has_value() || it.second.value() != currentHash) {
            repository.commit(it.first, message);
        }
    }
}

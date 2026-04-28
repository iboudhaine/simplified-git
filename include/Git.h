#ifndef GIT_H
#define GIT_H

#include <iostream>
#include <string>
#include "Repository.h"


/**
 * @brief The Git class provides a command-line interface for interacting with a Git repository.
 */
class Git {
public:

    /**
     * @brief The Command enumeration represents different Git commands.
     */
    enum class Command {
        Init,
        Add,
        Commit,
        Status,
        Exit,
        Unknown
    };


    /**
     * @brief Constructs a Git object with the specified repository path.
     * @param repoPath The path to the repository.
     */
    Git(const std::string& repoPath);


    /**
     * @brief Enters the Git command-line interface (REPL).
     */
    void repl();

private:
    Repository repository;


    /**
     * @brief Parses a user input command.
     * @param input The user input string.
     * @return The parsed Git command.
     */
    Command parseCommand(const std::string& input);


    /**
     * @brief Handles the "add" command.
     */
    void handleAdd();


    /**
     * @brief Handles the "commit" command.
     */
    void handleCommit();
};

#endif

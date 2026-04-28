#include "Repository.h"
#include "test_helpers.h"

#include <filesystem>

namespace fs = std::filesystem;
using namespace sgit_test;

void initCreatesGitFolder() {
    ScopedTmpDir tmp;
    Repository repo(tmp.str());
    repo.init();

    CHECK(fs::exists(tmp.path() / "git"));
    CHECK(fs::exists(tmp.path() / "git" / "log"));
    CHECK(fs::exists(tmp.path() / "git" / "files"));
}

void initIsIdempotent() {
    ScopedTmpDir tmp;
    Repository repo(tmp.str());
    repo.init();
    repo.init();  // should not throw, should not corrupt
    CHECK(fs::exists(tmp.path() / "git"));
}

void addRegistersFile() {
    ScopedTmpDir tmp;
    Repository repo(tmp.str());
    repo.init();
    writeFile(tmp.path() / "a.txt", "hello");

    repo.add("a.txt");

    const auto& tracked = repo.getCommittedFiles();
    CHECK(tracked.count("a.txt") == 1);
    CHECK(!tracked.at("a.txt").has_value());  // added but not committed
}

void addMissingFileIsRejected() {
    ScopedTmpDir tmp;
    Repository repo(tmp.str());
    repo.init();

    repo.add("nonexistent.txt");

    CHECK(repo.getCommittedFiles().count("nonexistent.txt") == 0);
}

void commitStoresHash() {
    ScopedTmpDir tmp;
    Repository repo(tmp.str());
    repo.init();
    writeFile(tmp.path() / "a.txt", "v1");

    repo.add("a.txt");
    repo.commit("a.txt", "first");

    const auto& tracked = repo.getCommittedFiles();
    CHECK(tracked.at("a.txt").has_value());
}

void commitWithoutAddIsRejected() {
    ScopedTmpDir tmp;
    Repository repo(tmp.str());
    repo.init();
    writeFile(tmp.path() / "a.txt", "x");

    repo.commit("a.txt", "msg");

    CHECK(repo.getCommittedFiles().count("a.txt") == 0);
}

void commitDetectsModification() {
    ScopedTmpDir tmp;
    Repository repo(tmp.str());
    repo.init();
    writeFile(tmp.path() / "a.txt", "v1");

    repo.add("a.txt");
    repo.commit("a.txt", "first");
    auto firstHash = repo.getCommittedFiles().at("a.txt");

    writeFile(tmp.path() / "a.txt", "v2");
    repo.commit("a.txt", "second");
    auto secondHash = repo.getCommittedFiles().at("a.txt");

    CHECK(firstHash.has_value());
    CHECK(secondHash.has_value());
    CHECK(firstHash != secondHash);
}

void noopCommitOnUnchangedFile() {
    ScopedTmpDir tmp;
    Repository repo(tmp.str());
    repo.init();
    writeFile(tmp.path() / "a.txt", "v1");

    repo.add("a.txt");
    repo.commit("a.txt", "first");
    auto firstHash = repo.getCommittedFiles().at("a.txt");
    repo.commit("a.txt", "again");
    auto secondHash = repo.getCommittedFiles().at("a.txt");

    CHECK(firstHash == secondHash);
}

void persistsAcrossInstances() {
    ScopedTmpDir tmp;
    {
        Repository repo(tmp.str());
        repo.init();
        writeFile(tmp.path() / "a.txt", "persisted");
        repo.add("a.txt");
        repo.commit("a.txt", "msg");
    }

    Repository reopened(tmp.str());
    reopened.init();
    const auto& tracked = reopened.getCommittedFiles();
    CHECK(tracked.count("a.txt") == 1);
    CHECK(tracked.at("a.txt").has_value());
}

void statusReflectsLifecycle() {
    ScopedTmpDir tmp;
    Repository repo(tmp.str());
    repo.init();
    writeFile(tmp.path() / "a.txt", "v1");
    writeFile(tmp.path() / "b.txt", "v1");

    auto s1 = repo.status();
    bool a_untracked = false, b_untracked = false;
    for (const auto& line : s1) {
        if (line.find("a.txt is untracked") != std::string::npos) a_untracked = true;
        if (line.find("b.txt is untracked") != std::string::npos) b_untracked = true;
    }
    CHECK(a_untracked);
    CHECK(b_untracked);

    repo.add("a.txt");
    auto s2 = repo.status();
    bool a_added = false;
    for (const auto& line : s2) {
        if (line.find("a.txt added but not committed") != std::string::npos) a_added = true;
    }
    CHECK(a_added);

    repo.commit("a.txt", "msg");
    auto s3 = repo.status();
    bool a_uptodate = false;
    for (const auto& line : s3) {
        if (line.find("a.txt is up to date") != std::string::npos) a_uptodate = true;
    }
    CHECK(a_uptodate);

    writeFile(tmp.path() / "a.txt", "v2");
    auto s4 = repo.status();
    bool a_modified = false;
    for (const auto& line : s4) {
        if (line.find("a.txt has been modified") != std::string::npos) a_modified = true;
    }
    CHECK(a_modified);
}

void filesFileIsLowercase() {
    ScopedTmpDir tmp;
    Repository repo(tmp.str());
    repo.init();

    CHECK(fs::exists(tmp.path() / "git" / "files"));
    CHECK(!fs::exists(tmp.path() / "git" / "Files"));
}

int main() {
    RUN(initCreatesGitFolder);
    RUN(initIsIdempotent);
    RUN(addRegistersFile);
    RUN(addMissingFileIsRejected);
    RUN(commitStoresHash);
    RUN(commitWithoutAddIsRejected);
    RUN(commitDetectsModification);
    RUN(noopCommitOnUnchangedFile);
    RUN(persistsAcrossInstances);
    RUN(statusReflectsLifecycle);
    RUN(filesFileIsLowercase);
    return report();
}

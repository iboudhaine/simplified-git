#include "File.h"
#include "test_helpers.h"

using namespace sgit_test;

void readsContent() {
    ScopedTmpDir tmp;
    auto p = tmp.path() / "a.txt";
    writeFile(p, "hello world");

    File f(p.string());
    CHECK(f.getContent() == "hello world");
    CHECK(f.getName() == p.string());
}

void hashIsDeterministic() {
    ScopedTmpDir tmp;
    auto p1 = tmp.path() / "a.txt";
    auto p2 = tmp.path() / "b.txt";
    writeFile(p1, "same content");
    writeFile(p2, "same content");

    File f1(p1.string());
    File f2(p2.string());
    CHECK(f1.getHash().has_value());
    CHECK(f2.getHash().has_value());
    CHECK(f1.getHash() == f2.getHash());
}

void hashChangesWithContent() {
    ScopedTmpDir tmp;
    auto p = tmp.path() / "a.txt";
    writeFile(p, "first");
    File f1(p.string());

    writeFile(p, "second");
    File f2(p.string());

    CHECK(f1.getHash() != f2.getHash());
}

void emptyContentHasHash() {
    ScopedTmpDir tmp;
    auto p = tmp.path() / "empty.txt";
    writeFile(p, "");

    File f(p.string());
    CHECK(f.getContent().empty());
    CHECK(f.getHash().has_value());
}

int main() {
    RUN(readsContent);
    RUN(hashIsDeterministic);
    RUN(hashChangesWithContent);
    RUN(emptyContentHasHash);
    return report();
}

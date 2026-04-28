#ifndef SGIT_TEST_HELPERS_H
#define SGIT_TEST_HELPERS_H

#include <atomic>
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace sgit_test {

inline int& failure_count() {
    static int n = 0;
    return n;
}

inline int& check_count() {
    static int n = 0;
    return n;
}

#define CHECK(cond)                                                            \
    do {                                                                       \
        ++::sgit_test::check_count();                                          \
        if (!(cond)) {                                                         \
            ++::sgit_test::failure_count();                                    \
            std::cerr << "  [FAIL] " << __FILE__ << ":" << __LINE__            \
                      << "  CHECK(" #cond ")" << std::endl;                    \
        }                                                                      \
    } while (0)

#define RUN(test_fn)                                                           \
    do {                                                                       \
        std::cout << "[RUN ] " #test_fn << std::endl;                          \
        int before = ::sgit_test::failure_count();                             \
        test_fn();                                                             \
        std::cout << (before == ::sgit_test::failure_count() ? "[ OK ] "       \
                                                             : "[FAIL] ")      \
                  << #test_fn << std::endl;                                    \
    } while (0)

inline int report() {
    int failed = failure_count();
    int total = check_count();
    std::cout << "\n" << (total - failed) << "/" << total << " checks passed";
    if (failed == 0) {
        std::cout << ". All good." << std::endl;
        return 0;
    }
    std::cout << ", " << failed << " failed." << std::endl;
    return 1;
}

// RAII wrapper that creates a unique temporary directory and removes it
// on destruction. Each test gets its own isolated working area.
class ScopedTmpDir {
public:
    ScopedTmpDir() {
        static std::atomic<int> counter{0};
        auto base = std::filesystem::temp_directory_path();
        auto stamp = std::chrono::steady_clock::now().time_since_epoch().count();
        path_ = base / ("sgit-test-" + std::to_string(stamp) + "-" +
                        std::to_string(counter.fetch_add(1)));
        std::filesystem::create_directories(path_);
    }

    ~ScopedTmpDir() {
        std::error_code ec;
        std::filesystem::remove_all(path_, ec);
    }

    ScopedTmpDir(const ScopedTmpDir&) = delete;
    ScopedTmpDir& operator=(const ScopedTmpDir&) = delete;

    const std::filesystem::path& path() const { return path_; }
    std::string str() const { return path_.string(); }

private:
    std::filesystem::path path_;
};

inline void writeFile(const std::filesystem::path& p, const std::string& content) {
    std::ofstream out(p);
    out << content;
}

inline std::string readFile(const std::filesystem::path& p) {
    std::ifstream in(p);
    return std::string((std::istreambuf_iterator<char>(in)),
                       std::istreambuf_iterator<char>());
}

}  // namespace sgit_test

#endif

# Tests

Unit tests for the core library, run via CTest. `test_file.cpp` covers the `File` class, reading content from disk and the determinism, sensitivity, and edge-cases of its hash. `test_repository.cpp` covers the `Repository` class, `init` (creation, idempotence, correct file casing), `add` (registration and rejection of missing files), `commit` (hash storage, modification detection, no-op skipping, rejection without prior add), `status` across the full file lifecycle, and persistence of tracked state across `Repository` instances. Each test runs in its own throwaway temporary directory, so there's no shared state between runs.

Run with `ctest` from the build directory after `cmake --build build`.

add_executable(test_file test_file.cpp)
target_link_libraries(test_file PRIVATE sgit_core)
target_include_directories(test_file PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
add_test(NAME File COMMAND test_file)

add_executable(test_repository test_repository.cpp)
target_link_libraries(test_repository PRIVATE sgit_core)
target_include_directories(test_repository PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
add_test(NAME Repository COMMAND test_repository)

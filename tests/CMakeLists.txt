include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(constraints test_constraints.cpp)
add_executable(sudoku_backtracking test_sudoku_backtracking.cpp)

target_include_directories(constraints PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(sudoku_backtracking PUBLIC ${CMAKE_SOURCE_DIR}/include)


add_test(constraints_test constraints)
add_test(backtracking_test sudoku_backtracking)
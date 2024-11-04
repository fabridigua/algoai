include_directories(../include)

add_executable(constraints test_constraints.cpp)

add_test(constraints_test constraints)
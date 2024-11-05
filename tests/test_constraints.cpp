#include <iostream>
#include <vector>
#include <unordered_map>

#include "../include/constraints/utils/csp_constraint.h"

int main() {
    std::cout << "Test Sudoku Constraints.." << std::endl;
    
    // Test for CSP constraints
    using namespace CSP;

    Constraint<int> _all_odd({{0},{1},{2}},
        [](std::vector<int> values) -> bool 
        {
            for (int& value : values) { if(value%2 == 0)  return false; }
            return true;
        }
    );

    Assignment<int> _true_assigment({{0, 1}, {1, 9},{2, 7}});
    Assignment<int> _partial_assigment({{0, 1}, {2, 9}});
    Assignment<int> _wrong_assigment({{0, 1}, {1, 10},{2, 2}});

    bool test_ok = _all_odd.is_satisfied(_true_assigment)
                    && _all_odd.is_satisfied(_partial_assigment)
                    && !_all_odd.is_satisfied(_wrong_assigment);

    std::cout << "_all_odd.is_satisfied(_true_assigment) " << _all_odd.is_satisfied(_true_assigment) << std::endl;
    std::cout << "_all_odd.is_satisfied(_partial_assigment) " << _all_odd.is_satisfied(_partial_assigment) << std::endl;
    std::cout << "_all_odd.is_satisfied(_wrong_assigment) " << _all_odd.is_satisfied(_wrong_assigment) << std::endl;


    std::cout << "All Constraints tests completed." << std::endl;

    return test_ok ? 0: -1;
}
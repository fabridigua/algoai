#include <iostream>
#include <vector>
#include <unordered_map>

#include "../include/constraints/csp_problem.h"
#include "../include/constraints/csp_solver.h"
#include <unordered_set>

using namespace CSP;

int main() {
    std::cout << "Test Sudoku Backtracking Solver.." << std::endl;
    
    CSProblem<int> sudoku;
    std::vector<int> domain_values = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    for (int i = 1; i <= 9; i++)
    {
        for (int j = 1; j <= 9; j++)
        {
            Variable v(i*10 + j);
            Domain<int> d(domain_values);
            sudoku.addVariable(v, d);            
        }
    }

    sudoku.assignValue({13},2);
    sudoku.assignValue({16},7);
    sudoku.assignValue({22},6);
    sudoku.assignValue({24},9);
    sudoku.assignValue({26},3);
    sudoku.assignValue({27},4);
    sudoku.assignValue({32},9);
    sudoku.assignValue({34},2);
    sudoku.assignValue({35},5);
    sudoku.assignValue({39},3);
    sudoku.assignValue({44},4);
    sudoku.assignValue({47},1);
    sudoku.assignValue({51},7);
    sudoku.assignValue({52},3);
    sudoku.assignValue({55},6);
    sudoku.assignValue({63},9);
    sudoku.assignValue({64},5);
    sudoku.assignValue({65},3);
    sudoku.assignValue({68},6);
    sudoku.assignValue({73},6);
    sudoku.assignValue({74},3);
    sudoku.assignValue({75},4);
    sudoku.assignValue({78},7);
    sudoku.assignValue({81},8);
    sudoku.assignValue({89},9);
    sudoku.assignValue({98},5);

    // Add sudoku rules
    auto all_different_rule = [](std::vector<int> values) -> bool
    {        
        std::unordered_set<int> uniqueValues(values.begin(), values.end());
        return uniqueValues.size() == values.size();
    };

    // All row and column values must be different
    for (int i = 1; i <= 9; i++)
    {
        std::vector<Variable> row;
        std::vector<Variable> col;
        for (int j = 1; j <= 9; j++)
        {
            row.push_back(i*10 + j);
            col.push_back(j*10 + i);           
        }
        sudoku.addConstraint({row, all_different_rule});
        sudoku.addConstraint({col, all_different_rule});
    }

    // Al square mus have different values
    std::vector<int> square_ids = {11,14,17,41,44,47,71,74,77};
    for (int k = 0; k < 9; k++)
    {
        std::vector<Variable> square;

        for (size_t i = 0; i < 3; i++)
            for (size_t j = 0; j < 3; j++)
                square.push_back(square_ids[k] + 10*i + j);   
        
        sudoku.addConstraint({square, all_different_rule});
    }
    
    // Create Solver
    CSPSolver<int> solver(CSPSolver<int>::CSPSrategy::Backtracking);

    solver._enableChrono();

    bool solved = solver.solve(sudoku);

    std::cout << "solved? " << solved << std::endl;

    // Print solution
    std::unordered_map<Variable, int> solution;
    sudoku.getSolution(&solution);

    for (int i = 1; i <= 9; i++)
    {
        for (int j = 1; j <= 9; j++)
        {
            std::cout << solution[i*10 + j] << " ";
            if(j == 3 || j== 6) std::cout << "| ";
            if(j == 9) std::cout << std::endl;
        }
        if(i==3 || i == 6) std::cout << "- - -   - - -   - - -"<< std::endl;        
    }

    // Test scheme solution:
    // 3 4 2 | 6 8 7 | 9 1 5
    // 5 6 8 | 9 1 3 | 4 2 7
    // 1 9 7 | 2 5 4 | 6 8 3
    // - - -   - - -   - - -
    // 6 8 5 | 4 7 9 | 1 3 2
    // 7 3 4 | 1 6 2 | 5 9 8
    // 2 1 9 | 5 3 8 | 7 6 4
    // - - -   - - -   - - -
    // 9 2 6 | 3 4 5 | 8 7 1
    // 8 5 1 | 7 2 6 | 3 4 9
    // 4 7 3 | 8 9 1 | 2 5 6

    std::unordered_map<int, int> test_sudoku = {
        {11, 3}, {12, 4}, {13, 2}, {14, 6}, {15, 8}, {16, 7}, {17, 9}, {18, 1}, {19, 5},
        {21, 5}, {22, 6}, {23, 8}, {24, 9}, {25, 1}, {26, 3}, {27, 4}, {28, 2}, {29, 7},
        {31, 1}, {32, 9}, {33, 7}, {34, 2}, {35, 5}, {36, 4}, {37, 6}, {38, 8}, {39, 3},

        {41, 6}, {42, 8}, {43, 5}, {44, 4}, {45, 7}, {46, 9}, {47, 1}, {48, 3}, {49, 2},
        {51, 7}, {52, 3}, {53, 4}, {54, 1}, {55, 6}, {56, 2}, {57, 5}, {58, 9}, {59, 8},
        {61, 2}, {62, 1}, {63, 9}, {64, 5}, {65, 3}, {66, 8}, {67, 7}, {68, 6}, {69, 4},

        {71, 9}, {72, 2}, {73, 6}, {74, 3}, {75, 4}, {76, 5}, {77, 8}, {78, 7}, {79, 1},
        {81, 8}, {82, 5}, {83, 1}, {84, 7}, {85, 2}, {86, 6}, {87, 3}, {88, 4}, {89, 9},
        {91, 4}, {92, 7}, {93, 3}, {94, 8}, {95, 9}, {96, 1}, {97, 2}, {98, 5}, {99, 6}
    };

    bool solution_ok = true;
    for (auto cell: test_sudoku) if(cell.second != solution[cell.first]) solution_ok = false;    

    std::cout << "Sudoku test result: " << (solution_ok? "passed":"failed") << std::endl;

    return solution_ok;
}
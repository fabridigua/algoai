#pragma once

#include <vector>
#include <unordered_map>

#include "csp_problem.h"

namespace CSP
{

    template <typename T>
    class CSPSolver
    {

    enum class CSPSrategy
    {
        Backtracking
    }

    public:
        CSPSolver(CSPSrategy s):_strategy(s){};
        ~CSPSolver(){};

        bool solve(CSProblem<T>& problem);

    private: 
        CSPSrategy _startegy;


    };

    

};

#pragma once

#include <vector>
#include <algorithm>
#include <unordered_map>

#include <constraints/csp_problem.h>

namespace CSP
{

    template <typename T>
    class CSPSolver
    {

    public:
        enum class CSPSrategy
        {
            Backtracking,
            BacktrackingEuristics
            //  Forward-Checking
            //Arc Consistency (AC-3)
            // Local Search con Propagazione di Vincoli (Min-Conflicts)
            // Algoritmo di Ricerca con Backjumping
            // Tecniche di Propagazione di Vincoli Avanzate (e.g., Constraint Propagation)
        };


        CSPSolver(CSPSrategy s):_strategy(s){};
        ~CSPSolver(){};

        bool solve(CSProblem<T>& problem)
        {
            switch (_strategy)
            {
                case CSPSrategy::Backtracking:
                    return backtracking(problem);
                case CSPSrategy::BacktrackingEuristics:
                    return backtracking_with_euristics(problem);
                default:
                    return false;
            }

            return false;
        }

    private: 
        CSPSrategy _strategy;

        // Utility solver methods
        // Check if an assignment is correct
        bool isValidAssignment(const CSProblem<T>& problem, Variable v, T value)
        {
            std::unordered_map<Variable, T> singleAssignment = {{v, value}};

            for (Constraint<T> c: problem.getConstraints())
            {
                // Copy single assisgment to test constraint
                Assignment<T> assignment = singleAssignment;

                // If constraint doen't involve variable v, don't check it
                if(!c.has_variable(v)) continue;

                //Fill the assignment with missing variables
                std::unordered_map<Variable, T> currentState = problem.getCurrentState();
                for(auto cv: c.getConstraintVariables())
                {
                    // If the value is not assigned yet, consider as a valid assignment
                    if(!currentState.contains(cv)) continue;

                    if(!assignment.getVariables().contains(cv)) assignment.assign(cv, currentState[cv]);
                }

                // Check if the assignment is satisfied
                if(!c.is_satisfied(assignment)) 
                    return false;
            }

            return true;
        }

        // Strategies
        bool backtracking(CSProblem<T>& problem)
        {
            // 0. Select an unassigned variable; if empty a solution has been found
            if ((int)problem.getUnassignedVariables().size() == 0)
                return true;

            Variable variable = problem.getUnassignedVariables()[0];
            Domain<T> domain = problem.getDomain(variable);

            //TODO: check var e dom

            // 1. Try each domain value until it's valid
            for (const T& value : domain.get_values())
            {
                // 2. Check current problem assignment
                if (isValidAssignment(problem, variable, value))
                {
                    // 3. Value is valid for variable, assign it and go deeper
                    problem.assignValue(variable, value);
                    if (backtracking(problem))
                        return true;
                    // 4. No solution found with that value, try another value
                    problem.unassignValue(variable);
                }
            }

            return false; // No solution found
        }

        bool backtracking_with_euristics(CSProblem<T>& problem, int varIndex=0){return false;};
        // TODO (other 5 strategies)


    };

};

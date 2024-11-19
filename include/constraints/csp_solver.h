#pragma once

#include <vector>
#include <algorithm>
#include <unordered_map>
#include <chrono>

#include <constraints/csp_problem.h>
#include <constraints/csp_binary_problem.h>
#include <constraints/utils/csp_domain.h>

namespace CSP
{

    template <typename T>
    class CSPSolver
    {

    public:
        enum class CSPSrategy
        {
            Backtracking,
            BacktrackingEuristics,
            ForwardChecking,
            ArcConsistency //(AC-3)
            // Local Search con Propagazione di Vincoli (Min-Conflicts)
            // Algoritmo di Ricerca con Backjumping
            // Tecniche di Propagazione di Vincoli Avanzate (e.g., Constraint Propagation)
        };


        CSPSolver(CSPSrategy s):_strategy(s), _enable_chrono(false){};
        ~CSPSolver(){};

        bool solve(CSProblem<T>& problem)
        {
            bool problem_solved = false;

            if(_enable_chrono) _start = std::chrono::high_resolution_clock::now();

            switch (_strategy)
            {
                case CSPSrategy::Backtracking:
                    problem_solved = backtracking(problem);
                    break;
                case CSPSrategy::ForwardChecking:
                    problem_solved = backtracking_forward_checking(problem);
                    break;
                case CSPSrategy::BacktrackingEuristics:
                    problem_solved = backtracking_with_euristics(problem);
                    break;
                case CSPSrategy::ArcConsistency:
                    problem_solved = arc_consistency((CSPBinaryProblem<T>&)problem);
                    break;
                default:
                    problem_solved = false;
                    break;
            }
            
            if(_enable_chrono) 
            {
                _end = std::chrono::high_resolution_clock::now();
                int duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(_end - _start).count();
                std::cout << "Execution time: " << duration_ms << " ms" << std::endl;
            }
            problem.set_solved(problem_solved);
            return problem_solved;
        }

        void _enableChrono(bool enabled=true) {_enable_chrono = enabled;}

    private: 
        CSPSrategy _strategy;

        bool _enable_chrono;
        std::chrono::high_resolution_clock::time_point _start, _end;

        // Utility solver methods

        // Remove value from incompatible domains
        std::unordered_map<Variable, Domain<T>> forward_check(CSProblem<T>& problem, Variable v, T value)
        {
            std::unordered_map<Variable, Domain<T>> original_domains = problem.getDomainsCopy();
            // 1. For each problem constraint
            for (Constraint<T> c: problem.getConstraints())
            {
                // 2. If variable v is involved on constraint
                if(!c.has_variable(v)) continue;
                // 3. Remove value from domain of other involved variables if c is not satisfied
                std::unordered_map<Variable, T> currentState = problem.getCurrentState();
                for(auto cv: c.getConstraintVariables())
                {
                    if (cv !=v) 
                    {
                        // 4. Try assign remaining value of cv domain and check constraint
                        Domain<T>& domain = problem.getDomain(cv);
                        for (Constraint<T> c_: problem.getConstraints())
                        {
                            if(!c_.has_variable(cv)) continue;

                            for (const T& cv_value : domain.get_values())
                            {
                                std::unordered_map<Variable, T> singleAssignment = {{cv, cv_value}};
                                Assignment assignment = singleAssignment;
                                //Fill the assignment with missing variables
                                for(auto cvs: c_.getConstraintVariables())
                                {
                                    // If the value is not assigned yet, consider as a valid assignment
                                    if(!currentState.contains(cvs)) continue;
                                    if(!assignment.getVariables().contains(cvs)) assignment.assign(cvs, currentState[cvs]);
                                }
                                // Check if the assignment is satisfied
                                if(!c_.is_satisfied(assignment)) 
                                    problem.getDomain(cv).remove(cv_value);
                            }
                        }

                    }
                }
            }

            return original_domains;
        }

        void revert_forward_check(CSProblem<T>& problem, std::unordered_map<Variable, Domain<T>> domains)
        {
            problem.setDomains(domains);
        }


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

        bool backtracking_forward_checking(CSProblem<T>& problem)        
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
                    // Forward-Checking step:
                    auto original_domains = forward_check(problem, variable, value);
                    // 4. Remove value from incompatibles domains of problem variables
                    if (backtracking_forward_checking(problem))
                        return true;
                    // 5. Revert forward check
                    revert_forward_check(problem, original_domains);
                    // 6. No solution found with that value, try another value
                    problem.unassignValue(variable);
                }
            }

            return false; // No solution found
        }

        bool backtracking_with_euristics(CSProblem<T>& problem){return false;};

        bool arc_consistency(CSPBinaryProblem<T>& problem){

            // 0. Retrive queue of arcs constraints
            std::queue<Constraint<T>> queue = problem.get_queue();

            while (!queue.empty()) 
            {
                // 1. Take the first arc (X,Y) and check the of X respect of Y
                Constraint<T> arc = queue.front();
                queue.pop();

                Variable X = arc.getConstraintVariables()[0];
                Variable Y = arc.getConstraintVariables()[1];

                Domain<T>& Dx = problem.getDomain(X);
                Domain<T>& Dy = problem.getDomain(Y);

                bool changed = false;

                for(auto Di: Dx.get_values())
                {
                    // 1.1 For each Di in D(X) check if C(Di, Dj) for each Dj in D(Y)
                    bool consistent = false;
                    for(auto Dj: Dy.get_values())
                    {
                        Assignment<T> assignment;
                        assignment.assign(X,Di);
                        assignment.assign(Y,Dj);
                        if(arc.is_satisfied(assignment))
                        {
                            consistent = true;
                            break;
                        }
                    }
                    // If no remove Di in D(X)
                    if(!consistent) 
                    {
                        Dx.remove(Di);   
                        changed = true;
                    }             
                    // Otherwise check next D(X) value
                }

                // If Dx is empty, problem has no solution
                if(Dx.get_values().empty()) return false;
                
                // If the domain has been reduced, add again each arc to queue to analyze them 
                if(changed)
                {
                    for(auto neighbor: problem.get_neighbors(X))
                    {
                        if(neighbor != Y) problem.addConstraint(arc.duplicate({neighbor,X}));
                    }
                }
            }

            for(auto v: problem.getVariables())
            {
                Domain<T>& domain_v = problem.getDomain(v);
                if(domain_v.get_values().empty()) return false;
                // Consider the only value of domain as valid (> 1 if more suitable solutions)
                problem.assignValue(v, domain_v.get_values()[0]);
            }

            return true;
        };


    };

};

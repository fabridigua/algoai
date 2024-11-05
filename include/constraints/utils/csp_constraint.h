#pragma once

#include <vector>
#include <functional>
#include <unordered_map>

#include "csp_variable.h"

namespace CSP
{
    template <typename T>
    class Assignment
    {
    private:
        std::unordered_map<Variable, T> _assignment;
    public:
        Assignment(std::unordered_map<Variable, T> assignment):_assignment(std::move(assignment)){};
        ~Assignment() = default;

        const std::unordered_map<Variable, T>& getVariables() const {return _assignment;};
    };
    
    template<typename T>
    class Constraint
    {
    private:
        std::vector<Variable> _variables;
        std::function<bool(std::vector<T>)> _rule;
    public:    
        Constraint(const std::vector<Variable>& variables, std::function<bool(std::vector<T>)> rule):_variables(variables), _rule(rule){};
        ~Constraint() = default;

        // Verify if a constraint is satisfied with a certain assignment of values
        bool is_satisfied(Assignment<T> assignment) 
        {
            std::vector<T> assignment_values;

            // 1. Get values of the assignment
            for(const Variable& v: _variables)
            {
                // If a varible is not yet assigned, don't evaluate the rule
                if(assignment.getVariables().find(v) == assignment.getVariables().end())
                    return true;

                assignment_values.push_back(assignment.getVariables().at(v));
            }
            
            // 2. Check the rule on assigned values
            return _rule(assignment_values);

        };
    };

    
};
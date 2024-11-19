#pragma once

#include <vector>
#include <functional>
#include <unordered_map>
#include <optional>

#include <constraints/utils/csp_variable.h>

namespace CSP
{
    template <typename T>
    class Assignment
    {
    private:
        std::unordered_map<Variable, T> _assignment;
    public:
        Assignment() = default;
        Assignment(std::unordered_map<Variable, T> assignment):_assignment(std::move(assignment)){};
        ~Assignment() = default;

        const std::unordered_map<Variable, T>& getVariables() const {return _assignment;};

        // Add a variable to the assignment
        void assign(const Variable& var, const T& value) { _assignment[var] = value; }

        // Remove a variable from the assignment
        void unassign(const Variable& var) { _assignment.erase(var); }

        bool is_assigned(const Variable& var) const { return _assignment.find(var) != _assignment.end(); }

        // Get the assigned value
        std::optional<T> get_value(const Variable& var) const {
            if (is_assigned(var)) 
                return _assignment.at(var);
            return std::nullopt;
        }
    };
    
    template<typename T>
    class Constraint
    {
    private:
        std::vector<Variable> _variables;
        std::function<bool(std::vector<T>)> _rule;
    public:    
        Constraint(std::vector<Variable> variables, std::function<bool(std::vector<T>)> rule):_variables(variables), _rule(rule){};
        ~Constraint() = default;

        std::vector<Variable> getConstraintVariables() const { return _variables;}

        Constraint duplicate(std::vector<Variable> variables) {return Constraint<T>(variables, _rule);} 
        
        bool has_variable(const Variable& v) {return std::find(_variables.begin(), _variables.end(), v) != _variables.end();}

        // Verify if a constraint is satisfied with a certain assignment of values
        bool is_satisfied(const Assignment<T>& assignment) 
        {
            std::vector<T> assignment_values;

            // 1. Get values of the assignment
            for(const Variable& v: _variables)
            {
                // If a varible is not yet assigned, don't evaluate the rule
                if(!assignment.getVariables().contains(v))
                    continue; // TODO: => duplicate of check in isValidAssignment?

                assignment_values.push_back(assignment.getVariables().at(v));
            }
            
            // 2. Check the rule on assigned values
            return _rule(assignment_values);
        };
    };

    
};
#pragma once

#include <vector>
#include <unordered_map>
#include <optional>

#include <constraints/utils/csp_variable.h>
#include <constraints/utils/csp_domain.h>
#include <constraints/utils/csp_constraint.h>

namespace CSP
{
    template <typename T>
    class CSProblem
    {
    public:
        CSProblem():_solved(false){};
        ~CSProblem(){};

        void addVariable(Variable v, Domain<T> d) 
        { 
            _variables.push_back(v);
            _domains[v] = std::move(d);
        };
        
        void addConstraint(Constraint<T> c) {_constraints.push_back(c);}

        const Domain<T>& getDomain(const Variable& v) const
        {
            auto it = _domains.find(v);
            // TODO check if not present
            
            return it->second;
        }

        const std::vector<Constraint<T>>& getConstraints() const {return _constraints;};

        void assignValue(Variable variable, T value){ _state[variable] = value;}
        void unassignValue(Variable variable){ _state.erase(variable);}

        bool getSolution(std::unordered_map<Variable, T>* solution){
            // if(!_solved) return false;
            *solution = _state;
            return true;
        }

        std::unordered_map<Variable, T> getCurrentState() const { return _state; }

        const std::vector<Variable> getVariables() const {return _variables;}

        std::vector<Variable> getUnassignedVariables() const {
            std::vector<Variable> unassigned;
            for(auto v: _variables)
            {
                if(!_state.contains(v))
                    unassigned.push_back(v);
            }
            return unassigned;
        }

    private:
        std::vector<Variable> _variables;
        std::unordered_map<Variable, Domain<T>> _domains;
        std::vector<Constraint<T>> _constraints;
        
        std::unordered_map<Variable, T> _state;

        bool _solved;
    };

    

};

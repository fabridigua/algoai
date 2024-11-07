#pragma once

#include <vector>
#include <unordered_map>

#include "csp_variable.h"
#include "csp_domain.h"
#include "csp_constraint.h"

namespace CSP
{
    template <typename T>
    class CSProblem
    {
    public:
        CSProblem(){};
        ~CSProblem(){};

        void addVariable(Variable v, Domain d) 
        { 
            _variables.push_back(v);
            _domains[v] = d;
        };
        
        void addConstraint(Constraint<T> c) {_constraints.push_back(c);}


    private:
        std::vector<Variable> _variables;
        std::unordered_map<Variable, Domain<T>> _domains;
        std::vector<Constraint<T>> _constraints;

    };

    

};

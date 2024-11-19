#pragma once

#include <queue>
#include <unordered_map>

#include <constraints/csp_problem.h>

namespace CSP
{
    template <typename T>
    class CSPBinaryProblem: public CSProblem<T>
    {  
    public:
        CSPBinaryProblem(){};
        ~CSPBinaryProblem(){};
        
        void addConstraint(Constraint<T> c) override {
            if (c.getConstraintVariables().size() != 2) return;

            CSProblem<T>::_constraints.push_back(c);
            
            // std::pair<Variable, Variable> vPair = {c.getConstraintVariables()[0], c.getConstraintVariables()[1]};
            _arcs.push(c);
            _neighbors[c.getConstraintVariables()[0]].push_back(c.getConstraintVariables()[1]);
            _neighbors[c.getConstraintVariables()[1]].push_back(c.getConstraintVariables()[0]);
        }

        std::queue<Constraint<T>>& get_queue() { return _arcs; }

        std::vector<Variable>& get_neighbors(Variable v) {return _neighbors[v];}

    private:
        std::queue<Constraint<T>> _arcs;

        std::unordered_map<Variable,std::vector<Variable>> _neighbors; // TODO: better a unordered set instead a vector?
    };

    

};

#pragma once

#include <vector>
#include <algorithm>

namespace CSP
{
    template <typename T>
    class Domain
    {   
        public:
            Domain(int& id, std::vector<T> range):variable_id(id),_values(std::move(range)){};
            Domain(std::vector);

            void remove(const& T value) 
            {
                _values.erase(std::remove(_values.begin(),_values.end(), value), _values.end());
            }

            const std::vector<T>& get_values() const {return _values;}
            const int& get_variable_id() const {return variable_id;}
        private:
            int& variable_id;
            std::vector<T> _values;


    };
    
};
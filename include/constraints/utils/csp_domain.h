#pragma once

#include <vector>
#include <algorithm>

namespace CSP
{
    template <typename T>
    class Domain
    {
        public:
            Domain() = default;
            Domain(std::vector<T> range):_values(std::move(range)){};

            void add(T value)
            {
                _values.push_back(value);
            }

            void remove(T value)
            {
                _values.erase(std::remove(_values.begin(),_values.end(), value), _values.end());
            }

            const std::vector<T>& get_values() const {return _values;}

            void set(std::vector<T> range){_values=range;}
            // int get_variable_id() const {return variable_id;}
        private:
            // TODO: use std::set to improve erase operation ((O(log n) fpr set o O(1) for unordered_set))
            std::vector<T> _values;

    };

};
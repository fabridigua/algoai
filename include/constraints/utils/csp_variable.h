#pragma once

#include "csp_domain.h"

namespace CSP
{
  struct Variable
  {
    Variable(int id):_id(id){};
    int _id;
    const int& get_id() const { return _id;};

    bool operator==(const Variable& other) const { return _id == other._id; }
    
  };
    
};

namespace std {
  template<>
  struct hash<CSP::Variable> {
      std::size_t operator()(const CSP::Variable& v) const {
          return std::hash<int>()(v._id);
      }
  };
}
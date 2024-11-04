#pragma once

namespace CSP
{
  struct Variable
  {
    Variable(int id):_id(id){};
    int _id;
    const int& get_id() const { return _id;};
  };
    
};
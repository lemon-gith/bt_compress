#ifndef BOOLTREE_HPP
#define BOOLTREE_HPP

#include <string>
#include <vector>
#include "bnode.hpp"


// class defining the final Boolean Tree structure
// this was provided to us as part of the project, not to be modified
class BoolTree {
private:
  BNode* t;
  // not provided, is just useful for me
  int fval_length;

public:
  explicit BoolTree(const std::vector<std::string>& fvalues, int length);

  std::string eval(const std::string& s);

  int n_nodes();

  ~BoolTree();

  // technically not allowed, just for debugging
  BNode* showRoot();

  int get_fval_length();
};

#endif
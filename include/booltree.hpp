#ifndef BOOLTREE_H
#define BOOLTREE_H

#include <string>
#include <vector>
#include "builder.h"


class BoolTree {
private:
  BNode* t;

public:
  explicit BoolTree(const std::vector<std::string>& fvalues);

  std::string eval(const std::string& s);

  int n_nodes();

  ~BoolTree();

  //technically not allowed, just for debugging
  BNode* showRoot();
};

#endif
#include "booltree.hpp"


explicit BoolTree::BoolTree(const std::vector<std::string>& fvalues) {
  t = build_bt(fvalues);
}

std::string BoolTree::eval(const std::string& s) {
  return eval_bt(t, s);
}

int BoolTree::n_nodes() {
  return n_nodes_bt(t);
}

BoolTree::~BoolTree() {
  deallocate_bt(t);
}

//technically not allowed, just for debugging
BNode* BoolTree::showRoot() {
  return t;
}

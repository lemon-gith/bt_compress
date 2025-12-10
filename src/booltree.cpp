#include "booltree.hpp"
#include "builder.hpp"
#include "bnode.hpp"


// these definitions were mostly provided to us as part of the project
// some minor modifications have been made, to accommodate my API changes

BoolTree::BoolTree(const std::vector<std::string>& fvalues, int length) {
  fval_length = length;
  t = build_bt(fvalues);
}

// returns result of evaluating the bool tree on input string s,
// or a string beginning with "Err:" if the input string is of incorrect length
std::string BoolTree::eval(const std::string& s) {
  if ((int)s.length() != fval_length) {
    return "Err: input string length does not match fval length";
  }
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

int BoolTree::get_fval_length(){
  return fval_length;
}
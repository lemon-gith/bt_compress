#ifndef BNODE_HPP
#define BNODE_HPP

#include <string>


// this is the code that was provided to us for the project
#pragma region Provided code
// public struct, defining the basic Binary Node (*l, val, *r)
struct BNode {
  std::string val;  // "xn" (e.g. x3 or x1 etc) in non-leaf nodes
  // "0" or "1" in leaf nodes
  BNode* left{};    // the 0 branch
  BNode* right{};  // the 1 branch
  // (as usual leaf nodes will have both left and right pointing to NULL)
};

int n_nodes_bt(BNode* t);

// this function converts e.g. std::string "x3" to int 2; don't alter
int label_to_idx(const std::string& label);

// don't alter
std::string eval_bt(BNode* bt, const std::string& input);
#pragma endregion


// constructs a new BNode using provided data, returning a pointer to that node
BNode* nodeCons(
  const std::string& data, BNode* l_ptr = nullptr, BNode* r_ptr = nullptr
);

#endif
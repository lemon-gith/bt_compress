#ifndef BUILDER_HPP
#define BUILDER_HPP

// needed in some struct definitions
#include <vector>
#include <string>
#include "bnode.hpp"


// the public main function for BT construction, defines high-level process
BNode* build_bt(const std::vector<std::string>& fvalues);
// ^ we were not allowed to modify this function signature

// the internal main function for BT building, using pre-simplified values
BNode* constructBT(Builder &Cob);

// builds 0 nodes at all open nullptrs in bt (unless there's a 1 there)
void add_0s (BNode* curr_root);

// given the root node, deallocates the entire binary tree
void deallocate_bt(BNode* &bt_root);

// ------------------- Builder class signatures -------------------

// private struct, for builder to use to split lists
struct SplitVals;

// TODO: idk, what is this?
class Builder {
private:
  std::vector<int> mSeq{};
  std::vector<std::string> mBvals;

  int mLength;

public:
  std::vector<int> createCocos(std::vector<std::string> &vals) const;

  void orderAttributes(std::vector<int> &seq, std::vector<std::string> &vals, std::vector<int> &order);

  static bool ordered(const std::vector<int> &cocos);

  void orderCol(std::vector<int> &seq, std::vector<std::string> &vals);

  explicit Builder(std::vector<std::string> svals);

  Builder(std::vector<int> sequence, std::vector<std::string>& bvals);

  bool straightBuild();

  BNode* branchCons();

  SplitVals pathSplit();

  void delete_();
};

#endif
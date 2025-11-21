#ifndef BUILDER_HPP
#define BUILDER_HPP

// needed in some struct definitions
#include <vector>
#include <string>


#pragma region Provided code
// public struct, defining the basic Binary Node (*l, val, *r)
struct BNode{
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

void deallocate_bt(BNode* &bt_root);

// constructs a new BNode using provided data, returning a pointer to that node
BNode* nodeCons(
  const std::string& data, BNode* l_ptr = nullptr, BNode* r_ptr = nullptr
);

// private struct, for builder to use to split lists
struct SplitVals;

//just a simplified and specialised ver. of fill() in <algorithms>
void empty_fill(std::vector<std::string> &vec, int count);


class Builder{
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

BNode* constructBT(Builder &Cob);


std::vector<std::pair<int, int>> findDuplicates(
  const std::vector<std::string> &rvals
);

int maxSecond(const std::vector<std::pair<int, int>> &dups);

void collapseSimilar(
  std::vector<std::string> &rvals, std::vector<char> &omitted_col,
  std::vector<std::pair<int, int>> &duplicates
);

std::vector<std::string> simplifyFvals(std::vector<std::string> fvals);


// builds 0 nodes at all open nullptrs in bt (unless there's a 1 there)
void add_0s (BNode* curr_root);


void removeDuplicates(std::vector<std::string> &vec);

// function to sanitise the input data
std::vector<std::string> border_control(std::vector<std::string> fvals);

// don't alter input parameters
BNode* build_bt(const std::vector<std::string>& fvalues);


#endif
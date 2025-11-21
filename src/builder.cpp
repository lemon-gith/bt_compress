#include <cmath>
#include <string>
// #include <vector>  // in header file
#include <numeric>
#include <stdexcept>
#include <unordered_set>
#include "builder.hpp"
#include "utils.hpp"
#include "simplification.hpp"


BNode* build_bt(const std::vector<std::string>& fvalues){
  if (fvalues.empty()){
    return nullptr;
  }

  std::vector<std::string> sanitised_fvals = border_control(fvalues);

  Builder Bob(simplifyFvals(sanitised_fvals));

  BNode* bt_root = constructBT(Bob);
  add_0s(bt_root);

  return bt_root;
}

BNode* constructBT(Builder &Cob) {
  if (Cob.straightBuild()){
    return Cob.branchCons();
  }

  SplitVals data = Cob.pathSplit();

  Cob.delete_();

  // TODO: Check if this 'malfunction' is now possible with path split allowing splitting on 'X'
  if (data.l_vals.empty()){  // for both l_vals and r_vals, would take a simplifyFvals() malfunction
    Builder Rob(data.seq_vals, data.r_vals);
    return nodeCons(data.curr_node_val, nullptr, constructBT(Rob));
  }
  else if (data.r_vals.empty()){
    Builder Lob(data.seq_vals, data.l_vals);
    return nodeCons(data.curr_node_val, constructBT(Lob), nullptr);
  }
  else{
    Builder Lob(data.seq_vals, data.l_vals);
    Builder Rob(data.seq_vals, data.r_vals);

    return nodeCons(data.curr_node_val, constructBT(Lob), constructBT(Rob));
  }
}

void add_0s (BNode* curr_root) {
  if (curr_root->val == "1"){
    return;
  }

  if (curr_root->left == nullptr){
    curr_root->left = nodeCons("0");
  }
  else{
    add_0s(curr_root->left);
  }

  if (curr_root->right == nullptr){
    curr_root->right = nodeCons("0");
  }
  else{
    add_0s(curr_root->right);
  }
}

void deallocate_bt(BNode* &bt_root) {
  if (bt_root->left != nullptr) {
    deallocate_bt(bt_root->left);
  }
  if (bt_root->right != nullptr) {
    deallocate_bt(bt_root->right);
  }
  delete bt_root;
}

// ------------------- Builder class definitions -------------------

struct SplitVals {
  std::string curr_node_val{};

  std::vector<int> seq_vals{};
  std::vector<std::string> l_vals{};
  std::vector<std::string> r_vals{};
};

std::vector<int> Builder::createCocos(std::vector<std::string> &vals) const {
  std::vector<int> cocos{}; // Correlation Coefficients
  cocos.reserve(mLength);
  int xCounter;

  for (int col = 0; col < mLength; col++) {
    xCounter = 0;

    for (std::string &branch : vals) {
      if (branch[col] == 'X') {
        xCounter--;
      }
    }

    if (xCounter) {
      cocos.push_back(xCounter);
    }
    else{
      bool diff = 0;

      for (int branch = 1; (branch < vals.size()) && (!diff); branch++) {
        if (vals[branch - 1][col] != vals[branch][col]) {
          diff = 1;
        }
      }

      cocos.push_back(diff);
    }
  }
  return cocos;
}

void Builder::orderAttributes(
  std::vector<int> &seq,
  std::vector<std::string> &vals,
  std::vector<int> &order
) {
  mSeq.reserve(mLength);
  empty_fill(mBvals, int(vals.size()));

  for (const int &idx : order) {
    mSeq.push_back(seq[idx]);
    for (int i = 0; i < vals.size(); i++) {
      mBvals[i].append(std::string(1, vals[i][idx]));
    }
  }
}

bool Builder::ordered(const std::vector<int> &cocos) {
  int curr_coco_val = 1;
  for (const int &coco : cocos) {
    if (coco > curr_coco_val) {
      return false;
    }
    else if (coco < curr_coco_val) {
      curr_coco_val = coco;
    }
  }
  return true;
}

void Builder::orderCol(std::vector<int> &seq, std::vector<std::string> &vals) {
  const std::vector<int> cocos = createCocos(vals);

  if (ordered(cocos)) {
    mSeq = std::move(seq);
    mBvals = std::move(vals);
  }
  else{
    // TODO: can a reservation initialisation be performed here?
    std::vector<int> new_order{};
    new_order.reserve(cocos.size());

    for (int coco_val = 1; new_order.size() < cocos.size(); coco_val--) {
      for (int i = 0; i < cocos.size(); i++) {
        if (cocos[i] == coco_val) {
          new_order.push_back(i);
        }
      }
    }
    orderAttributes(seq, vals, new_order);
  }
}

explicit Builder::Builder(std::vector<std::string> svals) {
  mLength = int(svals[0].size());

  std::vector<int> sequence(mLength);
  std::iota(sequence.begin(), sequence.end(), 1);

  orderCol(sequence, svals);
}

Builder::Builder(std::vector<int> sequence, std::vector<std::string>& bvals) {
  mLength = int(sequence.size());

  orderCol(sequence, bvals);
}

// TODO: I feel like this should be renamed
bool Builder::straightBuild() {
  return mBvals.size() == 1;
}

BNode* Builder::branchCons() {
  if (mBvals.size() != 1) {  // Just felt like throwing an error message here
    throw std::length_error("branchCons() should only deal with one row mBvals");
  }

  std::string branch_vals = mBvals[0];

  int starting_idx = 0;
  for (; (starting_idx < mLength) && (branch_vals[starting_idx] != 'X');) {
    starting_idx++;
  }
  starting_idx--;

  BNode* stored_ptr = nodeCons("1");
  for (int i = starting_idx; i >= 0; i--) {
    if (branch_vals[i] == '0') {
      stored_ptr = nodeCons("x" + std::to_string(mSeq[i]), stored_ptr, nullptr);
    }
    else if (branch_vals[i] == '1') {
      stored_ptr = nodeCons("x" + std::to_string(mSeq[i]),nullptr, stored_ptr);
    }
    else{
      throw std::invalid_argument("Somehow, sth not '0' or '1' has made it into branchCons() main loop");
    }
  }
  this->delete_();
  return stored_ptr;
}

SplitVals Builder::pathSplit() {
  SplitVals data;

  data.curr_node_val = "x" + std::to_string(mSeq[0]);

  for (int i = 1; i < mLength; i++) {
    data.seq_vals.push_back(mSeq[i]);
  }

  std::string trolley;
  for (std::string &branch : mBvals) {
    // drop first character, that's just to decide where to send the rest
    trolley = branch.substr(1, branch.size()-1);

    // if it's an 'X', I have to split it up,
    // since succeeding values clearly aren't 'X', else it wouldn't be in here

    if ((branch[0] == '0') || (branch[0] == 'X')) {
      data.l_vals.push_back(trolley);
    }
    if ((branch[0] == '1') || (branch[0] == 'X')) {
      data.r_vals.push_back(trolley);
    }

    if ((branch[0] != '0') && (branch[0] != '1') && (branch[0] != 'X')) {
      throw std::logic_error("attempted to split: unexpected value at branch[0] for " + branch);
    }
  }
  return data;
}

// TODO: what exactly does this do and how necessary is it
void Builder::delete_() {
  mSeq.clear();
  mSeq.shrink_to_fit();
  mBvals.clear();
  mBvals.shrink_to_fit();
  mLength = 0;
}

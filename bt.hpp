#include <vector>
#include <string>
#include <cmath>
#include <numeric>
#include <unordered_set>
#include <stdexcept>

//Restriction: can use any and only headers from the C++ standard library, except the algorithm library

//#pragma region Provided code
struct BNode{
  std::string val;  // "xn" (e.g. x3 or x1 etc) in non-leaf nodes
  // "0" or "1" in leaf nodes
  BNode* left{};    // the 0 branch
  BNode* right{};  // the 1 branch
  // (as usual leaf nodes will have both left and right pointing to NULL)
};

int n_nodes_bt(BNode* t){
  if(t == nullptr){
    return 0;
  }
  else{
    return 1 + n_nodes_bt(t->left) + n_nodes_bt(t->right);
  }
}

// this function converts e.g. std::string "x3" to int 2; don't alter
int label_to_idx(const std::string& label){

  std::string out;

  for(int i = 1; i < label.size(); i++){
    out.push_back(label[i]);
  }

  return std::stoi(out) - 1;
}

// don't alter
std::string eval_bt(BNode* bt, const std::string& input){

  if( (bt->left == nullptr) && (bt->right == nullptr) ){
    return bt->val;
  }
  else{
    int idx = label_to_idx(bt->val);
    std::string input_idx;
    input_idx.push_back(input[idx]);

    if(input_idx == "0"){
      return eval_bt(bt->left, input);
    }
    else{
      return eval_bt(bt->right, input);
    }
  }
}
//#pragma endregion Provided code

void deal_bt(BNode* &bt_root){
  if (bt_root->left != nullptr) {
    deal_bt(bt_root->left);
  }
  if (bt_root->right != nullptr) {
    deal_bt(bt_root->right);
  }
  delete bt_root;
}

BNode* nodeCons(const std::string& data, BNode* l_ptr = nullptr, BNode* r_ptr = nullptr){
  BNode* new_root;
  new_root = new BNode;
  new_root->left = l_ptr;
  new_root->val = data;
  new_root->right = r_ptr;
  return new_root;
}

struct SplitVals{
  std::string curr_node_val{};

  std::vector<int> seq_vals{};
  std::vector<std::string> l_vals{};
  std::vector<std::string> r_vals{};
};

//just a simplified and specialised ver. of fill() in <algorithms>
void empty_fill(std::vector<std::string> &vec, int count){
  for (int i = 0; i < count; i++){
    vec.push_back("");
  }
}
class Builder{
private:
  std::vector<int> mSeq{};
  std::vector<std::string> mBvals;

  int mLength;

public:
  std::vector<int> createCocos(std::vector<std::string> &vals) const{
    std::vector<int> cocos{}; // Correlation Coefficients
    cocos.reserve(mLength);
    int xCounter;

    for (int col = 0; col < mLength; col++){
      xCounter = 0;

      for (std::string &branch : vals){
        if (branch[col] == 'X'){
          xCounter--;
        }
      }

      if (xCounter){
        cocos.push_back(xCounter);
      }
      else{
        bool diff = 0;

        for (int branch = 1; (branch < vals.size()) && (!diff); branch++){
          if (vals[branch - 1][col] != vals[branch][col]){
            diff = 1;
          }
        }

        cocos.push_back(diff);
      }
    }
    return cocos;
  }
  void orderAttributes(std::vector<int> &seq, std::vector<std::string> &vals, std::vector<int> &order){
    mSeq.reserve(mLength);
    empty_fill(mBvals, int(vals.size()));

    for (const int &idx : order){
      mSeq.push_back(seq[idx]);
      for (int i = 0; i < vals.size(); i++){
        mBvals[i].append(std::string(1, vals[i][idx]));
      }
    }
  }
  static bool ordered(const std::vector<int> &cocos){
    int curr_coco_val = 1;
    for (const int &coco : cocos){
      if (coco > curr_coco_val){
        return false;
      }
      else if (coco < curr_coco_val){
        curr_coco_val = coco;
      }
    }
    return true;
  }
  void orderCol(std::vector<int> &seq, std::vector<std::string> &vals){
    const std::vector<int> cocos = createCocos(vals);

    if (ordered(cocos)){
      mSeq = std::move(seq);
      mBvals = std::move(vals);
    }
    else{
      // TODO: can a reservation initialisation be performed here?
      std::vector<int> new_order{};
      new_order.reserve(cocos.size());

      for (int coco_val = 1; new_order.size() < cocos.size(); coco_val--){
        for (int i = 0; i < cocos.size(); i++){
          if (cocos[i] == coco_val){
            new_order.push_back(i);
          }
        }
      }
      orderAttributes(seq, vals, new_order);
    }
  }
  explicit Builder(std::vector<std::string> svals){
    mLength = int(svals[0].size());

    std::vector<int> sequence(mLength);
    std::iota(sequence.begin(), sequence.end(), 1);

    orderCol(sequence, svals);
  }

  Builder(std::vector<int> sequence, std::vector<std::string>& bvals){
    mLength = int(sequence.size());

    orderCol(sequence, bvals);
  }


  bool straightBuild(){
    return mBvals.size() == 1;
  }


  BNode* branchCons(){
    if (mBvals.size() != 1){  // Just felt like throwing an error message here
      throw std::length_error("branchCons() should only deal with one row mBvals");
    }

    std::string branch_vals = mBvals[0];

    int starting_idx = 0;
    for (; (starting_idx < mLength) && (branch_vals[starting_idx] != 'X');){
      starting_idx++;
    }
    starting_idx--;

    BNode* stored_ptr = nodeCons("1");
    for (int i = starting_idx; i >= 0; i--){
      if (branch_vals[i] == '0'){
        stored_ptr = nodeCons("x" + std::to_string(mSeq[i]), stored_ptr, nullptr);
      }
      else if (branch_vals[i] == '1'){
        stored_ptr = nodeCons("x" + std::to_string(mSeq[i]),nullptr, stored_ptr);
      }
      else{
        throw std::invalid_argument("Somehow, sth not '0' or '1' has made it into branchCons() main loop");
      }
    }
    this->delete_();
    return stored_ptr;
  }


  SplitVals pathSplit(){
    SplitVals data;

    data.curr_node_val = "x" + std::to_string(mSeq[0]);

    for (int i = 1; i < mLength; i++){
      data.seq_vals.push_back(mSeq[i]);
    }

    std::string trolley;
    for (std::string &branch : mBvals){
      // drop first character, that's just to decide where to send the rest
      trolley = branch.substr(1, branch.size()-1);

      // if it's an 'X', I have to split it up,
      // since succeeding values clearly aren't 'X', else it wouldn't be in here

      if ((branch[0] == '0') || (branch[0] == 'X')){
        data.l_vals.push_back(trolley);
      }
      if ((branch[0] == '1') || (branch[0] == 'X')){
        data.r_vals.push_back(trolley);
      }

      if ((branch[0] != '0') && (branch[0] != '1') && (branch[0] != 'X')){
        throw std::logic_error("attempted to split: unexpected value at branch[0] for " + branch);
      }
    }
    return data;
  }

  // TODO: what exactly does this do and how necessary is it
  void delete_(){
    mSeq.clear();
    mSeq.shrink_to_fit();
    mBvals.clear();
    mBvals.shrink_to_fit();
    mLength = 0;
  }
};

BNode* constructBT(Builder &Cob){
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


std::vector<std::pair<int, int>> findDuplicates(const std::vector<std::string> &rvals){
  std::vector<std::pair<int, int>> duplicates{};

  for (int comparator = 0; comparator < rvals.size(); comparator++){
    for (int comparand = comparator + 1; comparand < rvals.size(); comparand++){
      if (rvals[comparator] == rvals[comparand]){
        duplicates.push_back(std::pair<int, int>(comparator, comparand)); // in ascending order
      }
    }
  }
  return duplicates;
}
int maxSecond(const std::vector<std::pair<int, int>> &dups){
  if (dups.empty()){
    return -1;
  }
  else if (dups.size() == 1){
    return 0;
  }
  else if (dups.size() == 2){
    return dups[0].second > dups[1].second ? 0 : 1;
  }
  else{
    int max_val = dups[0].second;
    int max_idx = 0;

    for (int i = 1; i < dups.size(); i++){
      if (dups[i].second > max_val){
        max_val = dups[i].second;
        max_idx = i;
      }
    }
    return max_idx;
  }
}
void collapseSimilar(std::vector<std::string> &rvals,
                      std::vector<char> &omitted_col, 
                      std::vector<std::pair<int, int>> &duplicates){
  int dup1, dup2;

  for (int i = maxSecond(duplicates); i >= 0; i = maxSecond(duplicates)){
    dup1 = duplicates[i].first;
    dup2 = duplicates[i].second;

    omitted_col[dup1] = 'X';
    omitted_col.erase(omitted_col.begin() + dup2);
    rvals.erase(rvals.begin() + dup2);

    duplicates.erase(duplicates.begin() + i);
  }
}

std::vector<std::string> simplifyFvals(std::vector<std::string> fvals){
  // not removing duplicates, as shouldn't theoretically happen here
  std::vector<char> omitted_col{};
  omitted_col.reserve(fvals.size());
  std::vector<std::string> rvals{};
  rvals.reserve(fvals.size());
  std::vector<std::pair<int, int>> duplicates{};

  for (int i = 0; i < fvals[0].size(); i++){
    omitted_col.clear();

    for (std::string &fval : fvals){
      omitted_col.push_back(fval[i]);
      fval.erase(fval.begin() + i);
    }

    duplicates = findDuplicates(fvals);
    collapseSimilar(fvals, omitted_col, duplicates);

    for (int rep_idx = 0; rep_idx < omitted_col.size(); rep_idx++){
      fvals[rep_idx].insert(fvals[rep_idx].begin() + i, omitted_col[rep_idx]);
    }
  }
  return fvals;
}


// builds 0 nodes at all open nullptrs in bt (unless there's a 1 there)
void add_0s (BNode* curr_root){
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


void removeDuplicates(std::vector<std::string> &vec){
  std::unordered_set<std::string> unique_fvals(vec.begin(), vec.end());
  vec.clear();
  vec.reserve(unique_fvals.size());
  for (const std::string fval : unique_fvals){
    vec.push_back(fval);
  }
}

// function to sanitise the input data
 std::vector<std::string> border_control(std::vector<std::string> fvals){
  int length = int(fvals[0].size());

  for (const std::string &fval : fvals){
    if (fval.size() != length){
      // TODO: track max-length and zero-pad to longest, also give error output
      throw std::length_error("not all fvals are of the same length");
    }
    for (const char &val : fval){
      if ((val != '0') && (val != '1')){
        throw std::invalid_argument("input contains non-binary characters");
      }
    }
  }
  removeDuplicates(fvals);
  return fvals;
}

// don't alter input parameters
BNode* build_bt(const std::vector<std::string>& fvalues){
  if (fvalues.empty()){
    return nullptr;
  }

  std::vector<std::string> sanitised_fvals = border_control(fvalues);

  Builder Bob(simplifyFvals(fvalues));


  BNode* bt_root = constructBT(Bob);
  add_0s(bt_root);

  return bt_root;
}

class BoolTree{
public:
  explicit BoolTree(const std::vector<std::string>& fvalues){
    t = build_bt(fvalues);
  }

  std::string eval(const std::string& s){
    return eval_bt(t, s);
  }

  int n_nodes(){
    return n_nodes_bt(t);
  }

  ~BoolTree(){
    deal_bt(t);
  }

  //technically not allowed, just for debugging
  BNode* showRoot(){
    return t;
  }

private:
  BNode* t;
};
#include "bnode.hpp"


#pragma region Provided code
int n_nodes_bt(BNode* t) {
  if(t == nullptr) {
    return 0;
  }
  else{
    return 1 + n_nodes_bt(t->left) + n_nodes_bt(t->right);
  }
}

int label_to_idx(const std::string& label) {

  std::string out;

  // ensure label.size() < INT_MAX in simplification::border_control
  for(int i = 1; i < (int)label.size(); i++) {
    out.push_back(label[i]);
  }

  return std::stoi(out) - 1;
}

std::string eval_bt(BNode* bt, const std::string& input) {

  if( (bt->left == nullptr) && (bt->right == nullptr) ) {
    return bt->val;
  }
  else{
    int idx = label_to_idx(bt->val);
    std::string input_idx;
    input_idx.push_back(input[idx]);

    if(input_idx == "0") {
      return eval_bt(bt->left, input);
    }
    else{
      return eval_bt(bt->right, input);
    }
  }
}
#pragma endregion


BNode* nodeCons(const std::string& data, BNode* l_ptr, BNode* r_ptr ) {
  BNode* new_root;
  new_root = new BNode;
  new_root->left = l_ptr;
  new_root->val = data;
  new_root->right = r_ptr;
  return new_root;
}

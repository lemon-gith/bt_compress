#include "bt.hpp"
#include <iostream>
#include <fstream>
#include <format>
#include <cstdlib>


void rec_print_bt(BNode* root, int layer = 0){
  if (root == nullptr){
    return;
  }
  for (int i = 0; i < layer; i++){
    std::cout << "-\t";
  }
  std::cout << root->val << std::endl;
  layer++;
  rec_print_bt(root->left, layer);
  rec_print_bt(root->right, layer);
}

//remember to alter i < ? and {:0?b}, otherwise it won't work for other lengths of fvals
void full_bin_test(BNode* root, const int &length){
  std::vector<std::string> fvals;
  std::string fmt = "{:0";
  fmt.append(std::to_string(length));
  fmt.append("b}");
  for (int i = 0; i < pow(2, length); i++){
    if (std::stoi(eval_bt(root, std::vformat(fmt, std::make_format_args(i))))){
      fvals.push_back(std::vformat(fmt, std::make_format_args(i)));
    }
  }

  for (const std::string& fval : fvals){
    std::cout << fval << std::endl;
  }
}

//region randomising fvalues
std::vector<std::string> ranFvals(const int &length, const int &fval_num){
  std::vector<std::string> ran_vals{};

  std::string fmt = "{:0";
  fmt.append(std::to_string(length));
  fmt.append("b}");

  int val;
  for (int i = 0; i < fval_num; i++){
    val = rand() % int(pow(2, length)) + 1;
    ran_vals.push_back(std::vformat(fmt, std::make_format_args(val)));
  }
  return ran_vals;
}

bool inFvals(const std::string &test, std::vector<std::string> &fvals){
  for (std::string &fval : fvals){
    if (fval == test){
      return true;
    }
  }
  return false;
}
std::string ranNonFval(const int &length, std::vector<std::string> &fvals){
  std::string fmt = "{:0";
  fmt.append(std::to_string(length));
  fmt.append("b}");

  int val = rand() % int(pow(2, length)) + 1;
  std::string test_str = std::vformat(fmt, std::make_format_args(val));

  while (inFvals(test_str, fvals)){
    val = rand() % int(pow(2, length)) + 1;
    test_str = std::vformat(fmt, std::make_format_args(val));
  }
  return test_str;
}
//endregion


int main(){
  //user-defined values
  bool read_from_txt = false;
  // ?
  int length = 15;
  int no_of_fvals = 1000;
  // :
  std::string txt_location = "values.txt";
  // ;
  bool print_tree = false;
  bool run_bin_test = false;

  // automated main program
  std::vector<std::string> values;
  if (read_from_txt){
    std::ifstream infile;
    infile.open(txt_location);

    if (!infile.is_open()){
      std::cout << "error opening file" << std::endl;
      return EXIT_FAILURE;
    }

    std::string tmp;

    while (infile >> tmp){
      // just a minor modification to allow for comments
      if (tmp[0] != '#'){
        values.push_back(tmp);
      }
    }

    infile.close();
  }
  else{
    values = ranFvals(length, no_of_fvals);
    for (const std::string &val : values){
      std::cout << val << std::endl;
    }
  }

  std::cout << "max: " << (pow(2, values[0].length() + 1) - 1) << " nodes :0" << std::endl;

  BoolTree ft2(values);
  // this corresponds to the f(x1, x2, x3) example shown above

  std::cout << "constructed with " << ft2.n_nodes() << " nodes :)" << std::endl;

  if (read_from_txt){
    std::cout << ft2.eval("0011001010") << std::endl;
    // this should print "0"

    std::cout << ft2.eval("1000010101") << std::endl;
    // this should print "1"
  }

  if (print_tree){
    std::cout << "behold, the tree: \n" << std::endl;
    
    BNode* beetroot = ft2.showRoot();
    
    rec_print_bt(beetroot);
    std::cout << std::endl;
  }

  if (run_bin_test){
    std::cout << std::endl;
    std::cout << "now running a full check of which fvals are represented by the tree:\n" << std::endl;

    BNode* beetroot = ft2.showRoot();
    
    full_bin_test(beetroot, length);
    std::cout << std::endl;
  }

  return 0;
}
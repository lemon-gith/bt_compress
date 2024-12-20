#include <format>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include "../src/builder.h"


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
    val = rand() % int(pow(2, length));
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

  int val = rand() % int(pow(2, length));
  std::string test_str = std::vformat(fmt, std::make_format_args(val));

  while (inFvals(test_str, fvals)){
    val = rand() % int(pow(2, length));
    test_str = std::vformat(fmt, std::make_format_args(val));
  }
  return test_str;
}
//endregion





// TODO: refactor and remove testing-related elements
//        only permit user-inputted values here
struct TestArguments {
  int length = 15;
  int no_of_fvals = 1000;
  bool print_tree = false;
  bool run_bin_test = false;
};

// TODO: allow this to take in fvals from cli, e.g. pipe
TestArguments parse_args(int argc, char* argv[]) {
  TestArguments args;

  if (argc == 1) {
    // nothing passed, use defaults
    return args;
  } else if (
    (std::string(argv[1]) == "-h") || (std::string(argv[1]) == "--help")
  ) {  // help page
    std::cout << "usage: ./test_main [\n"
    << "  -l|--length <length> : length of generated fvals\n"
    << "  -n|--number <number> : number of generated fvals\n"
    << "  -p|--print           : print the produced binary tree\n"
    << "  -b|--bin-test        : exhaustive test over all potential values\n"
    << "]\n\n"
    << "- calling binary without args will use default values:\n"
    << "  - length = 15\n"
    << "  - number = 1000\n"
    << "  - print = false\n"
    << "  - bin-test = false"
    << std::endl;
    exit(0);
  }

  for (int i = 1; i < argc; i++) {
    std::string arg = argv[i];
    if (arg[0] != '-') {
      std::cerr << "direct inputs not permitted in test"
      << arg << " is passed illegally" << std::endl;
      exit(2);
    }
    else if ((arg == "-l") || (arg == "--length")) {
      args.length = std::stoi(argv[++i]);
    }
    else if ((arg == "-n") || (arg == "--number")) {
      args.no_of_fvals = std::stoi(argv[++i]);
    }
    else if ((arg == "-p") || (arg == "--print")) {
      args.print_tree = true;
    }
    else if ((arg == "-b") || (arg == "--bin-test")) {
      args.run_bin_test = true;
    }
  }

  return args;
}


int main(int argc, char* argv[]) {
  TestArguments args = parse_args(argc, argv);

  // automated main program
  std::vector<std::string> values;
  values = ranFvals(args.length, args.no_of_fvals);
  for (const std::string &val : values){
    std::cout << val << std::endl;
  }

  std::cout << "max: " << (pow(2, values[0].length() + 1) - 1) << " nodes :0" << std::endl;

  BoolTree ft2(values);
  // this corresponds to the f(x1, x2, x3) example shown above

  std::cout << "constructed with " << ft2.n_nodes() << " nodes :)" << std::endl;

  if (args.print_tree){
    std::cout << "behold, the tree: \n" << std::endl;

    BNode* beetroot = ft2.showRoot();

    rec_print_bt(beetroot);
    std::cout << std::endl;
  }

  if (args.run_bin_test){
    std::cout << std::endl;
    std::cout << "now running a full check of which fvals are represented by the tree:\n" << std::endl;

    BNode* beetroot = ft2.showRoot();

    full_bin_test(beetroot, args.length);
    std::cout << std::endl;
  }

  return 0;
}
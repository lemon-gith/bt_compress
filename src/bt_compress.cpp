#include <format>
#include <fstream>
#include <cstdlib>
#include <iostream>
#include "builder.hpp"


void rec_print_bt(BNode* root, int layer = 0) {
  if (root == nullptr) {
    return;
  }
  for (int i = 0; i < layer; i++) {
    std::cout << "-\t";
  }
  std::cout << root->val << std::endl;
  layer++;
  rec_print_bt(root->left, layer);
  rec_print_bt(root->right, layer);
}


// TODO: refactor and remove testing-related elements
//        only permit user-inputted values here
struct Arguments {
  bool read_from_txt = true;
  std::string txt_location = "example.txt";
  bool print_tree = false;
  bool run_bin_test = false;
  std::vector<std::string> ivals;
};

// TODO: allow this to take in fvals from cli, e.g. pipe
Arguments parse_args(int argc, char* argv[]) {
  Arguments args;

  if (argc == 1) {
    // nothing passed, use defaults
    return args;
  } else if (
    (std::string(argv[1]) == "-h") || (std::string(argv[1]) == "--help")
  ) {  // help page
    std::cout << "usage: ./bt_compress [\n"
    << "  -f|--file <file>     : file to read fvals from\n"
    << "  -l|--length <length> : length of generated fvals\n"
    << "  -n|--number <number> : number of generated fvals\n"
    << "  -p|--print           : print the produced binary tree\n"
    << "  -b|--bin-test        : exhaustive test over all potential values\n"
    << "] [\n"
    << "  <input> (501)        : a list of binary values to be compressed\n"
    << "]\n\n"
    << "- `length` and `number` are currently not working\n"
    << "- calling binary without args will use default values"
    << std::endl;
    exit(0);
  }

  for (int i = 1; i < argc; i++) {
    std::string arg = argv[i];
    if (arg[0] != '-') {
      // TODO: implement, could write a parsing fn (w/ whitespace trimming)
      //        and make std::vector<std::string> ivals
      std::cerr << "501: direct input not yet implemented" << std::endl;
      std::cerr << arg << " is passed illegally" << std::endl;
      exit(2);
    }
    else if ((arg == "-f") || (arg == "--file")) {
      args.txt_location = argv[++i];
      args.read_from_txt = true;
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
  Arguments args = parse_args(argc, argv);

  // grab fvals from given location
  std::vector<std::string> values;
  if (args.read_from_txt) {
    std::cout << "reading from file: " << args.txt_location << std::endl;
    std::ifstream infile;
    infile.open(args.txt_location);

    if (!infile.is_open()) {
      std::cout << "error opening file" << std::endl;
      return EXIT_FAILURE;
    }

    std::string tmp;

    while (infile >> tmp) {
      // just a minor modification to allow for comments
      if (tmp[0] != '#') {
        values.push_back(tmp);
      }
    }

    infile.close();
  }
  else{
    values = args.ivals;
  }

  std::cout << "\nmax: "
  << (pow(2, values[0].length() + 1) - 1) << " nodes :0"
  << std::endl;

  BoolTree ft2(values);
  // this corresponds to the f(x1, x2, x3) example shown above
  // TODO: ^ what is this referring to?

  std::cout << "constructed with " << ft2.n_nodes() << " nodes :)" << std::endl;

  // TODO: rewrite this to be a bit more useful
  if ((args.read_from_txt) && (args.txt_location == "example.txt")) {
    std::cout << ft2.eval("0011001010") << std::endl;
    // this should print "0"

    std::cout << ft2.eval("1000010101") << std::endl;
    // this should print "1"
  }

  if (args.print_tree) {
    std::cout << "behold, the tree: \n" << std::endl;

    // TODO: make this a method of BoolTree
    BNode* beetroot = ft2.showRoot();

    rec_print_bt(beetroot);
    std::cout << std::endl;
  }

  if (args.run_bin_test) {
    std::cout << std::endl;
    std::cout << "now running a full check of which fvals are represented by the tree:\n" << std::endl;

    BNode* beetroot = ft2.showRoot();

    // TODO: make this take in object pointer and call eval method
    // full_bin_test(beetroot, length);
    std::cout << std::endl;
  }

  return 0;
}
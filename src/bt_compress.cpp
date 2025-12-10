#include <format>
#include <fstream>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include "simplification.hpp"
#include "booltree.hpp"
#include "builder.hpp"
#include "bnode.hpp"


// TODO: refactor and remove testing-related elements
//        only permit user-inputted values here
struct Arguments {
  bool read_from_txt = true;
  // location of the text file to read fvals from
  std::string txt_location = "test/data/values.txt";
  bool print_tree = false;
  bool run_bin_test = false;
  // input values
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
    std::cout << "usage: " << argv[0] << " [\n"
    << "  -f|--file <file>     : "
    << args.txt_location
    << " : file to read fvals from\n"
    // << "  -l|--length <length> : length of generated fvals\n"
    // << "  -n|--number <number> : number of generated fvals\n"
    << "  -p|--print           : false : print the produced binary tree\n"
    << "  -b|--bin-test        : false "
    << ": exhaustive test over all potential values\n"
    << "] [\n"
    << "  <input>              : {NULL} "
    << ": a list of binary values to be compressed\n"
    << "]\n\n"
    << "- calling binary without args will use default values"
    << std::endl;
    exit(0);
  }

  for (int i = 1; i < argc; i++) {
    std::string arg = argv[i];
    if (arg[0] != '-') {
      if (arg[0] == '0' || arg[0] == '1') {  // likely a binary string
        args.ivals.push_back(arg);
        args.read_from_txt = false;
      }
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

  // ensure that the provided fvals are valid
  int val_length = border_control(values);

  std::cout << "\nmax: " << (pow(2, values[0].length() + 1) - 1)
    << " nodes :0" << std::endl;

  BoolTree ft2(values, val_length);

  std::cout << "constructed with " << ft2.n_nodes() << " nodes :)" << std::endl;

  if ((args.read_from_txt) && (args.txt_location == "test/data/values.txt")) {
    std::cout << ft2.eval("0011001010") << std::endl;
    // this should print "0"

    std::cout << ft2.eval("1000010101") << std::endl;
    // this should print "1"
  }

  if (args.print_tree) {
    std::cout << "behold, the tree: \n" << std::endl;

    BNode* beetroot = ft2.showRoot();

    rec_print_bt(beetroot);
    std::cout << std::endl;
  }

  if (args.run_bin_test) {
    std::cout << std::endl;
    std::cout
      << "Please enter the values you would like to test (one-by-one), "
      << "or type 'exit' to quit:\n" << std::endl;

    bool exit = false;
    while (!exit) {
      bool valid_fval = true;
      std::string test_val;
      std::cout << "> ";
      std::cin >> test_val;

      for (char& c : test_val) {
        if (c != '0' && c != '1') {
          valid_fval = false;
          break;
        }
      }

      if (valid_fval) {
        std::string result = ft2.eval(test_val);

        if (result == "0" || result == "1") {
          std::cout << "Result: " << (result == "0" ? "false" : "true")
            << std::endl;
        } else {
          std::cout << result << std::endl;
        }

      } else {
        if (test_val == "exit") {
          exit = true;
          std::cout << "Exiting interactive binary test.\n" << std::endl;
        } else {
          std::cout << "Err: Invalid fval entered, please try again."
            << std::endl;
        }
      }
    }

    std::cout << std::endl;
  }

  return 0;
}

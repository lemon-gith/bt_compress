#ifndef UTILS_HPP
#define UTILS_HPP

/*
A set of static utility functions,
that are completely independent of any components/definitions
used in the rest of the project.
*/

#include <vector>
#include <string>


// just a simplified and specialised ver. of fill() in <algorithms>
void empty_fill(std::vector<std::string> &vec, int count);

// given a vector of strings, find all duplicates and return their indices as pairs
std::vector<std::pair<int, int>> findDuplicates(
  const std::vector<std::string> &rvals
);

// produces a copy of the input vector with duplicates removed
std::vector<std::string> removeDuplicates(std::vector<std::string> vec);

#endif
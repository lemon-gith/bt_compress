#ifndef SIMPLIFICATION_HPP
#define SIMPLIFICATION_HPP

#include <vector>
#include <string>


int maxSecond(const std::vector<std::pair<int, int>> &dups);

void collapseSimilar(
  std::vector<std::string> &rvals, std::vector<char> &omitted_col,
  std::vector<std::pair<int, int>> &duplicates
);

std::vector<std::string> simplifyFvals(std::vector<std::string> fvals);

// function to sanitise the input data
int border_control(std::vector<std::string> fvals);

#endif
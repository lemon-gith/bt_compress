#include "utils.hpp"

#include <unordered_set>

void empty_fill(std::vector<std::string> &vec, int count) {
  for (int i = 0; i < count; i++) {
    vec.push_back("");
  }
}

std::vector<std::pair<int, int>> findDuplicates(
  const std::vector<std::string> &rvals
) {
  std::vector<std::pair<int, int>> duplicates{};

  int rvals_size = int(rvals.size());

  for (int comparator = 0; comparator < rvals_size; comparator++){
    for (int comparand = comparator + 1; comparand < rvals_size; comparand++){
      if (rvals[comparator] == rvals[comparand]){
        // in ascending order
        duplicates.push_back(std::pair<int, int>(comparator, comparand));
      }
    }
  }
  return duplicates;
}

std::vector<std::string> removeDuplicates(std::vector<std::string> vec) {
  std::unordered_set<std::string> unique_fvals(vec.begin(), vec.end());
  std::vector<std::string> unique_vec = std::vector<std::string>(
    unique_fvals.begin(), unique_fvals.end()
  );

  return unique_vec;
}
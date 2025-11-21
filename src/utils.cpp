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

  for (int comparator = 0; comparator < rvals.size(); comparator++){
    for (int comparand = comparator + 1; comparand < rvals.size(); comparand++){
      if (rvals[comparator] == rvals[comparand]){
        duplicates.push_back(std::pair<int, int>(comparator, comparand)); // in ascending order
      }
    }
  }
  return duplicates;
}

void removeDuplicates(std::vector<std::string> &vec) {
  std::unordered_set<std::string> unique_fvals(vec.begin(), vec.end());
  vec.clear();
  vec.reserve(unique_fvals.size());
  for (const std::string fval : unique_fvals){
    vec.push_back(fval);
  }
}
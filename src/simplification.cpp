#include "simplification.hpp"
#include "utils.hpp"
#include <stdexcept>


int maxSecond(const std::vector<std::pair<int, int>> &dups) {
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

    for (int i = 1; i < (int)dups.size(); i++){
      if (dups[i].second > max_val){
        max_val = dups[i].second;
        max_idx = i;
      }
    }
    return max_idx;
  }
}

void collapseSimilar(
  std::vector<std::string> &rvals, std::vector<char> &omitted_col,
  std::vector<std::pair<int, int>> &duplicates
) {
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

std::vector<std::string> simplifyFvals(std::vector<std::string> fvals) {
  // not removing duplicates, as shouldn't theoretically happen here
  std::vector<char> omitted_col{};
  omitted_col.reserve(fvals.size());
  std::vector<std::string> rvals{};
  rvals.reserve(fvals.size());
  std::vector<std::pair<int, int>> duplicates{};

  for (int i = 0; i < (int)fvals[0].size(); i++){
    omitted_col.clear();

    for (std::string &fval : fvals){
      omitted_col.push_back(fval[i]);
      fval.erase(fval.begin() + i);
    }

    duplicates = findDuplicates(fvals);
    collapseSimilar(fvals, omitted_col, duplicates);

    for (int rep_idx = 0; rep_idx < (int)omitted_col.size(); rep_idx++) {
      fvals[rep_idx].insert(fvals[rep_idx].begin() + i, omitted_col[rep_idx]);
    }
  }
  return fvals;
}

int border_control(std::vector<std::string> fvals) {
  int length = int((fvals)[0].size());

  // perform a checked type conversion
  unsigned long label_len_ul = (fvals)[0].size();
  if (label_len_ul > (__INT32_MAX__ - 1)) {
    // `-1` as label can be up to (fval length + 1)
    throw std::length_error("fvals are too long for int");;
  }

  for (const std::string &fval : fvals){
    if ((int)fval.size() != length){
      // TODO: track max-length and zero-pad to longest, also give error output
      throw std::length_error("not all fvals are of the same length");
    }
    for (const char &val : fval){
      if ((val != '0') && (val != '1')){
        throw std::invalid_argument("input contains non-binary characters");
      }
    }
  }

  return length;
}
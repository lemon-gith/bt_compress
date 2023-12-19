# bt_compress
Binary String Storage Tree Compressor: Reduces and re-arranges data as it builds the tree, removing as much redundancy as it can, resulting in many fewer nodes than a (Full) Perfect Binary Tree. The general idea is to take a vector of input data (binary strings) and construct a tree that allows one to later check whether a (later) given binary string was or wasn't in the original list, used to construct the tree.

## What and why is this?
This project was born of frustration (storytime):\
in my first year at uni, our 2nd piece of coursework was to write a program that generated a Perfect Binary Tree that would store a list of binary strings (all the same length). The 3rd piece of coursework then asked us to improve on that and to try to make the final tree take up as little space as possible. The easy answer was to just construct the necessary branches and then do a bit of pruning, however, that rarely produces an optimal structure that takes up the minimum amount of space, so I scribbled some ideas down and with inspiration from a friend was able to come up with an algorithmic solution that would first simplify the incoming values, then keep them arranged optimally while building the tree.

As a note, this is more a display of something that I've done, than a genuine solution that I expect to see people flocking to use :p

## How does it work?
The three key concepts:
1. the collapsing of `fvals` (the vector of binary strings) where they overlap
2. the re-ordering of `bvals` columns to optimise the column on which trees are split
3. the recursive splitting of `bvals` between subtrees, so each subtree builds itself on a smaller subset of the data, i.e. it divides and conquers

### Core Algorithm
```
                              split into subtrees
                           |-----<------|----<----|
                           |            |         |
[fvals] --> [svals] --> [bvals] --> [l_vals], [r_vals]
   |----->-----|
   using [rvals]

|_________________|     |____________________________|
       SETUP                        BUILD
```

### Key Terms
| name | expanded | description |
| --- | --- | --- |
| `fvals` | functional values | input values to each sub-tree |
| `rvals` | reduced values | temporary value-vector that has one column removed (for matching) |
| `svals` | simplified values | `fvals`, but with similar values collapsed using don't cares ('X') |
| `bvals` | build values | essentially `svals`, but with reordered columns: used for construction |

| fvals | rvals | svals | bvals (L) | bvals (R) |
| :---: | :---: | :---: | :---: | :---: |
| 0100101<br>1001001<br>1001011<br>0101001<br>0100001 | 01001-1<br>10010-1<br>10010-1<br>01010-1<br>01000-1 | 10010X1<br>0101001<br>0100X01 | 101001<br>100X01 | 0010X1 |

### Walkthrough
Pre-SETUP, some input sanitisation is done (not necessary for the coursework, but why not future-proof it :p). This is important because the following simplification depends on each layer having already eliminated all duplicates and each value being the same length.

`fvals` are then simplified into `svals` using a series of `rvals`: this iteration is only performed once, since, once collapsed, `svals` don't usually re-expand, meaning that they don't need to be re-collapsed. To demystify:\
My friend came up with the first key idea that, by removing a column and checking for subsequent duplicates, you can see which strings only differ by only one value (have a hamming distance of 1), and in tree terms, this means that these strings can be collapsed into one string with a don't care ('X') in place of that omitted value: it doesn't matter whether the later query asks if the value at that position was a '0' or a '1', since both of those were in the original list.\
The way this is done, is by iteratively removing each column, one-by-one (producing a corresponding `rvals` vector), keeping track of matching columns, then collapsing them in the `svals`, before continuing the iteration.

Once those simplifications are performed, the `svals` are fed into a `Builder`: it is the Builder's job to construct a specific sub-tree and then return the root of that sub-tree. Of course, the Base Builder, `Bob`, gets the recursive tree construction started, then later returns its root, which is the root of the whole tree. 

When a `Builder` object is constructed, it takes the incoming values (and _current_ order of the columns) and re-orders the columns according to a vector of `CoCo`s, which are calculated for each column. This is the second key idea, since this allows the algorithm to optimise which columns are split on and when. See [`CoCo`s](#cocos) for more info.

Once re-ordered, the Central `Builder`, `Cob` (i.e. the parent), splits these `bvals` between its left and right children/sub-trees, `Lob` and `Rob`, respectively, based on the character in the first column of each string, '0' goes to `Lob`, '1' for `Rob`, and 'X' must be split up and given to both sub-trees. The actual construction is a recursive process that terminates when a leaf is constructed.

At this point, if a new `fval` is evaluated it would return '1' if it is in the tree and hit a `nullptr` otherwise, but there's one more [`add_0s()`](/bt.hpp#L378) function that converts all `nullptr`s to 0-leaves, to fit the coursework specifications, so that it will return 0 if an `fval` is not in the tree. Once that's done, a pointer to the root of the tree is returned and we're done :)

### `CoCo`s
Correlation Coefficients: an artifact from my initial attempts at devising an algorithm. I was initially thinking of counting the number of '0's and '1's in each column (not each string) and using the computed values from that (basically entropy) to re-order the columns, but that didn't properly yield results and also lacked a logical thread of reasoning. 

Despite moving on from that method, the concept, of calculating a coefficient for each column and re-ordering based on that, was here to stay. Now, it is a measure of how many don't cares, 'X's, there are in each column, and if there aren't any, it represents whether the values in a column are homogeneous or not. The reason behind the first point, is that by pushing the column with the most 'X's to the very end, we are minimising the amount of re-ordering we'll have to do in future passes and maximising the chance that we'll have a chain of 'X's at the end of more branches to come.\
The truth is, that actually, it doesn't matter what order the columns are in, as long as:
> If a homogenous column* exists, it should _always_ be split on.

*a column that contains only '0's or only '1's, since this reduces the complexity of the tree (an unnecessary sub-tree doesn't need to be created)

#### Sidenote
Having an 'X' as the first character in a re-ordered `bval` is something I hadn't anticipated when first writing this, it required me to implement a fix just before uploading this project to GitHub (it's now been 2 years since I wrote the original code). It's something that's only possible when _every_ column has at least one don't care ('X') in it. It's an undesirable but unavoidable consequence of being able to collapse too many `fvals` (another reason to re-order the columns): it is the one exception to the permanence of the collapsing done by `simplifyFvals()`, since it represents a string that holds redundancy, but must now be re-expanded to be sent down to the correct sub-trees. Fortunately, this doesn't ruin the optimality of the algorithm, since the only reason a string would be re-expanded is if that column absolutely _had_ to be at the front, i.e. if we were to split on a different column we would have to re-expand a greater than or equal number of strings.

## How do I use it?
You can expect usability edits in the future when I gain more experience writing code for something other than personal projects, but for now:

If you want to use my `main.cpp` file for testing and seeing just what levels of shenaniganry my solution can cope with, then feel free to  compile that file (`bt.hpp` is `included`) and modify the [user-defined values](./main.cpp#L80).

```c++
// if read_from_text is false, the program generates a randomised list of fvals 
bool read_from_txt = false;
// ?
int length = 15;  // how long is each individual fval
int no_of_fvals = 1000;  // how many fvals
// :
std::string txt_location = "values.txt";  // where should the program read fvals from?
// ;
bool print_tree = false;  // should the tree be printed in the terminal?
bool run_bin_test = false;  // should the program run a full test of all binary values of length `length`, to check the tree?
```

If you want to use your own `main.cpp`, be my guest, just remember to `#include "bt.hpp"`. \
This is the defined interface:

```c++
class BoolTree{
  // The class from which binary string storage trees can be instantiated and constructed using provided fvals
  explicit BoolTree(const std::vector<std::string>& fvalues){...}

  // a function that can be called to evaluate whether or not a given string is in the tree or not
  std::string eval(const std::string& s){...}

  // a function that returns the total number of nodes in the tree (including leaves)
  int n_nodes(){...}
};
```

Since this was initially designed as a coursework problem, the interface is pretty limited and minimal, but with time, I may come back to this project and add more things to make it a bit easier to use.\
The origin story is also the reason that there's a [provided code](/bt.hpp#L11,60) section at the top.

## Notes

I plan on making quite a few improvements to readability, correctness, and function when I find the time. If there are any other suggestions or other improvements, please feel free to open an issue or fork my code.

If you want to use this code as reference or in your own personal project, I would be honoured. As long as any subsequent code isn't commercialised without permission (cos that's no fun for anyone), go ham :)

And if there's anything you feel to be missing or if there's a very similar solution that exists somewhere, feel free to make me aware, I'm more than happy to add references or links to other people's work; just know that this was pretty much wholly the product of my here noggin after hours of intense thinking and scribbling, so if you feel something went unreferenced, I likely just didn't reference it in the first place.

---

## TODOs
TODO4: rewrite confusing names in bt\
TODO5: deal with `delete_()` and properly `delete` `Builder`s in [`constructBT()`](/bt.hpp#L275)\
TODO6: [improve the randomisation](https://cplusplus.com/reference/random/mt19937_64/) to increase the range to be able to take [any length of fval](https://www.geeksforgeeks.org/binary-representation-of-a-given-number/) that you want\
TODO7: improve outer interface and allow tree to be more searchable, perhaps reconstructing the original list or searching using gapped inputs?\
TODO8: idk, any other [improvements](https://en.cppreference.com/w/cpp/utility/format/vformat) to the code\

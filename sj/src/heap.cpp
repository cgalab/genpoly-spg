#include <vector>
#include <iostream> // for endl and cerr

// testing implementing heap's algorithm
// code from https://www.topcoder.com/blog/generating-permutations

void swap (std::vector<unsigned int>& a, int i, int j) {
  unsigned int temp = a[i];
  a[i] = a[j];
  a[j] = temp;
}

void heap(std::vector<unsigned int>& a, int n) {
  if(n == 1) {
    // (got a new permutation)
    for(unsigned int i = 0; i < a.size(); ++i) std::cerr << a[i] << ",";
    std::cerr << "\n";
    return;
  }
  for(int i = 0;i < (n - 1);i++) {
    heap(a, n-1);
    // always swap the first when odd,
    // swap the i-th when even
    if(n % 2 == 0) swap(a, n-1, i);
    else swap(a, n-1, 0);
  }
  heap(a, n-1);
}

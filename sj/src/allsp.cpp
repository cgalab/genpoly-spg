#include <iostream> // for endl
#include <assert.h>
#include <array>
#include "basicDefinitions.h"
#include "point.h"
#include "edge.h"
#include "heap.h"
#include "allsp.h"


enum error allsp(std::vector<std::vector<unsigned int>>& sph, std::vector<Point>& points, unsigned int randseed) {
  std::cerr << "inside allsp" << std::endl;
  std::cerr << "sph: " << sph.size() << ", points: " << points.size() << ", seed: " << randseed << std::endl;
  unsigned int arr_s = points.size()*points.size();
  std::vector<std::vector<bool> > checked;
  std::vector<std::vector<bool> > intersects;

  for (unsigned int i = 0; i < arr_s;++i) {
    checked.push_back(std::vector<bool> (arr_s,0));
    intersects.push_back(std::vector<bool> (arr_s,0));
  }

  std::cerr << "checked: " << checked.size() << ", c[0][0]: " << checked[0][0] << ", intersects: " << intersects.size() << std::endl;

  std::cerr << "=========== Heap's alg. test ================" << std::endl;

  std::vector<unsigned int> a;
  for (unsigned int i = 0; i < points.size();++i) a.push_back(i);
  heap2(a, a.size()-1);


  return SUCCESS;
}

void swap2 (std::vector<unsigned int>& a, int i, int j) {
  unsigned int temp = a[i];
  a[i] = a[j];
  a[j] = temp;
}

void heap2(std::vector<unsigned int>& a, int n) {
  if(n == 1) {
    // (got a new permutation)
    for(unsigned int i = 0; i < a.size(); ++i) std::cerr << a[i] << ",";
    std::cerr << "\n";
    return;
  }
  for(int i = 0;i < (n - 1);i++) {
    heap2(a, n-1);
    // always swap the first when odd,
    // swap the i-th when even
    if(n % 2 == 0) swap2(a, n-1, i);
    else swap2(a, n-1, 0);
  }
  heap2(a, n-1);
}

#ifndef __RAND_H_
#define __RAND_H_

#include <random>

std::random_device rd;  // global variables for random engine
std::mt19937 mt (rd());

// Min and Max are inclusive elements
#define UniformRandomI(C, Min, Max)                   \
{                                                     \
  std::uniform_int_distribution<int> dist(Min, Max);  \
  C = dist(mt);                                       \
}

// Min and Max are inclusive elements
#define UniformRandomD(C, Min, Max)                     \
{                                                       \
  std::uniform_real_distribution<double> dist(Min, Max); \
  C = dist(mt);                                         \
}


#endif

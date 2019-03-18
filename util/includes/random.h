#include <random>

#ifndef __RANDOM_H_
#define __RANDOM_H_

std::random_device rd;  // global variables for random engine
std::mt19937 mt(rd());

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
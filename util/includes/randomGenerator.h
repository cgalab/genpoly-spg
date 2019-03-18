#include <random>

#ifndef __RANDOMGENERATOR_H_
#define __RANDOMGENERATOR_H_

class RandomGenerator{
	private:
		std::default_random_engine generator;
		
	public:
		int getRandomIndex(int n){ return std::uniform_int_distribution<int> {0, n - 1} (generator); }
};

#endif
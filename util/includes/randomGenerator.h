#include <random>

#ifndef __RANDOMGENERATOR_H_
#define __RANDOMGENERATOR_H_

class RandomGenerator{
	private:
		std::random_device rd;  // global variables for random engine
		std::mt19937 generator;

	public:
		RandomGenerator(){
			unsigned int seed = rd();
			//seed = 2187281837;

			printf("seed: %u \n", seed);

			generator.seed(seed);
		}

		int getRandomIndex(int n){ 
			return std::uniform_int_distribution<int> {0, n - 1} (generator);
		}

		double getTranslationNormal(double mean, double stddev){
			double trans;
			double limit = 0.001;

			trans = std::normal_distribution<double> {mean, stddev} (generator);	

			if(trans < limit && trans > - limit){
				if(trans > 0)
					trans = limit;
				else
					trans = - limit;
			}

			return trans;
		}

		double getTranslationUniform(double min, double max){
			return std::uniform_real_distribution<double> {min, max} (generator);	
		}
};

#endif
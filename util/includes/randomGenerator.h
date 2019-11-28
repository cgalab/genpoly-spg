/*
   Include standard libraries
*/
#include <random>

#ifndef __RANDOMGENERATOR_H_
#define __RANDOMGENERATOR_H_

class RandomGenerator{
	private:
		
		/*
			The random device for getting a random seed
		*/
		std::random_device rd;

		/*
			The generator
		*/
		std::mt19937 generator;

	public:
		
		/*
			C ~ O ~ N ~ S ~ T ~ R ~ U ~ C ~ T ~ O ~ R ~ S
		*/

		/*
			Constructor:
			
			@param 	fixedSeed 	Indicates whether the generator shall be initialized with a random
								seed from the random device or a fixed seed
			@param 	seed 		This number will be used as the seed of the random generator if the
								flag fixedSeed is unset
		*/
		RandomGenerator(bool fixedSeed, unsigned int &seed){
			
			if(!fixedSeed){
				seed = 0;
				while(seed == 0)
					seed = rd();
			}

			generator.seed(seed);
		}


		/*
			O ~ T ~ H ~ E ~ R ~ S
		*/

		/*
			The function getRandomIndex() generates a uniformly distributed random integer betweem 0
			and max.

			@param 	max 	The number of elements to chose one
			@return 		The random index 	
		*/
		int getRandomIndex(int max){ 
			return std::uniform_int_distribution<int> {0, max - 1} (generator);
		}

		/*
			The function getDoubleNormal() generates a random double from a normal distribution with
			parameters mean and stddev. The absolute value of the double is at least 0.0000001.

			@param 	mean	The mean value of the distribution
			@param 	stddev 	The standard deviation of the distribution
			@return 		The generated random double
		*/
		double getDoubleNormal(double mean, double stddev){
			double trans;
			double limit = 0.0000001;

			trans = std::normal_distribution<double> {mean, stddev} (generator);	

			if(trans < limit && trans > - limit){
				if(trans > 0)
					trans = limit;
				else
					trans = - limit;
			}

			return trans;
		}

		/*
			The function getDoubleUniform() generates a random double from a uniform distribution with
			between min and max.

			@param 	min		The lower bound of the random value
			@param 	max 	The upper bound of the random value
			@return 		The generated random double
		*/
		double getDoubleUniform(double min, double max){
			return std::uniform_real_distribution<double> {min, max} (generator);	
		}
};

#endif

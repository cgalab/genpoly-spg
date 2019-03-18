#include <random>

#ifndef __RANDOMGENERATOR_H_
#define __RANDOMGENERATOR_H_

class RandomGenerator{
	private:
		std::default_random_engine generator;
		
	public:
		int getRandomIndex(int n){ return std::uniform_int_distribution<int> {0, n - 1} (generator); }
		
		Point* translatePointNormal(Point* v, double mean, double stddev){
			double x = (*v).x;
			double y = (*v).y;
			unsigned int i = (*v).i;
			double dx, dy;
			
			dx = std::normal_distribution<double> {mean, stddev} (generator);	
			dy = std::normal_distribution<double> {mean, stddev} (generator);				
			
			return new Point(x + dx, y + dy, i);
		}
};

#endif
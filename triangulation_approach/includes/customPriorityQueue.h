#include "triangle.h"
#include <queue>
#include <stdio.h>

class custom_priority_queue : public std::priority_queue<std::pair<double, Triangle*>, std::vector<std::pair<double,Triangle*>>, std::greater<std::pair<double, Triangle*>>>{

public:
	void remove(Triangle* t){
		Triangle* T;
		int s = c.size();
		auto it = c.begin();

		for(int i = 0; i < s; i++){
			T = c[i].second;

			if(t == T){
				c.erase(it);
				std::make_heap(c.begin(), c.end(), comp);
				break;
			}
			std::advance(it, 1);
		}
	}

	void check(){
		std::pair<double, Triangle*> e;
		Triangle* t;
		double time;

		while(!empty()){
			e = top();
			pop();
			time = e.first;
			t = e.second;

			printf("at time %f:\n", time);
			(*t).print();
		}
	}
};
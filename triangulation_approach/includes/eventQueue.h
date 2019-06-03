#include "triangle.h"

#ifndef __EVENTQUEUE_H_
#define __EVENTQUEUE_H_

struct element{
	double collapseTime;
	Triangle* triangle;

	struct element* next;
};

class EventQueue{

private:

	struct element* first;
	int n;
	double epsilon;

public:

	EventQueue(double eps);
	void remove(Triangle* t);
	bool insert(double time, Triangle* t);
	std::pair<double, Triangle*> pop();
	int size();
	void print();
};

#endif
#include "triangle.h"

#ifndef __EVENTQUEUE_H_
#define __EVENTQUEUE_H_

struct Element{
	double collapseTime;
	Triangle* triangle;

	struct Element* next;
};

class EventQueue{

private:

	struct Element* first;
	int n;
	double epsilon;

public:

	EventQueue(double eps);
	void remove(Triangle* t);
	bool insert(double time, Triangle* t);
	std::pair<double, Triangle*> pop();
	int size();
	void print();
	~EventQueue();
};

#endif
#include "triangle.h"
#include "vertex.h"
#include "tedge.h"

#ifndef __EVENTQUEUE_H_
#define __EVENTQUEUE_H_

struct Event{
	double collapseTime;
	Triangle* triangle;

	struct Event* next;
};

class EventQueue{

private:

	struct Event* first;
	int n;
	double epsilon;

	Vertex* original;
	Vertex* oldV;
	Vertex* newV;

	void stabilize(struct Event* e0, struct Event* e1);
	void stabilizeConvex(struct Event* e0, struct Event* e1, TEdge* commonE);
	void stabilizeNonConvex(struct Event* e0, struct Event* e1, TEdge* commonE);

public:

	EventQueue(double eps, Vertex* orig, Vertex* oV, Vertex* nV);

	bool insert(double time, Triangle* t);
	void insertWithoutCheck(double time, Triangle* t);

	bool makeStable(bool initial);

	std::pair<double, Triangle*> pop();
	int size();
	void remove(Triangle* t);
	
	void print();

	~EventQueue();
};

#endif
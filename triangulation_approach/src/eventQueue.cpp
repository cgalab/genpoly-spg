#include "eventQueue.h"

EventQueue::EventQueue(double eps, Vertex* orig, Vertex* oV, Vertex* nV) : 
first(NULL), n(0), epsilon(eps), original(orig), oldV(oV), newV(nV) {}

void EventQueue::insertWithoutCheck(double time, Triangle* t){
	struct Event* e0;
	struct Event* e1, *prev = NULL;
	double time1, prevTime = -100;

	e1 = first;
	while(e1 != NULL){
		time1 = e1 -> collapseTime;

		if(time < time1){
			// insert new element
			e0 = (struct Event*)malloc(sizeof(struct Event));

			e0 -> collapseTime = time;
			e0 -> triangle = t;
			e0 -> next = NULL;

			if(prev != NULL){
				e0 -> next = prev -> next;
				prev -> next = e0;
			}else{
				e0 -> next = first;
				first = e0;
			}

			break;
		}
		
		prev = e1;
		prevTime = time1;

		e1 = e1 -> next;
	}

	// insert at the end
	if(e1 == NULL){
		// insert new element
		e0 = (struct Event*)malloc(sizeof(struct Event));

		e0 -> collapseTime = time;
		e0 -> triangle = t;
		e0 -> next = NULL;

		// check whether there was already an element in the queue
		if(prev != NULL){
			e0 -> next = prev -> next;
			prev -> next = e0;
		}else
			first = e0;
	}

	n++;
}


/*bool EventQueue::insert(double time, Triangle* t){
	struct Event* e0;
	struct Event* e1, *prev = NULL;
	double time1, prevTime = -100;

	e1 = first;
	while(e1 != NULL){
		time1 = e1 -> collapseTime;

		if(time < time1){
			// check for numerical stability
			if(time + epsilon >= time1){
				//printf("Eventqueue: prevented numerical stability\n");
				return false;
			}

			if(time - epsilon <= prevTime){
				//printf("Eventqueue: prevented numerical stability\n");
				return false;
			}

			// insert new element
			e0 = (struct Event*)malloc(sizeof(struct Event));

			e0 -> collapseTime = time;
			e0 -> triangle = t;
			e0 -> next = NULL;

			if(prev != NULL){
				e0 -> next = prev -> next;
				prev -> next = e0;
			}else{
				e0 -> next = first;
				first = e0;
			}

			break;
		}
		
		prev = e1;
		prevTime = time1;

		e1 = e1 -> next;
	}

	// insert at the end
	if(e1 == NULL){
		// check for numerical stability
		if(time - epsilon <= prevTime){
			//printf("Eventqueue: prevented numerical stability\n");
			return false;
		}

		// insert new element
		e0 = (struct Event*)malloc(sizeof(struct Event));

		e0 -> collapseTime = time;
		e0 -> triangle = t;
		e0 -> next = NULL;

		
		if(prev != NULL){
			e0 -> next = prev -> next;
			prev -> next = e0;
		}else
			first = e0;
	}

	n++;

	return true;
}*/

bool EventQueue::makeStable(bool initial){
	struct Event *e0, *e1;
	double time0, time1, dif;
	Triangle *t0, *t1;

	if(n == 0)
		return true;

	e0 = first;
	time0 = e0 -> collapseTime;
	while(e0 -> next != NULL){
		e1 = e0 -> next;

		time1 = e1 -> collapseTime;
		dif = fabs(time1 - time0);

		// two events are at the same time
		if(dif < epsilon){
			// check first whether there is a third event at the same time
			dif = fabs(time1 - e1 -> collapseTime);
			if(dif < epsilon){
				if(initial)
					printf("Eventqueue: More than two events at the same time -> refused translation\n");
				else
					printf("Eventqueue: More than two events at the same time -> aborted translation\n");
				return false;
			}

			// Attentione: do not change the ordering of e0 and e1 in stabilize, otherwise you would
			// get an infinite loop here!
			stabilize(e0, e1);
		}

		e0 = e1;
		time0 = time1;
	}

	return true;
}

void EventQueue::stabilize(struct Event* e0, struct Event* e1){
	Triangle *t0, *t1, *t;
	double area0, area1;
	TEdge* edge;
	Vertex *common, *v0, *v1;

	t0 = e0 -> triangle;
	t1 = e1 -> triangle;

	// check whether triangles are adjacent (share an edge):
	// as both triangles contain the vertex original, they have to share one of the two
	// edges incident to original
	edge = (*t0).getEdgeContaining(original);
	t = (*edge).getOtherTriangle(t0);
	// triangles do not share the first edge
	if((*t).getID() != (*t1).getID()){
		edge = (*t0).getOtherEdgeContaining(original, edge);
		t = (*edge).getOtherTriangle(t0);

		// triangles also don't share the second edge => triangles aren't adjacent
		// => the exact order doesn't matter
		if((*t).getID() != (*t1).getID())
			return;
	}

	// get the second vertex the two triangles share
	common = (*edge).getOtherVertex(original);

	// get the two not-shared vertices
	v0 = (*t0).getOtherVertex(edge);
	v1 = (*t1).getOtherVertex(edge);

	// comment: the edge of a triangle which is not incident to the moving vertex (original)
	// is called the opposing edge

	// find the right case:
	// 1. case:
	// opposing edges in convex shape

	t = new Triangle(v0, v1, original);
	area0 = (*t).signedArea();
	delete t;

	t = new Triangle(v0, v1, common);
	area1 = (*t).signedArea();
	delete t;

	// we have convex shape if sign(area0) != sign(area1)
	// or in the special case that area1 equals 0

	if(area1 == 0 || (signbit(area0) != signbit(area1)))
		stabilizeConvex(e0, e1, edge);

	// 2. case:
	// opposing edges in non-convex shape
	else
		stabilizeNonConvex(e0, e1, edge);
}

void EventQueue::stabilizeConvex(struct Event* e0, struct Event* e1, TEdge* commonE){
	Triangle *t0, *t1, *t;
	Vertex *common, *v0, *v1;
	double area0, area1, areaCommon;

	t0 = e0 -> triangle;
	t1 = e1 -> triangle;

	// get the second vertex the two triangles share
	common = (*commonE).getOtherVertex(original);

	// get the two not-shared vertices
	v0 = (*t0).getOtherVertex(commonE);
	v1 = (*t1).getOtherVertex(commonE);

	// now we have to check whether the transition line intersects one of the
	// opposing edges or just goes by

	t = new Triangle(oldV, newV, v0);
	area0 = (*t).signedArea();
	delete t;

	t = new Triangle(oldV, newV, v1);
	area1 = (*t).signedArea();
	delete t;

	// transition line doesn't intersect the opposing edges
	if(signbit(area0) == signbit(area1)){
		// now we have to check on which side the transition line passes
		// the closer triangle collapses first

		// therefore we search for the edge in the middle
		t = new Triangle(original, v0, newV);
		area0 = (*t).signedArea();
		delete t;

		t = new Triangle(original, v0, v1);
		area1 = (*t).signedArea();
		delete t;

		// if area0 and area1 have now different signs, then the edge from original
		// to v0 is in the middle, so the transition line is closer to t0, i.e. t0
		// collapses first and the initial order was right

		// otherwise we have to switch t0 and t1
		if(signbit(area0) == signbit(area1)){
			e0 -> triangle = t1;
			e1 -> triangle = t0;

			printf("order change time diff: %.25f \n", fabs(e0 -> collapseTime - e1 -> collapseTime));
		}

	// transition line intersects one of the oppossing edges
	}else{
		t = new Triangle(oldV, newV, common);
		areaCommon = (*t).signedArea();
		delete t;

		// if the transition line intersects t0, then t0 has to collapse first
		// so the ordering was right

		// if it intersects t1, the ordering must be changed
		if(signbit(area1) != signbit(areaCommon)){
			e0 -> triangle = t1;
			e1 -> triangle = t0;

			printf("order change time diff: %.25f \n", fabs(e0 -> collapseTime - e1 -> collapseTime));
		}
	}
}

void EventQueue::stabilizeNonConvex(struct Event* e0, struct Event* e1, TEdge* commonE){
	Triangle *t0, *t1, *t;
	Vertex *common, *v0, *v1;
	double area0, area1, areaCommon;

	t0 = e0 -> triangle;
	t1 = e1 -> triangle;

	// get the second vertex the two triangles share
	common = (*commonE).getOtherVertex(original);

	// get the two not-shared vertices
	v0 = (*t0).getOtherVertex(commonE);
	v1 = (*t1).getOtherVertex(commonE);

	// now we have to check whether the transition line intersects one of the
	// opposing edges or just goes by

	t = new Triangle(oldV, newV, v0);
	area0 = (*t).signedArea();
	delete t;

	t = new Triangle(oldV, newV, v1);
	area1 = (*t).signedArea();
	delete t;

	// transition line doesn't intersect the opposing edges
	if(signbit(area0) == signbit(area1)){
		// now we have to check on which side the transition line passes
		// the closer triangle collapses last

		// therefore we search for the edge in the middle
		t = new Triangle(original, v0, newV);
		area0 = (*t).signedArea();
		delete t;

		t = new Triangle(original, v0, v1);
		area1 = (*t).signedArea();
		delete t;

		// if area0 and area1 have now different signs, then the edge from original
		// to v0 is in the middle, so the transition line is closer to t0, i.e. t1
		// collapses first and the initial order was wrong

		// so we have to switch t0 and t1
		if(signbit(area0) != signbit(area1)){
			e0 -> triangle = t1;
			e1 -> triangle = t0;

			printf("order change time diff: %.25f \n", fabs(e0 -> collapseTime - e1 -> collapseTime));
		}

	// transition line intersects one of the oppossing edges
	}else{
		t = new Triangle(oldV, newV, common);
		areaCommon = (*t).signedArea();
		delete t;

		// if the transition line intersects t1, then t0 has to collapse first
		// so the ordering was right

		// if it intersects t0, the ordering must be changed
		if(signbit(area0) != signbit(areaCommon)){
			e0 -> triangle = t1;
			e1 -> triangle = t0;

			printf("order change time diff: %.25f \n", fabs(e0 -> collapseTime - e1 -> collapseTime));
		}
	}
}

std::pair<double, Triangle*> EventQueue::pop(){
	struct Event* e;
	std::pair<double, Triangle*> out;

	e = first;

	first = e -> next;

	out = std::make_pair(e -> collapseTime, e -> triangle);

	free(e);
	n--;

	return out;
}

int EventQueue::size(){
	return n;
}

void EventQueue::remove(Triangle* t){
	unsigned long long id = (*t).getID();
	struct Event *e, *prev = NULL;

	e = first;
	while(e != NULL){
		if(id == (*(e -> triangle)).getID()){
			if(prev != NULL)
				prev -> next = e -> next;
			else
				first = e -> next;

			free(e);
			n--;

			return;
		}

		prev = e;
		e = e -> next;
	}
}

void EventQueue::print(){
	struct Event* e;
	int i = 0;

	e = first;
	while(e != NULL){
		printf("element nr. %d: time = %f \n", i, e -> collapseTime);

		i++;
		e = e -> next;
	}
}

EventQueue::~EventQueue(){
	struct Event *prev, *e;
	Triangle *t;

	e = first;

	while(e != NULL){
		prev = e;
		e = prev -> next;

		t = prev -> triangle;
		(*t).dequeue();

		free(prev);
	}
}
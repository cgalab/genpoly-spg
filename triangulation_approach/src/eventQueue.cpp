#include "eventQueue.h"

/*
	P ~ R ~ I ~ V ~ A ~ T ~ E 	M ~ E ~ M ~ B ~ E ~ R 	F ~ U ~ N ~ C ~ T ~ I ~ O ~ N ~ S
*/

/*
	The function stabilize() checks and changes (if necessary) the ordering of two
	neighboring events in the event queue with very close event times. If the
	corresponding triangles of the events are adjacent it decides whether the opposite
	edges to the original vertex form a convex or a non-convex shape and call
	stabilizeConvex() resp. stabilizeNonConvex() to check the ordering of the events.
	In case the corresponding triangles are not adjacent, the exact order does not
	matter, so stabilize() does nothing.

	@param 	e0 	The event which is scheduled earlier in the event queue
	@param 	e1 	The event which is scheduled later in the event queue

	Note:
		It is required that the collapseTime of e0 is less than the collapseTime of e1
*/
// TODO:
// Check whether the stabilize() stuff does change anything
void EventQueue::stabilize(struct Event *e0, struct Event *e1){
	Triangle *t0, *t1, *t;
	double area0, area1;
	TEdge *edge;
	Vertex *common, *v0, *v1;

	t0 = e0 -> triangle;
	t1 = e1 -> triangle;

	// Check whether triangles are adjacent (share an edge):
	// As both triangles contain the vertex original, they have to share one of the two
	// edges incident to original
	edge = (*t0).getEdgeContaining(original);
	t = (*edge).getOtherTriangle(t0);

	// Triangles do not share the first edge
	if((*t).getID() != (*t1).getID()){
		edge = (*t0).getOtherEdgeContaining(original, edge);
		t = (*edge).getOtherTriangle(t0);

		// Triangles also don't share the second edge => triangles aren't adjacent
		// => the exact order doesn't matter
		if((*t).getID() != (*t1).getID())
			return;
	}

	// Get the second vertex the two triangles share
	common = (*edge).getOtherVertex(original);

	// Get the two not-shared vertices
	v0 = (*t0).getOtherVertex(edge);
	v1 = (*t1).getOtherVertex(edge);

	// Comment: The edge of a triangle which is not incident to the moving vertex (original)
	// is called the opposing edge

	// Find the right case:
	// 1. case:
	// Opposing edges in convex shape

	t = new Triangle(v0, v1, original);
	area0 = (*t).signedArea();
	delete t;

	t = new Triangle(v0, v1, common);
	area1 = (*t).signedArea();
	delete t;

	// We have convex shape if sign(area0) != sign(area1)
	// The special case area1 = 0 counts as non-convex shape

	if(area1 != 0 && (signbit(area0) != signbit(area1)))
		stabilizeConvex(e0, e1, edge);

	// 2. case:
	// Opposing edges in non-convex shape
	else
		stabilizeNonConvex(e0, e1, edge);
}

/*
	The function stabilizeConvex() checks for two events with adjacent triangles and
	convex shape of the opposing edges whether the events are in the right order and
	change if necessary. In case the events are in the wrong order the events
	themselfes are not changed, just the triangles are switched such that it still
	holds that the events in the event queue are order by their collapse times.
	For further information on the decision criterion for reordering take a look into
	my master thesis.

	@param 	e0 			The event which is scheduled earlier in the event queue
	@param 	e1 			The event which is scheduled later in the event queue
	@param 	commonE 	The edge which is shared by the triangles of e0 and e1

	Note:
		It is important not to change the events, but just switch their triangles,
		otherwise this might lead to an infinite loop in makeStable().	
*/
void EventQueue::stabilizeConvex(struct Event* e0, struct Event* e1, TEdge* commonE){
	Triangle *t0, *t1, *t;
	Vertex *common, *v0, *v1;
	double area0, area1, areaCommon;

	t0 = e0 -> triangle;
	t1 = e1 -> triangle;

	// Get the second vertex the two triangles share
	common = (*commonE).getOtherVertex(original);

	// Get the two not-shared vertices
	v0 = (*t0).getOtherVertex(commonE);
	v1 = (*t1).getOtherVertex(commonE);

	// Now we have to check whether the transition line intersects one of the
	// opposing edges or just goes by

	t = new Triangle(oldV, newV, v0);
	area0 = (*t).signedArea();
	delete t;

	t = new Triangle(oldV, newV, v1);
	area1 = (*t).signedArea();
	delete t;

	// Transition line doesn't intersect the opposing edges
	if(signbit(area0) == signbit(area1)){
		// Now we have to check on which side the transition line passes
		// The closer triangle collapses first

		// Therefore we search for the edge in the middle
		t = new Triangle(original, v0, newV);
		area0 = (*t).signedArea();
		delete t;

		t = new Triangle(original, v0, v1);
		area1 = (*t).signedArea();
		delete t;

		// If area0 and area1 have now different signs, then the edge from original
		// to v0 is in the middle, so the transition line is closer to t0, i.e. t0
		// collapses first and the initial order was right

		// Otherwise we have to switch t0 and t1
		if(signbit(area0) == signbit(area1)){
			e0 -> triangle = t1;
			e1 -> triangle = t0;

			if(Settings::correctionInfo)
				printf("Numerical correction: Order change for events with time diff: %.20f \n",
					fabs(e0 -> collapseTime - e1 -> collapseTime));
		}

	// Transition line intersects one of the oppossing edges
	}else{
		t = new Triangle(oldV, newV, common);
		areaCommon = (*t).signedArea();
		delete t;

		// If the transition line intersects t0, then t0 has to collapse first
		// so the ordering was right

		// If it intersects t1, the ordering must be changed
		if(signbit(area1) != signbit(areaCommon)){
			e0 -> triangle = t1;
			e1 -> triangle = t0;

			if(Settings::correctionInfo)
				printf("Numerical correction: Order change for events with time diff: %.20f \n",
					fabs(e0 -> collapseTime - e1 -> collapseTime));
		}
	}
}

/*
	The function stabilizeNonConvex() checks for two events with adjacent triangles
	and non-convex shape of the opposing edges whether the events are in the right
	order and change if necessary. In case the events are in the wrong order the
	events themselfes are not changed, just the triangles are switched such that it
	still holds that the events in the event queue are order by their collapse times.
	For further information on the decision criterion for reordering take a look into
	my master thesis.

	@param 	e0 			The event which is scheduled earlier in the event queue
	@param 	e1 			The event which is scheduled later in the event queue
	@param 	commonE 	The edge which is shared by the triangles of e0 and e1

	Note:
		It is important not to change the events, but just switch their triangles,
		otherwise this might lead to an infinite loop in makeStable()	
*/
void EventQueue::stabilizeNonConvex(struct Event *e0, struct Event *e1, TEdge *commonE){
	Triangle *t0, *t1, *t;
	Vertex *common, *v0, *v1;
	double area0, area1, areaCommon;

	t0 = e0 -> triangle;
	t1 = e1 -> triangle;

	// Get the second vertex the two triangles share
	common = (*commonE).getOtherVertex(original);

	// Get the two not-shared vertices
	v0 = (*t0).getOtherVertex(commonE);
	v1 = (*t1).getOtherVertex(commonE);

	// Now we have to check whether the transition line intersects one of the
	// opposing edges or just goes by

	t = new Triangle(oldV, newV, v0);
	area0 = (*t).signedArea();
	delete t;

	t = new Triangle(oldV, newV, v1);
	area1 = (*t).signedArea();
	delete t;

	// Transition line doesn't intersect the opposing edges
	if(signbit(area0) == signbit(area1)){
		// Now we have to check on which side the transition line passes
		// the closer triangle collapses last

		// Therefore we search for the edge in the middle
		t = new Triangle(original, v0, newV);
		area0 = (*t).signedArea();
		delete t;

		t = new Triangle(original, v0, v1);
		area1 = (*t).signedArea();
		delete t;

		// If area0 and area1 have now different signs, then the edge from original
		// to v0 is in the middle, so the transition line is closer to t0, i.e. t1
		// collapses first and the initial order was wrong

		// So we have to switch t0 and t1
		if(signbit(area0) != signbit(area1)){
			e0 -> triangle = t1;
			e1 -> triangle = t0;

			if(Settings::correctionInfo)
				printf("Numerical correction: Order change for events with time diff: %.20f \n",
					fabs(e0 -> collapseTime - e1 -> collapseTime));
		}

	// Transition line intersects one of the oppossing edges
	}else{
		
		t = new Triangle(oldV, newV, common);
		areaCommon = (*t).signedArea();
		delete t;

		// If the transition line intersects t1, then t0 has to collapse first
		// so the ordering was right

		// If it intersects t0, the ordering must be changed
		if(signbit(area0) != signbit(areaCommon)){
			e0 -> triangle = t1;
			e1 -> triangle = t0;

			if(Settings::correctionInfo)
				printf("Numerical correction: Order change for events with time diff: %.20f \n",
					fabs(e0 -> collapseTime - e1 -> collapseTime));
		}
	}
}


/*
	C ~ O ~ N ~ S ~ T ~ R ~ U ~ C ~ T ~ O ~ R ~ S
*/

/*
	Constructor:

	@param 	orig 	The vertex which is moved by the translation
	@param 	oV 		A copy of the vertex at its start position
	@param	nV 		A copy of the vertex at its target position
*/	
EventQueue::EventQueue(Vertex * const orig, Vertex * const oV, Vertex * const nV) : 
	first(NULL), n(0), original(orig), oldV(oV), newV(nV) {}


/*
	O ~ T ~ H ~ E ~ R ~ S
*/

/*
	The function insertWithoutCheck() creates a new event for the triangle t collapsing
	at time and inserts it into the list of events such that the list is still order by
	the collapse times of the events.

	@param 	time 	The collapse time for the new event
	@param 	t 		The triangle which will collapse at the new event

	Note:
		The ordering of the events will not be checked by this function!
*/
// TODO:
// Maybe add a function for inserting elements with checking
void EventQueue::insertWithoutCheck(const double time, Triangle *t){
	struct Event *e0;
	struct Event *e1, *prev = NULL;
	double time1;

	e1 = first;
	// Find the right position in the event queue
	while(e1 != NULL){
		time1 = e1 -> collapseTime;

		if(time < time1){
			// Insert new element
			e0 = (struct Event*) malloc(sizeof(struct Event));

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

		e1 = e1 -> next;
	}

	// Insert at the end
	if(e1 == NULL){
		// Insert new element
		e0 = (struct Event*) malloc(sizeof(struct Event));

		e0 -> collapseTime = time;
		e0 -> triangle = t;
		e0 -> next = NULL;

		// Check whether there was already an element in the queue
		if(prev != NULL){
			e0 -> next = prev -> next;
			prev -> next = e0;
		}else
			first = e0;
	}

	n++;
}

/*
	The function makeStable() iterates over the whole event queue and searches for
	events closer then Settings::epsEventTime which are then considered as concurrent.
	Two concurrent events then get checked and potentially reorder by the function
	stabilize(). If it finds three or more neighboring events which are pairwise
	concurrent, it returns false and the translation will be arborted.

	@param 	initial 	Indicates whether the event queue is the initial event queue
						or not, which leads to different outputs in case the
						translation gets aborted.
	@return 			True if there are not more than two neighboring concurrent
						events, otherwise false
*/
bool EventQueue::makeStable(const bool initial){
	struct Event *e0, *e1;
	double time0, time1, dif;

	if(n == 0)
		return true;

	e0 = first;
	time0 = e0 -> collapseTime;
	while(e0 -> next != NULL){
		e1 = e0 -> next;

		time1 = e1 -> collapseTime;
		dif = fabs(time1 - time0);

		// Two events are concurrent
		if(dif < Settings::epsEventTime){
			// Check first whether there is a third concurrent event

			// TODO:
			// Definitelly the changing of event orders is not stable at the moment
			/*if(e1 -> next != NULL){
				dif = fabs(time1 - e1 -> next -> collapseTime);
				if(dif < Settings::epsEventTime){
					if(initial)
						printf("Eventqueue: More than two events at the same time -> refused translation\n");
					else
						printf("Eventqueue: More than two events at the same time -> aborted translation\n");
					return false;
				}
			}*/

			if(Settings::correctionInfo){
				if(initial)
					printf("Eventqueue: Two events at the same time -> refused translation\n");
				else
					printf("Eventqueue: Two events at the same time -> aborted translation\n");
			}
			
			return false;
			
			// Attentione: do not change the ordering of e0 and e1 in stabilize, otherwise you would
			// get an infinite loop here!
			stabilize(e0, e1);
		}

		e0 = e1;
		time0 = time1;
	}

	return true;
}

/*
	The function pop() returns the element on top of the event queue and removes it.

	@return 	The element on top of the event queue
*/
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

/*
	@return 	The actual number of events in the event queue
*/
int EventQueue::size() const{
	return n;
}

/*
	The function remove() searches for an event containing the triangle t in the event
	queue and deletes it. If no event with this triangle exists in the event queue it
	simple does nothing.

	@param 	t 	The triangle to be deleted
*/
void EventQueue::remove(Triangle * const t){
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

/*
	The function print() prints all event times in an ordered way to stdout.
*/
void EventQueue::print() const{
	struct Event *e;
	int i = 0;

	e = first;
	while(e != NULL){
		printf("Element nr. %d: time = %f \n", i, e -> collapseTime);

		i++;
		e = e -> next;
	}
}

/*
	D ~ E ~ S ~ T ~ R ~ U ~ C ~ T ~ O ~ R
*/

/*
	Destructor:
	The destructor frees up all memory of still existing events and sets the enqueued
	flag of the remaining triangles back to false.

	Note:
		If the translation is aborted the event queue might not be empty in the end, so
		it is necessary to use the destructor.
*/
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
#include "eventQueue.h"

EventQueue::EventQueue(double eps){
	first = NULL;
	n = 0;

	epsilon = eps;
}

bool EventQueue::insert(double time, Triangle* t){
	struct element* e0;
	struct element* e1, *prev = NULL;
	double time1, prevTime = -100;

	e1 = first;
	while(e1 != NULL){
		time1 = e1 -> collapseTime;

		if(time < time1){
			// check for numerical stability
			if(time + epsilon >= time1){
				printf("Eventqueue: prevented numerical stability\n");
				return false;
			}

			if(time - epsilon <= prevTime){
				printf("Eventqueue: prevented numerical stability\n");
				return false;
			}

			// insert new element
			e0 = (struct element*)malloc(sizeof(struct element));

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
			printf("Eventqueue: prevented numerical stability\n");
			return false;
		}

		// insert new element
		e0 = (struct element*)malloc(sizeof(struct element));

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
}

int EventQueue::size(){
	return n;
}

std::pair<double, Triangle*> EventQueue::pop(){
	struct element* e;
	std::pair<double, Triangle*> out;

	e = first;

	first = e -> next;

	out = std::make_pair(e -> collapseTime, e -> triangle);

	free(e);
	n--;

	return out;
}

void EventQueue::remove(Triangle* t){
	unsigned long long id = (*t).getID();
	struct element *e, *prev = NULL;

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
	struct element* e;
	int i = 0;

	e = first;
	while(e != NULL){
		printf("element nr. %d: time = %f \n", i, e -> collapseTime);

		i++;
		e = e -> next;
	}
}

// TODO: implement destructor which deletes the remaining elements if the execution is aborted
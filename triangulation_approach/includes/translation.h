#include "vertex.h"
#include "triangle.h"
#include "tedge.h"
#include "triangulation.h"
#include "customPriorityQueue.h"
#include <list>

class Translation{

private:
	Triangulation* T;

	Vertex* original;
	Vertex* oldV;
	Vertex* newV;
	Vertex* prevV;
	Vertex* nextV;

	TEdge* transPath;

	int index;

	double dx, dy;

	double actualTime;
	double maxTime;
	bool split;
	custom_priority_queue Q;

	void generateInitialQueue(){
		double t;
		std::list<Triangle*> triangles = (*original).getTriangles();

		for(auto& i : triangles){
			t = (*i).calculateCollapseTime(original, dx, dy);

			if(t >= 0 && t <= 1){
				(*i).enqueue();
				Q.push(std::make_pair(t, i));
			}
		}

		//Q.check();
	}

public:
	Translation(Triangulation* Tr, int i, double dX, double dY){
		T = Tr;
		index = i;
		actualTime = 0;
		split = false;

		dx = dX;
		dy = dY;

		original = (*T).getVertex(index);
		prevV = (*T).getPVertex(index - 1);
		nextV = (*T).getPVertex(index + 1);

		oldV = (*original).getTranslated(0, 0);
		newV = (*original).getTranslated(dx, dy);

		transPath = new TEdge(oldV, newV);

		maxTime = (*transPath).length();
	}

	Vertex* getOriginal(){ return original;}
	Vertex* getOldV(){ return oldV;}
	Vertex* getNewV(){ return newV;}
	Vertex* getPrevV(){ return prevV;}
	Vertex* getNextV(){ return nextV;}

	void setSplit(){ split = true;}

	TEdge* getTranslationPath(){ return transPath;}

	void execute(){
		Triangle* t = NULL;
		std::pair<double, Triangle*> e;
		Translation* trans;
		double middleX, middleY, transX, transY;

		// ATTENTIONE: just works, if the chosen vertex changes its side relative to the edge between its neighbors or at least the triangle with its neighbors exist
		if(split){
			// get translation to end position for first translation which is the middle between the nieghboring vertices
			middleX = ((*prevV).getX() + (*nextV).getX()) / 2;
			middleY = ((*prevV).getY() + (*nextV).getY()) / 2;
			// compute translation vector
			transX = middleX - (*oldV).getX();
			transY = middleY - (*oldV).getY();

			trans = new Translation(T, index, transX, transY);
			(*trans).execute();

			delete trans;

			// get translation from middle to end
			transX = (*newV).getX() - (*original).getX();
			transY = (*newV).getY() - (*original).getY();

			trans = new Translation(T, index, transX, transY);
			(*trans).execute();

			delete trans;

		}else{
			generateInitialQueue();

			while(!Q.empty()){
				e = Q.top();
				Q.pop();
				actualTime = e.first;
				t = e.second;

				flip(t);
			}

			(*original).setPosition((*oldV).getX() + dx, (*oldV).getY() + dy);
		}
	}

	void flip(Triangle* t0){
		Triangle *t1;
		Vertex *vj0, *vj1; // joint vertices
		Vertex *vn0, *vn1; // non joint vertices
		TEdge *e, *e1, *e2;
		double t;

		// move vertex to event time
		(*original).setPosition((*oldV).getX() + dx * actualTime, (*oldV).getY() + dy * actualTime);


		e = (*t0).getLongestEdge();
		t1 =(*e).getOtherTriangle(t0); // TODO: take care, whether other triangle must be removed from queue
		if((*t1).isEnqueued())
			Q.remove(t1);

		vj0 = (*e).getV1();
		vj1 = (*e).getV2();
		vn0 = (*t0).getOtherVertex(e);
		vn1 = (*t1).getOtherVertex(e);

		delete e;

		// new triangle vn0, vn1, vj0
		e = new TEdge(vn0, vn1);
		(*T).addEdge(e);

		e1 = (*vj0).getEdgeTo(vn0);
		e2 = (*vj0).getEdgeTo(vn1);

		t0 = new Triangle(e, e1, e2, vn0, vn1, vj0);

		// new triangle vn0, vn1, vj1
		(*T).addEdge(e);

		e1 = (*vj1).getEdgeTo(vn0);
		e2 = (*vj1).getEdgeTo(vn1);

		t1 = new Triangle(e, e1, e2, vn0, vn1, vj1);

		// add new triangles to queue if necessary
		t = (*t0).calculateCollapseTime(original, dx, dy); // again between 0 and 1 but 0 is now the acutal time
		t = t + actualTime;

		if(t >= actualTime && t <= 1){
			(*t0).enqueue();
			Q.push(std::make_pair(t, t0));
		}

		t = (*t1).calculateCollapseTime(original, dx, dy); // again between 0 and 1 but 0 is now the acutal time
		t = t + actualTime;

		if(t >= actualTime && t <= 1){
			(*t1).enqueue();
			Q.push(std::make_pair(t, t1));
		}
	}

	~Translation(){
		delete transPath;
		delete oldV;
		delete newV;
	}

};
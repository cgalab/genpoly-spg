#include "vertex.h"
#include "triangle.h"
#include "tedge.h"
#include "triangulation.h"
#include "customPriorityQueue.h"
#include <list>

class Translation{

private:
	Triangulation* T;

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
		std::list<Triangle*> triangles = (*oldV).getTriangles();

		for(auto& i : triangles){
			t = (*i).calculateCollapseTime(oldV, dx, dy);

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

		oldV = (*T).getVertex(index);
		prevV = (*T).getPVertex(index - 1);
		nextV = (*T).getPVertex(index + 1);

		newV = (*oldV).getTranslated(dx, dy);

		transPath = new TEdge(oldV, newV);

		maxTime = (*transPath).length();
	}

	Vertex* getOldV(){ return oldV;}
	Vertex* getNewV(){ return newV;}
	Vertex* getPrevV(){ return prevV;}
	Vertex* getNextV(){ return nextV;}

	void setSplit(){ split = true;}

	TEdge* getTranslationPath(){ return transPath;}

	void execute(){
		Triangle* t;
		std::pair<double, Triangle*> e;

		if(split){

		}else{
			generateInitialQueue();

			while(!Q.empty()){
				e = Q.top();
				Q.pop();
				actualTime = e.first;
				t = e.second;

				flip(t);
			}
		}
	}

	void flip(Triangle* t1){
		TEdge* e = (*t1).getLongestEdge();
		// remove old triangles and insert new ones
		// attention: shift old point to new one and don't forget to add the actual time to the triangle collapse time of the new triangles!
	}

};
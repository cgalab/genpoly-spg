#include "vertex.h"
#include "triangle.h"
#include "tedge.h"
#include "triangulation.h"
#include "customPriorityQueue.h"
#include <list>
#include <math.h>

class Translation{

private:
	Triangulation* T;

	Vertex* original;
	Vertex* oldV;
	Vertex* newV;
	Vertex* prevV;
	Vertex* nextV;

	TEdge* transPath;
	TEdge* prevOldE;
	TEdge* nextOldE;
	TEdge* prevNewE;
	TEdge* nextNewE;

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

	double signedArea(Vertex* v0, Vertex* v1, Vertex* v2){
		double area;
		double ax, ay, bx, by, cx, cy;

		ax = (*v0).getX();
		ay = (*v0).getY();

		bx = (*v1).getX();
		by = (*v1).getY();

		cx = (*v2).getX();
		cy = (*v2).getY();

		area = 0.5 * (- ay * bx + ax * by + ay * cx - by * cx - ax * cy + bx * cy);

		return area;
	}

	bool insideQuadrilateral(Vertex* v, Vertex* q0, Vertex* q1, Vertex* q2, Vertex* q3){
		double a0, a1, a2, a3;
		bool inside;

		a0 = signedArea(q0, q1, v);
		a1 = signedArea(q1, q2, v);
		a2 = signedArea(q2, q3, v);
		a3 = signedArea(q3, q0, v);

		inside = (signbit(a0) == signbit(a1));
		inside = inside && (signbit(a0) == signbit(a2));
		inside = inside && (signbit(a0) == signbit(a3));

		return inside;
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

		prevOldE = (*original).getEdgeTo(prevV);
		nextOldE = (*original).getEdgeTo(nextV);

		prevNewE = new TEdge(prevV, newV);
		nextNewE = new TEdge(newV, nextV);

		maxTime = (*transPath).length();
	}

	Vertex* getOriginal(){ return original;}
	Vertex* getOldV(){ return oldV;}
	Vertex* getNewV(){ return newV;}
	Vertex* getPrevV(){ return prevV;}
	Vertex* getNextV(){ return nextV;}

	void setSplit(){ split = true;}

	TEdge* getTranslationPath(){ return transPath;}

	bool checkOverroll(){
		bool simple;
		Vertex* randomV;

		// check whether the quadrilateral of the chosen Vertex P, its translated version P' and the two neighbors M and N is simple
		// otherwise there can not be any overroll
		simple = !(checkIntersection(prevOldE, nextNewE) || checkIntersection(nextOldE, prevNewE));

		if(!simple) return false;

		// check for the next vertex whether its inside the quadrilateral
		randomV = (*T).getPVertex(index - 2);

		simple = insideQuadrilateral(randomV, oldV, nextV, newV, prevV);

		// check also for a second vertex to increase the chance the reject non-simple translation
		randomV = (*T).getPVertex(index + 2);

		simple = simple || insideQuadrilateral(randomV, oldV, nextV, newV, prevV);

		//if(simple) printf("Potential overroll detected! \n");

		return simple;
	}

	void execute(){
		Triangle* t = NULL;
		std::pair<double, Triangle*> e;
		Translation* trans;
		double middleX, middleY, transX, transY, oldArea, newArea;
		TEdge* edge;

		// ATTENTIONE: just works, if the chosen vertex changes its side relative to the edge between its neighbors or at least the triangle with its neighbors exist
		if(split){
			oldArea = signedArea(prevV, nextV, oldV);
			newArea = signedArea(prevV, nextV, newV);

			// vertex stays on the same side of the edge between the neigboring vertices
			if(signbit(oldArea) == signbit(newArea)){

			// vertex changes side
			}else{
				// get translation to end position for first translation which is the middle between the neighboring vertices
				middleX = ((*prevV).getX() + (*nextV).getX()) / 2;
				middleY = ((*prevV).getY() + (*nextV).getY()) / 2;
				// compute translation vector
				transX = middleX - (*oldV).getX();
				transY = middleY - (*oldV).getY();

				trans = new Translation(T, index, transX, transY);
				(*trans).execute();

				delete trans;

				// for numerical reasons its possible that the triangle of the old vertex and the neighboring vertices doesn't vanish at the time when the vertex arrives between its neighbors
				// therefore this must be checked and corrected before starting the second translation
				edge = (*prevV).getEdgeTo(nextV);
				if(edge != NULL){
					t = (*edge).getTriangleContaining(original);
					flip(t, true);
				}
				

				// get translation from middle to end
				transX = (*newV).getX() - (*original).getX();
				transY = (*newV).getY() - (*original).getY();

				trans = new Translation(T, index, transX, transY);
				(*trans).execute();

				delete trans;
			}

		}else{
			generateInitialQueue();

			while(!Q.empty()){
				e = Q.top();
				Q.pop();
				actualTime = e.first;
				t = e.second;

				flip(t, false);
			}

			(*original).setPosition((*oldV).getX() + dx, (*oldV).getY() + dy);
		}
	}

	void flip(Triangle* t0, bool singleFlip){
		Triangle *t1;
		Vertex *vj0, *vj1; // joint vertices
		Vertex *vn0, *vn1; // non joint vertices
		TEdge *e, *e1, *e2;
		double t;

		// move vertex to event time
		if(!singleFlip) (*original).setPosition((*oldV).getX() + dx * actualTime, (*oldV).getY() + dy * actualTime);


		e = (*t0).getLongestEdge();
		if((*e).getEdgeType() == EdgeType::POLYGON){
			printf("attention: polygon edge gets deleted :O \n");
			exit(1);
		}
		t1 =(*e).getOtherTriangle(t0);
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
		if(!singleFlip){
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
	}

	~Translation(){
		delete transPath;
		delete oldV;
		delete newV;
	}

};
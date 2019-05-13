#include "translation.h"

// Constructors
Translation::Translation(Triangulation* Tr, int i, double dX, double dY){
	T = Tr;
	index = i;
	actualTime = 0;
	split = false;

	dx = dX;
	dy = dY;

	original = (*T).getVertex(index);
	
	prevV = (*original).getPrev();
	nextV = (*original).getNext();

	oldV = (*original).getTranslated(0, 0);
	newV = (*original).getTranslated(dx, dy);

	transPath = new TEdge(oldV, newV);

	prevOldE = (*original).getToPrev();
	nextOldE = (*original).getToNext();

	prevNewE = new TEdge(prevV, newV);
	nextNewE = new TEdge(newV, nextV);
}

void Translation::generateInitialQueue(){
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

double Translation::signedArea(Vertex* v0, Vertex* v1, Vertex* v2){
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

bool Translation::insideQuadrilateral(Vertex* v, Vertex* q0, Vertex* q1, Vertex* q2, Vertex* q3){
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

bool Translation::checkEdge(Vertex* fromV, TEdge* newE){
	std::vector<TEdge*> surEdges;
	enum intersect_t iType = intersect_t::IS_FALSE;
	EdgeType eType;
	Triangle* nextT = NULL;
	surEdges = (*fromV).getSurroundingEdges();
	Triangle* nexT;

	// start with the edge to the previous vertex
	for(auto& i : surEdges){
		iType = checkIntersection(newE, i);

		// new edge hits vertex of surrounding polygon
		if(iType > intersect_t::IS_FALSE && iType <= intersect_t::IS_VERTEX22) return false;
		// intersection with edge of surrounding polygon
		else if(iType == intersect_t::IS_TRUE){
			eType = (*i).getEdgeType();

			// intersected edge is polygon edge
			if(eType == EdgeType::POLYGON){
				//printf("edge hit polygon edge from vertex %d to vertex %d \n", (*(*i).getV1()).getID(), (*(*i).getV2()).getID());
				return false;
			// intersected edge is frame edge
			}else if(eType == EdgeType::FRAME){
				//printf("edge hit frame edge from vertex %d to vertex %d \n", (*(*i).getV1()).getID(), (*(*i).getV2()).getID());
				return false;
			// intersected edge is just a triangulation edge
			}else{
				//printf("edge hit triangulation edge from vertex %d to vertex %d \n", (*(*i).getV1()).getID(), (*(*i).getV2()).getID());
				nextT = (*i).getTriangleNotContaining(fromV);
				surEdges = (*nextT).getOtherEdges(i);
				break;
			}
		}
	}

	// iterate over the adjacent triangles it there was an intersection with a triangulation edge
	while(!(iType == intersect_t::IS_FALSE)){

		for(auto& i : surEdges){

			iType = checkIntersection(newE, i);

			// new edge hits vertex of surrounding polygon
			if(iType > intersect_t::IS_FALSE && iType <= intersect_t::IS_VERTEX22) return false;
			// intersection with edge of surrounding polygon
			else if(iType == intersect_t::IS_TRUE){
				eType = (*i).getEdgeType();

				// intersected edge is polygon edge
				if(eType == EdgeType::POLYGON){
					//printf("edge hit polygon edge from vertex %d to vertex %d \n", (*(*i).getV1()).getID(), (*(*i).getV2()).getID());
					return false;
				// intersected edge is frame edge
				}else if(eType == EdgeType::FRAME){
					//printf("edge hit frame edge from vertex %d to vertex %d \n", (*(*i).getV1()).getID(), (*(*i).getV2()).getID());
					return false;
				// intersected edge is just a triangulation edge
				}else{
					//printf("edge hit triangulation edge from vertex %d to vertex %d \n", (*(*i).getV1()).getID(), (*(*i).getV2()).getID());
					nextT = (*i).getOtherTriangle(nextT);
					surEdges = (*nextT).getOtherEdges(i);
					break;
				}
			}
		}
	}

	return true;
}

// public member functions

bool Translation::checkOverroll(){
	bool simple;
	Vertex* randomV;

	// check whether the quadrilateral of the chosen Vertex P, its translated version P' and the two neighbors M and N is simple
	// otherwise there can not be any overroll
	simple = !(checkIntersection(prevOldE, nextNewE) || checkIntersection(nextOldE, prevNewE));

	if(!simple) return false;

	// check for the next vertex whether its inside the quadrilateral
	randomV = (*T).getVertex(index - 2);

	simple = insideQuadrilateral(randomV, oldV, nextV, newV, prevV);

	// check also for a second vertex to increase the chance the reject non-simple translation
	randomV = (*T).getVertex(index + 2);

	simple = simple || insideQuadrilateral(randomV, oldV, nextV, newV, prevV);

	//if(simple) printf("Potential overroll detected! \n");

	return simple;
}

void Translation::execute(){
	Triangle* t = NULL;
	std::pair<double, Triangle*> e;
	Translation* trans;
	double middleX, middleY, transX, transY, oldArea, newArea, area;
	TEdge* edge;
	Vertex* intersectionPoint;
	std::list<Triangle*> triangles;

	if(split){
		oldArea = signedArea(prevV, nextV, oldV);
		newArea = signedArea(prevV, nextV, newV);

		// vertex stays on the same side of the edge between the neigboring vertices
		if(signbit(oldArea) == signbit(newArea)){
				
			// compute the intersection point to split the translation
			intersectionPoint = getIntersectionPoint(prevV, oldV, nextV, newV);
			if(intersectionPoint == NULL)
				intersectionPoint = getIntersectionPoint(nextV, oldV, prevV, newV);
			if(intersectionPoint == NULL){
				printf("something went wrong computing the intersection point to split the translation \n");
				printf("index: %d dx: %f dy: %f \n", index, dx, dy);
				(*T).addVertex(newV);
				(*T).print("triangulation.graphml");
				exit(1);
			}

			// first part of the translation
			transX = (*intersectionPoint).getX() - (*oldV).getX();
			transY = (*intersectionPoint).getY() - (*oldV).getY();

			trans = new Translation(T, index, transX, transY);
			(*trans).execute();

			delete trans;

			// second part of the translation
			transX = (*newV).getX() - (*original).getX();
			transY = (*newV).getY() - (*original).getY();

			trans = new Translation(T, index, transX, transY);
			(*trans).execute();

			delete trans;
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

		//(*original).setPosition((*oldV).getX() + dx, (*oldV).getY() + dy);
		(*original).setPosition((*newV).getX(), (*newV).getY());

		// TODO: check after translation whether each triangle still exists

		triangles = (*original).getTriangles();

		for(auto& i : triangles){
			area = (*i).signedArea();

			if(area == 0){
				//printf("numerical error! \n");
				edge = (*i).getLongestEdge();
				if((*edge).getEdgeType() != EdgeType::POLYGON)
					flip(i, true);
				else
					printf("longest edge is PE\n");
				//printf("corrected! \n");
			}
		}
	}
}

void Translation::flip(Triangle* t0, bool singleFlip){
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
		printf("id: %d dx: %f dy: %f \n", (*original).getID(), dx, dy);
		(*T).addVertex(newV);
		(*T).print("triangulation.graphml");
		(*original).printEnvironment(3, "env.graphml");
		(*T).check();
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

bool Translation::checkSimplicityOfTranslation(){
	bool simple;

	(*prevOldE).setEdgeType(EdgeType::TRIANGULATION);
	(*nextOldE).setEdgeType(EdgeType::TRIANGULATION);

	simple = checkEdge(prevV, prevNewE);
	simple = simple && checkEdge(nextV, nextNewE);

	(*prevOldE).setEdgeType(EdgeType::POLYGON);
	(*nextOldE).setEdgeType(EdgeType::POLYGON);

	return simple;
}

void Translation::checkSplit(){
	split = !checkEdge(original, transPath);
}
	
Translation::~Translation(){
	delete transPath;
	delete oldV;
	delete newV;
}
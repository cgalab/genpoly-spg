#include "translation.h"

// Constructors
Translation::Translation(Triangulation* Tr, Settings &set, int i, double dX, double dY) : 
Q(EventQueue(epsilon)), dx(dX), dy(dY), index(i), T(Tr), actualTime(0), split(false), settings(set)

{
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

bool Translation::generateInitialQueue(){
	double t;
	std::list<Triangle*> triangles = (*original).getTriangles();
	bool ok = true;


	for(auto& i : triangles){
		t = (*i).calculateCollapseTime(original, dx, dy);

		if(t <= epsilon && t >= - epsilon)
			return false;

		if(t >= 1 - epsilon && t <= 1 + epsilon)
			return false;

		if(t >= 0 && t <= 1){
			(*i).enqueue();		

			if(!Q.insert(t, i))
				return false;
		}
	}

	return true;
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

// https://www.geeksforgeeks.org/how-to-check-if-a-given-point-lies-inside-a-polygon/
bool Translation::insideQuadrilateral(Vertex* v){
	Vertex* dummyVertex;
	TEdge* dummyEdge;
	double maxX, x;
	int count = 0;
	IntersectionType intersection;

	// find maximum x value
	maxX = (*oldV).getX();
	x = (*newV).getX();
	if(x > maxX)
		maxX = x;
	x = (*prevV).getX();
	if(x > maxX)
		maxX = x;
	x = (*nextV).getX();
	if(x > maxX)
		maxX = x;

	if((*v).getX() > maxX)
		return false;
	maxX = maxX + 10;

	dummyVertex = new Vertex(maxX, (*v).getY());
	dummyEdge = new TEdge(v, dummyVertex);

	intersection = checkIntersection(dummyEdge, prevOldE);
	if(intersection != IntersectionType::NONE)
		count++;
	intersection = checkIntersection(dummyEdge, nextOldE);
	if(intersection != IntersectionType::NONE)
		count++;
	intersection = checkIntersection(dummyEdge, prevNewE);
	if(intersection != IntersectionType::NONE)
		count++;
	intersection = checkIntersection(dummyEdge, nextNewE);
	if(intersection != IntersectionType::NONE)
		count++;

	delete dummyEdge;
	delete dummyVertex;

	if(count % 2 == 1)
		return true;
	else
		return false;
}

bool Translation::checkEdge(Vertex* fromV, TEdge* newE){
	std::vector<TEdge*> surEdges;
	enum IntersectionType iType = IntersectionType::NONE;
	enum IntersectionType iType0, iType1;
	TEdge* intersectedE = NULL;
	EdgeType eType;
	Triangle* nextT = NULL;
	int count = 0;

	(*fromV).checkSurroundingPolygonFast();

	surEdges = (*fromV).getSurroundingEdges();

	// iterate over all edges of the surrounding polygon
	for(auto& i : surEdges){
		iType = checkIntersection(newE, i);

		// new edge hits vertex of surrounding polygon
		if(iType == IntersectionType::VERTEX)
			return false;

		// count intersections to detect numerical errors
		if(iType == IntersectionType::EDGE){
			count++;
			intersectedE = i;
		}	
	}

	// no intersection -> the new vertex stays in the surrounding polygon
	if(count == 0){
		return true;
	// multiple intersections -> numerical error
	}else if(count > 1){
		if(settings.getFBMode() == FeedbackMode::VERBOSE)
			printf("CheckEdge: new edge intersects multiple edges of the surrounding polygon -> translation rejected due to numerical problem\n");
		return false;
	}

	// one real intersection with an edge of the surrounding polygon
	eType = (*intersectedE).getEdgeType();

	// intersected edge is polygon edge
	if(eType == EdgeType::POLYGON){
		return false;
	// intersected edge is frame edge
	}else if(eType == EdgeType::FRAME){
		return false;
	// intersected edge is just a triangulation edge
	}else{
		nextT = (*intersectedE).getTriangleNotContaining(fromV);
		surEdges = (*nextT).getOtherEdges(intersectedE);
	}
	
	// iterate over the adjacent triangles if there was an intersection with a triangulation edge
	// here surEdges always have the length 2
	while(true){
		iType0 = checkIntersection(newE, surEdges[0]);
		iType1 = checkIntersection(newE, surEdges[1]);

		// the new edge doesn't interesect any further edges
		if(iType0 == IntersectionType::NONE && iType1 == IntersectionType::NONE)
			return true;

		// new edge hits vertex of surrounding polygon
		if(iType0 == IntersectionType::VERTEX) 
			return false;
		if(iType1 == IntersectionType::VERTEX) 
			return false;

		// check for numerical problems
		if(iType0 != IntersectionType::NONE && iType1 != IntersectionType::NONE){
			if(settings.getFBMode() == FeedbackMode::VERBOSE)
				printf("CheckEdge: new edge intersects multiple edges of the actual triangle -> translation rejected due to numerical problem\n");
			return false;
		}

		// chose the intersected edge
		if(iType0 != IntersectionType::NONE){
			iType = iType0;
			intersectedE = surEdges[0];
		}else{
			iType = iType1;
			intersectedE = surEdges[1];
		}

		// intersection with edge of surrounding polygon
		eType = (*intersectedE).getEdgeType();

		// intersected edge is polygon edge
		if(eType == EdgeType::POLYGON){
			return false;
		// intersected edge is frame edge
		}else if(eType == EdgeType::FRAME){
			return false;
		// intersected edge is just a triangulation edge
		}else{
			nextT = (*intersectedE).getOtherTriangle(nextT);
			surEdges = (*nextT).getOtherEdges(intersectedE);
		}	
	}

	return true;
}

// public member functions

bool Translation::checkOverroll(){
	bool overroll;
	Vertex* randomV;

	// check whether the quadrilateral of the chosen Vertex P, its translated version P' and the two neighbors M and N is simple
	// otherwise there can not be any overroll
	overroll = !(checkIntersection(prevOldE, nextNewE) != IntersectionType::NONE || checkIntersection(nextOldE, prevNewE) != IntersectionType::NONE);

	if(!overroll)
		return false;

	// check for the next vertex whether its inside the quadrilateral
	randomV = (*T).getVertex(index - 2);

	overroll = insideQuadrilateral(randomV);

	// check also for a second vertex to increase the chance the reject non-simple translation
	randomV = (*T).getVertex(index + 2);

	overroll = overroll || insideQuadrilateral(randomV);

	//if(simple) printf("Potential overroll detected! \n");

	return overroll;
}

enum Executed Translation::execute(){
	Triangle* t = NULL;
	std::pair<double, Triangle*> e;
	Translation* trans;
	double middleX, middleY, transX, transY, oldArea, newArea, area;
	TEdge* edge;
	Vertex* intersectionPoint;
	std::list<Triangle*> triangles;
	enum Executed ex;

	if(split){
		oldArea = signedArea(prevV, nextV, oldV);
		newArea = signedArea(prevV, nextV, newV);

		// vertex stays on the same side of the edge between the neigboring vertices
		if(signbit(oldArea) == signbit(newArea)){
			
			// TODO: maybe the intersection point computation causes parts of the trouble	
			// compute the intersection point to split the translation
			intersectionPoint = getIntersectionPoint(prevV, oldV, nextV, newV);
			if(intersectionPoint == NULL)
				intersectionPoint = getIntersectionPoint(nextV, oldV, prevV, newV);
			if(intersectionPoint == NULL){
				/*printf("something went wrong computing the intersection point to split the translation \n");
				printf("index: %d dx: %f dy: %f \n", index, dx, dy);
				(*T).addVertex(newV);
				(*T).print("triangulation.graphml");
				exit(1);*/
				return Executed::REJECTED;
			}

			// first part of the translation
			transX = (*intersectionPoint).getX() - (*oldV).getX();
			transY = (*intersectionPoint).getY() - (*oldV).getY();

			trans = new Translation(T, settings, index, transX, transY);
			ex = (*trans).execute();

			delete trans;

			if(ex != Executed::FULL)
				return ex;

			// second part of the translation
			transX = (*newV).getX() - (*original).getX();
			transY = (*newV).getY() - (*original).getY();

			trans = new Translation(T, settings, index, transX, transY);
			ex = (*trans).execute();

			delete trans;

			if(ex == Executed::FULL)
				return ex;
			else
				return Executed::PARTIAL;
		// vertex changes side
		}else{
			// get translation to end position for first translation which is the middle between the neighboring vertices
			middleX = ((*prevV).getX() + (*nextV).getX()) / 2;
			middleY = ((*prevV).getY() + (*nextV).getY()) / 2;
			// compute translation vector
			transX = middleX - (*oldV).getX();
			transY = middleY - (*oldV).getY();

			trans = new Translation(T, settings, index, transX, transY);
			ex = (*trans).execute();

			delete trans;

			if(ex != Executed::FULL)
				return ex;

			// for numerical reasons its possible that the triangle of the old vertex and the neighboring vertices doesn't vanish at the time when the vertex arrives between its neighbors
			// therefore this must be checked and corrected before starting the second translation
			edge = (*prevV).getEdgeTo(nextV);
			if(edge != NULL){
				t = (*edge).getTriangleContaining(original);
				flip(t, true);
				//printf("did a security flip\n");
			}				

			// get translation from middle to end
			transX = (*newV).getX() - (*original).getX();
			transY = (*newV).getY() - (*original).getY();

			trans = new Translation(T, settings, index, transX, transY);
			ex = (*trans).execute();

			delete trans;

			if(ex == Executed::FULL)
				return ex;
			else
				return Executed::PARTIAL;
		}

	}else{
		if(!generateInitialQueue())
			return Executed::REJECTED;

		while(Q.size() > 0){
			e = Q.pop();
			actualTime = e.first;
			t = e.second;

			if(!flip(t, false))
				return Executed::PARTIAL;
		}

		//(*original).setPosition((*oldV).getX() + dx, (*oldV).getY() + dy);
		(*original).setPosition((*newV).getX(), (*newV).getY());

		// check after translation whether any of the still existing triangles has the area 0
		// this is not good enough to avoid all security flips for splited translations!
		triangles = (*original).getTriangles();

		for(auto& i : triangles){
			area = (*i).signedArea();

			if(area == 0){
				if(settings.getFBMode() == FeedbackMode::VERBOSE)
					printf("Translation: Triangle area = 0 after translation...");

				edge = (*i).getLongestEdgeAlt();
				if((*edge).getEdgeType() != EdgeType::POLYGON)
					flip(i, true);
				else{
					/*printf("\nTriangle area = 0 after translation: PE can not be fliped\n");
					printf("index: %d id: %llu dx: %f dy: %f\n", index, (*original).getID(), dx, dy);
					(*i).print();
					(*(*i).getVertex(0)).print();
					(*(*i).getVertex(1)).print();
					(*(*i).getVertex(2)).print();
					printf("oldv\n");
					(*oldV).print();
					printf("newV\n");
					(*newV).print();
					exit(2);*/

					trans = new Translation(T, settings, index, - dx * 0.1, - dy * 0.1);
					ex = (*trans).execute();
					delete trans;

					if(ex == Executed::REJECTED){
						printf("\nTriangle area = 0 after translation: PE can not be fliped\n");
						exit(2);
					}
				}

				if(settings.getFBMode() == FeedbackMode::VERBOSE)
					printf("corrected! \n");
			}
		}

		return Executed::FULL;
	}
}

bool Translation::flip(Triangle* t0, bool singleFlip){
	Triangle *t1;
	Vertex *vj0, *vj1; // joint vertices
	Vertex *vn0, *vn1; // non joint vertices
	TEdge *e, *e1, *e2;
	double t;
	double x, y;
	bool ok = true;
	bool stable = true;

	// move vertex to event time
	if(!singleFlip) (*original).setPosition((*oldV).getX() + dx * actualTime, (*oldV).getY() + dy * actualTime);


	e = (*t0).getLongestEdgeAlt();
	if((*e).getEdgeType() == EdgeType::POLYGON){
		printf("Flip: polygon edge gets deleted\n");
		printf("id: %llu index: %d dx: %f dy: %f \n", (*original).getID(), index, dx, dy);

		(*T).addVertex(oldV);
		(*T).addVertex(newV);
		(*T).addEdge(transPath);
		(*original).printEnvironment(3, "env.graphml");
		(*T).print("bug.graphml");

		(*T).check();
		exit(3);
	}

	t1 =(*e).getOtherTriangle(t0);
	if((*t1).isEnqueued())
		Q.remove(t1);

	

	vj0 = (*e).getV0();
	vj1 = (*e).getV1();
	vn0 = (*t0).getOtherVertex(e);
	vn1 = (*t1).getOtherVertex(e);

	delete e;

	// new triangle vn0, vn1, vj0
	e = new TEdge(vn0, vn1);
	(*T).addEdge(e);

	e1 = (*vj0).getEdgeTo(vn0);
	e2 = (*vj0).getEdgeTo(vn1);

	t0 = new Triangle(e, e1, e2, vn0, vn1, vj0, "Flip1", ok);

	// new triangle vn0, vn1, vj1
	(*T).addEdge(e);

	e1 = (*vj1).getEdgeTo(vn0);
	e2 = (*vj1).getEdgeTo(vn1);

	t1 = new Triangle(e, e1, e2, vn0, vn1, vj1, "Flip2", ok);

	// add new triangles to queue if necessary
	if(!singleFlip){
		// reset coordinates temporarely to original position
		x = (*original).getX();
		y = (*original).getY();
		(*original).setPosition((*oldV).getX(), (*oldV).getY());

		t = (*t0).calculateCollapseTime(original, dx, dy); // again between 0 and 1

		if(t <= actualTime + epsilon && t >= actualTime - epsilon)
			stable = false;

		if(t <= 1 + epsilon && t >= 1 - epsilon)
			stable = false;

		if(t >= actualTime && t <= 1){
			(*t0).enqueue();
			stable = stable && Q.insert(t, t0);
		}

		t = (*t1).calculateCollapseTime(original, dx, dy); // again between 0 and 1

		if(t <= actualTime + epsilon && t >= actualTime - epsilon)
			stable = false;

		if(t <= 1 + epsilon && t >= 1 - epsilon)
			stable = false;

		if(t >= actualTime && t <= 1){
			(*t1).enqueue();
			stable = stable && Q.insert(t, t1);
		}

		(*original).setPosition(x, y);
	}
	
	return stable;
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
	// TODO: what happens if checkEdge runs in the case that the edge intersects all edges of a triangle?
	split = !checkEdge(original, transPath);
}
	
Translation::~Translation(){
	(*original).checkSurroundingPolygonAdvanced();

	delete transPath;
	delete oldV;
	delete newV;
}
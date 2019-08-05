#include "translation.h"

// Constructors
Translation::Translation(Triangulation* Tr, Settings &set, int i, double dX, double dY) : 
T(Tr), index(i), dx(dX), dy(dY), actualTime(0), split(false), type(TranslationType::DEFAULT), settings(set)

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

	Q = new EventQueue(epsilon, original, oldV, newV);
}

Translation::Translation(Triangulation* Tr, Settings &set, int i, double dX, double dY, TranslationType tp) : 
T(Tr), index(i), dx(dX), dy(dY), actualTime(0), split(false), type(tp), settings(set)

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

	Q = new EventQueue(epsilon, original, oldV, newV);
}

// old version with non-exact arithmetic
/*bool Translation::generateInitialQueue(){
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
}*/

/*
bool Translation::generateInitialQueue(){
	double t;
	std::list<Triangle*> triangles = (*original).getTriangles();
	bool ok = true;
	TEdge *opposite;
	double areaOld, areaNew;
	Triangle *tr;
	Vertex *v0, *v1;


	for(auto& i : triangles){
		opposite = (*i).getEdgeNotContaining(original);
		v0 = (*opposite).getV0();
		v1 = (*opposite).getV1();

		tr = new Triangle(v0, v1, oldV);
		areaOld = (*tr).signedArea();
		delete tr;

		tr = new Triangle(v0, v1, newV);
		areaNew = (*tr).signedArea();
		delete tr;

		if(areaOld == 0 || (areaNew == 0 && type != TranslationType::SPLIT_PART_1)){
			printf("The vertex %llu to be translated lays exactly on an edge :0\n", (*original).getID());
			printf("areaOld: %.20f areaNew: %.20f \n", areaOld, areaNew);

			(*newV).print();
			(*v0).print();
			(*v1).print();

			(*T).addVertex(newV);
			(*T).print("debug.graphml");
			exit(7);
		}

		if(signbit(areaOld) == signbit(areaNew))
			continue;
		else{
			t = (*i).calculateCollapseTime(original, dx, dy);

			if(t < 0){
				printf("numerical correction: collapse time of collapsing triangle was %.20f \n", t);
				t = 0;
			}

			if(t > 1){
				printf("numerical correction: collapse time of collapsing triangle was %.20f \n", t);
				if(t > 1.00001){
					printf("dx: %.20f dy: %.20f \n", dx, dy);
					printf("start\n");
					(*oldV).print();
					printf("end\n");
					(*newV).print();
					(*v0).print();
					(*v1).print();

					exit(10);
				}

				t = 1;
			}

			(*i).enqueue();

			if(!Q.insert(t, i))
				return false;
		}
	}

	return true;
}
*/

bool Translation::generateInitialQueue(){
	double t;
	std::list<Triangle*> triangles = (*original).getTriangles();
	TEdge *opposite;
	Vertex *v0, *v1;
	double areaOld, areaNew;
	Triangle *tr;
	bool ok;
	
	for(auto& i : triangles){
		opposite = (*i).getEdgeNotContaining(original);
		v0 = (*opposite).getV0();
		v1 = (*opposite).getV1();

		tr = new Triangle(v0, v1, oldV);
		areaOld = (*tr).signedArea();
		delete tr;

		tr = new Triangle(v0, v1, newV);
		areaNew = (*tr).signedArea();
		delete tr;

		if(areaOld == 0 || (areaNew == 0 && type != TranslationType::SPLIT_PART_1)){
			printf("The vertex %llu to be translated lays exactly on an edge :0\n", (*original).getID());
			printf("areaOld: %.20f areaNew: %.20f \n", areaOld, areaNew);

			(*newV).print();
			(*v0).print();
			(*v1).print();

			(*T).addVertex(newV);
			(*T).print("debug.graphml");
			exit(7);
		}

		if(signbit(areaOld) == signbit(areaNew))
			continue;
		else{
			t = (*i).calculateCollapseTime(original, dx, dy);

			if(t < 0){
				printf("numerical correction: collapse time of collapsing triangle was %.20f \n", t);
				t = 0;
			}

			if(t > 1){
				printf("numerical correction: collapse time of collapsing triangle was %.20f \n", t);
				t = 1;
			}

			(*i).enqueue();

			(*Q).insertWithoutCheck(t, i);
		}
	}

	ok = (*Q).makeStable(true);

	return ok;
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

	intersection = checkIntersection(dummyEdge, prevOldE, epsilonInt);
	if(intersection != IntersectionType::NONE)
		count++;
	intersection = checkIntersection(dummyEdge, nextOldE, epsilonInt);
	if(intersection != IntersectionType::NONE)
		count++;
	intersection = checkIntersection(dummyEdge, prevNewE, epsilonInt);
	if(intersection != IntersectionType::NONE)
		count++;
	intersection = checkIntersection(dummyEdge, nextNewE, epsilonInt);
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
		iType = checkIntersection(newE, i, epsilonInt);

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
		iType0 = checkIntersection(newE, surEdges[0], epsilonInt);
		iType1 = checkIntersection(newE, surEdges[1], epsilonInt);

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


// check after translation whether any of the still existing triangles has the area 0
// as the cehckintersection tries to keep vertices away from edges, this should just be the case
// for splitted translations of type 2
// but this is not good enough to avoid all security flips for splited translations!
void Translation::repairEnd(){
	std::list<Triangle*> triangles;
	double area;
	TEdge *edge;
	Translation *trans;
	enum Executed ex;
	
	triangles = (*original).getTriangles();

	for(auto& i : triangles){
		area = (*i).signedArea();

		if(area == 0){
			if(type == TranslationType::DEFAULT){
				printf("Translation: Triangle area = 0 after translation...");
				printf("start position\n");
				(*oldV).print();
				printf("target position\n");
				(*newV).print();
				printf("acutal end position\n");
				(*original).print();
			}

			edge = (*i).getLongestEdgeAlt();

			if(type == TranslationType::DEFAULT)
				printf("intersectiontype: %d\n", (int)checkIntersection(transPath, edge, epsilonInt));

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

			if(type == TranslationType::DEFAULT)
				printf("corrected! \n");
		}
	}
}

// public member functions

bool Translation::checkOverroll(){
	bool overroll;
	Vertex* randomV;

	// check whether the quadrilateral of the chosen Vertex P, its translated version P' and the two neighbors M and N is simple
	// otherwise there can not be any overroll
	overroll = !(checkIntersection(prevOldE, nextNewE, epsilonInt) != IntersectionType::NONE || checkIntersection(nextOldE, prevNewE, epsilonInt) != IntersectionType::NONE);

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
	double middleX, middleY, transX, transY, oldArea, newArea;
	TEdge* edge;
	Vertex* intersectionPoint;
	enum Executed ex;

	if(split){
		
		t = new Triangle(prevV, nextV, oldV);
		oldArea = (*t).signedArea();
		delete t;

		t = new Triangle(prevV, nextV, newV);
		newArea = (*t).signedArea();
		delete t;

		// vertex stays on the same side of the edge between the neighboring vertices
		if(signbit(oldArea) == signbit(newArea)){
			
			// compute the intersection point to split the translation
			intersectionPoint = getIntersectionPoint(prevV, oldV, nextV, newV);
			if(intersectionPoint == NULL)
				intersectionPoint = getIntersectionPoint(nextV, oldV, prevV, newV);
			if(intersectionPoint == NULL){
				return Executed::REJECTED;
			}

			// first part of the translation
			transX = (*intersectionPoint).getX() - (*oldV).getX();
			transY = (*intersectionPoint).getY() - (*oldV).getY();

			trans = new Translation(T, settings, index, transX, transY, TranslationType::SPLIT_PART_1);
			ex = (*trans).execute();

			delete intersectionPoint;
			delete trans;

			if(ex != Executed::FULL)
				return ex;

			// second part of the translation
			transX = (*newV).getX() - (*original).getX();
			transY = (*newV).getY() - (*original).getY();

			trans = new Translation(T, settings, index, transX, transY, TranslationType::SPLIT_PART_2);
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

			trans = new Translation(T, settings, index, transX, transY, TranslationType::SPLIT_PART_1);
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

			trans = new Translation(T, settings, index, transX, transY, TranslationType::SPLIT_PART_2);
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

		while((*Q).size() > 0){
			e = (*Q).pop();
			actualTime = e.first;
			t = e.second;

			if(!flip(t, false)){
				printf("aborted\n");
				return Executed::PARTIAL;
			}
		}

		//(*original).setPosition((*oldV).getX() + dx, (*oldV).getY() + dy);
		(*original).setPosition((*newV).getX(), (*newV).getY());

		return Executed::FULL;
	}
}

bool Translation::flip(Triangle *t0, bool singleFlip){
	bool ok = true;
	TEdge *e, *e1, *e2;
	Triangle *t1;
	Vertex *vj0, *vj1; // joint vertices
	Vertex *vn0, *vn1; // non-joint vertices
	// we call it an oppositeFlip, if the flipped edge is the one opposite to the moving vertex
	// i.e. it doesn't contain the moving vertex
	bool oppositeFlip;
	Vertex *dummyVertex;
	double x, y;
	double area0, area1;
	Triangle *dummyTriangle;
	bool insertion = false; //indicates whether a new triangle was inserted into the eventqueue
	double time;

	if(!singleFlip)
		// move vertex to event time
		(*original).setPosition((*oldV).getX() + dx * actualTime, (*oldV).getY() + dy * actualTime);

	// get the edge which should be flipped
	e = (*t0).getLongestEdgeAlt();
	if((*e).getEdgeType() == EdgeType::POLYGON){
		printf("Flip: polygon edge gets deleted\n");
		printf("id: %llu index: %d dx: %f dy: %f \n", (*original).getID(), index, dx, dy);

		(*T).check();
		exit(3);
	}

	// check for flip type
	if((*e).contains(original))
		oppositeFlip = false;
	else
		oppositeFlip = true;

	// remove the other triangle from the eventqueue if it is enqueued
	t1 =(*e).getOtherTriangle(t0);
	if((*t1).isEnqueued())
		(*Q).remove(t1);

	// get all vertices of the triangles which are removed
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

	if(!singleFlip){
		// reset coordinates temporarely to original position
		x = (*original).getX();
		y = (*original).getY();
		(*original).setPosition((*oldV).getX(), (*oldV).getY());

		// decide which of the new triangles has to be inserted into the eventqueue
		// if the opposite edge was flipped potentially both triangles can collapse in the future
		if(oppositeFlip){
			// first decide whether the non-moving vertex which is shared by both triangles
			// after the flip (we call it static vertex in the following)
			// is inside the corridor built by the two lines parallel to the transition line through
			// the non-shared vertices

			// find the static vertex (store in vn0)
			if((*vn0).getID() == (*original).getID())
				vn0 = vn1;

			dummyVertex = (*vj0).getTranslated(dx, dy);
			dummyTriangle = new Triangle(vj0, dummyVertex, vn0);
			area0 = (*dummyTriangle).signedArea();
			delete dummyTriangle;
			delete dummyVertex;

			dummyVertex = (*vj1).getTranslated(dx, dy);
			dummyTriangle = new Triangle(vj1, dummyVertex, vn0);
			area1 = (*dummyTriangle).signedArea();
			delete dummyTriangle;
			delete dummyVertex;

			// the static vertex is inside the corridor
			// i.e. both new triangles will collapse in the future
			if(signbit(area0) != signbit(area1)){
				// now we have to check for both new triangles, whether they collapse before
				// the end of the translation

				// for t0 (consisting of vn0, vn1, vj0)
				// we have to check the edge from the static vertex vn0 to the joint vertex vj0
				dummyTriangle = new Triangle(vn0, vj0, oldV);
				area0 = (*dummyTriangle).signedArea();
				delete dummyTriangle;

				dummyTriangle = new Triangle(vn0, vj0, newV);
				area1 = (*dummyTriangle).signedArea();
				delete dummyTriangle;

				if(signbit(area0) != signbit(area1)){
					time = (*t0).calculateCollapseTime(original, dx, dy);
					(*Q).insertWithoutCheck(time, t0);
					(*t0).enqueue();
					insertion = true;
				}

				// for t1 (consisting of vn0, vn1, vj1)
				// we have to check the edge from the static vertex vn0 to the joint vertex vj1
				dummyTriangle = new Triangle(vn0, vj1, oldV);
				area0 = (*dummyTriangle).signedArea();
				delete dummyTriangle;

				dummyTriangle = new Triangle(vn0, vj1, newV);
				area1 = (*dummyTriangle).signedArea();
				delete dummyTriangle;

				if(signbit(area0) != signbit(area1)){
					time = (*t1).calculateCollapseTime(original, dx, dy);
					(*Q).insertWithoutCheck(time, t1);
					(*t1).enqueue();
					insertion = true;
				}

			// the static vertex is not inside the corridor
			}else{
				// now we have to find out which of the two new triangles is the one which collapses
				// in the future

				// try t0 (consisting of vn0, original, vj0)
				// the non-joint vertex of t0 is vj0

				// now we can take a look whether the static and the moving vertex are on different sides
				// of the line parallel to the transition line through the non-joint vertex

				dummyVertex = (*vj0).getTranslated(dx, dy);

				dummyTriangle = new Triangle(vj0, dummyVertex, vn0);
				area0 = (*dummyTriangle).signedArea();
				delete dummyTriangle;

				dummyTriangle = new Triangle(vj0, dummyVertex, original);
				area0 = (*dummyTriangle).signedArea();
				delete dummyTriangle;

				delete dummyVertex;

				// if they are on different sides, then t1 is the triangle which will collapse in the future
				// t1 consists of original, vn0 (static) and vj1
				if(signbit(area0) != signbit(area1)){
					// now we have to check whether the triangle t1 collapses before the end of the translation
					// i.e. start and end position have to be on different side of the edge (vn0, vj1)

					dummyTriangle = new Triangle(vn0, vj1, oldV);
					area0 = (*dummyTriangle).signedArea();
					delete dummyTriangle;

					dummyTriangle = new Triangle(vn0, vj1, newV);
					area1 = (*dummyTriangle).signedArea();
					delete dummyTriangle;

					if(signbit(area0) != signbit(area1)){
						time = (*t1).calculateCollapseTime(original, dx, dy);
						(*Q).insertWithoutCheck(time, t1);
						(*t1).enqueue();
						insertion = true;
					}
				}else{
					// now we have to check whether the triangle t0 collapses before the end of the translation
					// i.e. start and end position have to be on different side of the edge (vn0, vj0)

					dummyTriangle = new Triangle(vn0, vj0, oldV);
					area0 = (*dummyTriangle).signedArea();
					delete dummyTriangle;

					dummyTriangle = new Triangle(vn0, vj0, newV);
					area1 = (*dummyTriangle).signedArea();
					delete dummyTriangle;

					if(signbit(area0) != signbit(area1)){
						time = (*t0).calculateCollapseTime(original, dx, dy);
						(*Q).insertWithoutCheck(time, t0);
						(*t0).enqueue();
						insertion = true;
					}
				}
			}


		// a non-opposite edge was flipped, so just one triangle can collapse in the future
		}else{
			// the new triangle will collapse in the future if the non-joint vertex of the triangle
			// which won't change anymore is on the same side of the new edge (vn0, vn1) as the 
			// target position is


			// find the static vertex (store in vj0)
			if((*vj0).getID() == (*original).getID())
				vj0 = vj1;

			dummyTriangle = new Triangle(vn0, vn1, vj0);
			area0 = (*dummyTriangle).signedArea();
			delete dummyTriangle;

			dummyTriangle = new Triangle(vn0, vn1, newV);
			area1 = (*dummyTriangle).signedArea();
			delete dummyTriangle;

			// otherwise none of the two new triangles will collapse in the future
			if(signbit(area0) == signbit(area1)){
				// get the triangle which still contains the moving vertex
				if(!(*t0).contains(original))
					t0 = t1;

				// now we have to check whether the new triangle will collapse before the
				// translation ends, i.e. the start position end the end position of the movements
				// are on different sides of the new edge which is given by vn0 and vn1

				dummyTriangle = new Triangle(vn0, vn1, oldV);
				area0 = (*dummyTriangle).signedArea();
				delete dummyTriangle;

				dummyTriangle = new Triangle(vn0, vn1, newV);
				area1 = (*dummyTriangle).signedArea();
				delete dummyTriangle;

				if(signbit(area0) != signbit(area1)){
					time = (*t0).calculateCollapseTime(original, dx, dy);
					(*Q).insertWithoutCheck(time, t0);
					(*t0).enqueue();
					insertion = true;
				}
			}	
		}

		// get original back to actual position
		(*original).setPosition(x, y);

		if(insertion)
			return (*Q).makeStable(false);
		else
			return true;
	}

	return true;
}

/*bool Translation::flip(Triangle* t0, bool singleFlip){
	Triangle *t1;
	Vertex *vj0, *vj1; // joint vertices
	Vertex *vn0, *vn1; // non joint vertices
	TEdge *e, *e1, *e2;
	double t;
	double x, y;
	bool ok = true;
	bool stable = true;
	double areaOld, areaNew;


	if(!singleFlip){
		// check whether the moving vertex is still on the right side of the flipping edge
		e = (*t0).getEdgeNotContaining(original);
		vj0 = (*e).getV0();
		vj1 = (*e).getV1();

		t1 = new Triangle(vj0, vj1, original);
		areaOld = (*t1).signedArea();
		delete t1;

		t1 = new Triangle(vj0, vj1, newV);
		areaNew = (*t1).signedArea();
		delete t1;

		if(signbit(areaOld) == signbit(areaNew)){
			printf("moving vertex already passed position for next event\n");

			exit(8);
		}

		// move vertex to event time
		(*original).setPosition((*oldV).getX() + dx * actualTime, (*oldV).getY() + dy * actualTime);
	}


	e = (*t0).getLongestEdgeAlt();
	if((*e).getEdgeType() == EdgeType::POLYGON){
		printf("Flip: polygon edge gets deleted\n");
		printf("id: %llu index: %d dx: %f dy: %f \n", (*original).getID(), index, dx, dy);

		(*t0).print();

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
		(*Q).remove(t1);

	

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
			stable = stable && (*Q).insert(t, t0);
		}

		t = (*t1).calculateCollapseTime(original, dx, dy); // again between 0 and 1

		if(t <= actualTime + epsilon && t >= actualTime - epsilon)
			stable = false;

		if(t <= 1 + epsilon && t >= 1 - epsilon)
			stable = false;

		if(t >= actualTime && t <= 1){
			(*t1).enqueue();
			stable = stable && (*Q).insert(t, t1);
		}

		(*original).setPosition(x, y);
	}
	
	return stable;
}*/

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
	bool ok;

	repairEnd();

	ok = (*original).checkSurroundingPolygonAdvanced();

	if(!ok){
		printf("\nstart position:\n");
		(*oldV).print();
		printf("original position:\n");
		(*original).print();
		printf("target position:\n");
		(*newV).print();
		printf("translation vector: dx = %.20f dy = %.20f \n", dx, dy);

		(*T).print("bug.graphml");

		exit(6);
	}

	delete transPath;
	delete oldV;
	delete newV;
}
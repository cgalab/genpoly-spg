#include "polygonTransformer.h"

void transformPolygon(Triangulation* T, int iterations, Timer t){
	int index = 1;
	RandomGenerator* generator = new RandomGenerator();
	double dx = -45, dy = 0; // radius of the initial polygon was 30
	Vertex *newV, *oldV;
	bool simple;

	oldV = (*T).getVertex(index);
	newV = (*oldV).getTranslated(dx, dy);

	simple = checkSimplicityOfTranslation(T, index, oldV, newV);

	printf("simplicity check gives result %d \n", simple);

	(*T).addVertex(newV);
}

bool checkSimplicityOfTranslation(Triangulation* T, int index, Vertex *oldV, Vertex* newV){
	Vertex *prevV, *nextV;
	TEdge *prevE, *nextE, *prevNewE, *nextNewE;
	std::vector<TEdge*> prevSur, nextSur;
	enum intersect_t iType = intersect_t::IS_FALSE;
	std::map<int, TEdge*> intersected;
	EdgeType eType;
	Triangle* nextT = NULL;

	prevV = (*T).getVertex(index - 1);
	nextV = (*T).getVertex(index + 1);

	prevE = (*prevV).getEdgeTo(oldV);
	nextE = (*nextV).getEdgeTo(oldV);

	(*prevE).setEdgeType(EdgeType::TRIANGULATION);
	(*nextE).setEdgeType(EdgeType::TRIANGULATION);

	prevSur = (*prevV).getSurroundingEdges();
	nextSur = (*nextV).getSurroundingEdges();

	prevNewE = new TEdge(prevV, newV, EdgeType::POLYGON);
	nextNewE = new TEdge(newV, nextV, EdgeType::POLYGON);

	// start with the edge to the previous vertex
	for(auto& i : prevSur){
		iType = checkIntersection(prevNewE, i);

		// new edge hits vertex of surrounding polygon
		if(iType > intersect_t::IS_FALSE && iType <= intersect_t::IS_VERTEX22) return false;
		// intersection with edge of surrounding polygon
		else if(iType == intersect_t::IS_TRUE){
			eType = (*i).getEdgeType();

			// intersected edge is polygon edge
			if(eType == EdgeType::POLYGON){
				printf("prev hit polygon edge from vertex %d to vertex %d \n", (*(*i).getV1()).getID(), (*(*i).getV2()).getID());
				return false;
			// intersected edge is frame edge
			}else if(eType == EdgeType::FRAME){
				printf("prev hit frame edge from vertex %d to vertex %d \n", (*(*i).getV1()).getID(), (*(*i).getV2()).getID());
				return false;
			// intersected edge is just a triangulation edge
			}else{
				printf("prev hit triangulation edge from vertex %d to vertex %d \n", (*(*i).getV1()).getID(), (*(*i).getV2()).getID());
				intersected.insert(std::pair<int, TEdge*>((*i).getID(), i));
				nextT = (*i).getTriangleNotContaining(prevV);
				prevSur = (*nextT).getOtherEdges(i);
				break;
			}
		}
	}

	// iterate over the adjacent triangles it there was an intersection with a triangulation edge
	while(!(iType == intersect_t::IS_FALSE)){

		for(auto& i : prevSur){
			iType = checkIntersection(prevNewE, i);

			// new edge hits vertex of surrounding polygon
			if(iType > intersect_t::IS_FALSE && iType <= intersect_t::IS_VERTEX22) return false;
			// intersection with edge of surrounding polygon
			else if(iType == intersect_t::IS_TRUE){
				eType = (*i).getEdgeType();

				// intersected edge is polygon edge
				if(eType == EdgeType::POLYGON){
					printf("prev hit polygon edge from vertex %d to vertex %d \n", (*(*i).getV1()).getID(), (*(*i).getV2()).getID());
					return false;
				// intersected edge is frame edge
				}else if(eType == EdgeType::FRAME){
					printf("prev hit frame edge from vertex %d to vertex %d \n", (*(*i).getV1()).getID(), (*(*i).getV2()).getID());
					return false;
				// intersected edge is just a triangulation edge
				}else{
					printf("prev hit triangulation edge from vertex %d to vertex %d \n", (*(*i).getV1()).getID(), (*(*i).getV2()).getID());
					intersected.insert(std::pair<int, TEdge*>((*i).getID(), i));
					nextT = (*i).getOtherTriangle(nextT);
					prevSur = (*nextT).getOtherEdges(i);
					break;
				}
			}
		}
	}

	return true;


	// das funktioniert so nicht (returns before)
	(*prevE).setEdgeType(EdgeType::POLYGON);
	(*nextE).setEdgeType(EdgeType::POLYGON);
}

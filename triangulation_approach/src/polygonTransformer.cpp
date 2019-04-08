#include "polygonTransformer.h"

void transformPolygon(Triangulation* T, int iterations, Timer t){
	int index = 1;
	RandomGenerator* generator = new RandomGenerator();
	double dx = -45, dy = 0; // radius of the initial polygon was 30
	Vertex *newV, *oldV;
	bool simple;
	std::map<int, TEdge*> intersected;

	oldV = (*T).getVertex(index);
	newV = (*oldV).getTranslated(dx, dy);

	simple = checkSimplicityOfTranslation(T, index, oldV, newV, intersected);

	printf("simplicity check gives result %d \n", simple);

	(*T).addVertex(newV);
}

bool checkSimplicityOfTranslation(Triangulation* T, int index, Vertex *oldV, Vertex* newV, std::map<int, TEdge*> &intersected){
	Vertex *prevV, *nextV;
	TEdge *prevE, *nextE, *prevNewE, *nextNewE;
	bool simple;
	

	prevV = (*T).getVertex(index - 1);
	nextV = (*T).getVertex(index + 1);

	prevE = (*prevV).getEdgeTo(oldV);
	nextE = (*nextV).getEdgeTo(oldV);

	(*prevE).setEdgeType(EdgeType::TRIANGULATION);
	(*nextE).setEdgeType(EdgeType::TRIANGULATION);


	prevNewE = new TEdge(prevV, newV, EdgeType::POLYGON);
	nextNewE = new TEdge(newV, nextV, EdgeType::POLYGON);

	simple = checkEdge(prevV, prevNewE, intersected);
	simple = simple && checkEdge(nextV, nextNewE, intersected);

	(*prevE).setEdgeType(EdgeType::POLYGON);
	(*nextE).setEdgeType(EdgeType::POLYGON);

	return simple;
}

bool checkEdge(Vertex* fromV, TEdge* newE, std::map<int, TEdge*> &intersected){
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
				printf("edge hit polygon edge from vertex %d to vertex %d \n", (*(*i).getV1()).getID(), (*(*i).getV2()).getID());
				return false;
			// intersected edge is frame edge
			}else if(eType == EdgeType::FRAME){
				printf("edge hit frame edge from vertex %d to vertex %d \n", (*(*i).getV1()).getID(), (*(*i).getV2()).getID());
				return false;
			// intersected edge is just a triangulation edge
			}else{
				printf("edge hit triangulation edge from vertex %d to vertex %d \n", (*(*i).getV1()).getID(), (*(*i).getV2()).getID());
				intersected.insert(std::pair<int, TEdge*>((*i).getID(), i));
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
					surEdges = (*nextT).getOtherEdges(i);
					break;
				}
			}
		}
	}

	return true;
}

#include "polygonTransformer.h"

void transformPolygon(Triangulation* T, int iterations, Timer t){
	int index = 1;
	RandomGenerator* generator = new RandomGenerator();
	double dx = -50, dy = 10; // radius of the initial polygon was 30
	Vertex *newV, *oldV;

	oldV = (*T).getVertex(index);
	newV = (*oldV).getTranslated(dx, dy);

	checkSimplicityOfTranslation(T, index, oldV, newV);
}

bool checkSimplicityOfTranslation(Triangulation* T, int index, Vertex *oldV, Vertex* newV){
	Vertex *prevV, *nextV;
	TEdge *prevE, *nextE, *prevNewE, *nextNewE;
	std::vector<TEdge*> prevSur, nextSur;
	enum intersect_t iType;

	prevV = (*T).getVertex(index - 1);
	nextV = (*T).getVertex(index + 1);

	prevE = (*prevV).getEdgeTo(oldV);
	nextE = (*nextV).getEdgeTo(oldV);

	(*prevE).setEdgeType(EdgeType::TRIANGULATION);
	(*nextE).setEdgeType(EdgeType::TRIANGULATION);

	prevSur = (*prevV).getSurroundingEdges();
	nextSur = (*nextV).getSurroundingEdges();

	printf("surrounding edges of vertex %d:\n", (*prevV).getID());
	for(auto const& i : prevSur){
		(*i).print();
	}

	printf("surrounding edges of vertex %d:\n", (*nextV).getID());
	for(auto const& i : nextSur){
		(*i).print();
	}

	prevNewE = new TEdge(prevV, newV, EdgeType::POLYGON);
	nextNewE = new TEdge(newV, nextV, EdgeType::POLYGON);

	for(auto const& i : nextSur){
		iType = checkIntersection(nextNewE, i);
		printf("intersection type with edge from vertex %d to vertex %d: %d \n", (*(*i).getV1()).getID(), (*(*i).getV2()).getID(), iType);
	}




	(*prevE).setEdgeType(EdgeType::POLYGON);
	(*nextE).setEdgeType(EdgeType::POLYGON);
}

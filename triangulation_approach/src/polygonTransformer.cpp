#include "polygonTransformer.h"

void transformPolygon(Triangulation* T, int iterations, Timer t){
	int index = 1;
	RandomGenerator* generator = new RandomGenerator();
	double dx = 3, dy = 0;
	Vertex *newV, *oldV;

	oldV = (*T).getVertex(index);
	newV = (*oldV).getTranslated(dx, dy);

	checkSimplicityOfTranslation(T, index, oldV, newV);
}

bool checkSimplicityOfTranslation(Triangulation* T, int index, Vertex *oldV, Vertex* newV){
	Vertex *prevV, *nextV;
	TEdge *prevE, *nextE;
	std::vector<TEdge*> prevSur, nextSur; 

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

	(*prevE).setEdgeType(EdgeType::POLYGON);
	(*nextE).setEdgeType(EdgeType::POLYGON);
}

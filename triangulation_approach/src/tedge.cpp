#include "tedge.h"

TEdge::TEdge(Vertex* V1, Vertex* V2, bool PEdge, bool CHEdge){ 
	v1 = V1;
	v2 = V2;
	t1 = NULL;
	t2 = NULL;
	isPEdge = PEdge;
	isCHEdge = CHEdge;

	(*V1).addEdge(this);
	(*V2).addEdge(this);
}

TEdge::TEdge(Vertex* V1, Vertex* V2){ 
	v1 = V1;
	v2 = V2;
	t1 = NULL;
	t2 = NULL;
	isPEdge = false;
	isCHEdge = false;

	(*V1).addEdge(this);
	(*V2).addEdge(this);
}

void TEdge::setTriangle(Triangle* t){
	if(t1 == NULL) t1 = t;
	else if(t2 == NULL) t2 = t;
	else printf("This edge already has two triangles\n");
}

void TEdge::print(FILE* f){
	fprintf(f, "<edge vertex1=\"%d\" vertex2=\"%d\"></edge>\n", (*v1).getID(), (*v2).getID());
}
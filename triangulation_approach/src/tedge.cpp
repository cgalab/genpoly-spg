#include "tedge.h"

TEdge::TEdge(Vertex* V1, Vertex* V2, bool PEdge, bool REdge){ 
	v1 = V1;
	v2 = V2;
	t1 = NULL;
	t2 = NULL;
	isPE = PEdge;
	isRE = REdge;

	(*V1).addEdge(this);
	(*V2).addEdge(this);

	id = n;
	n++;
}

TEdge::TEdge(Vertex* V1, Vertex* V2){ 
	v1 = V1;
	v2 = V2;

	t1 = NULL;
	t2 = NULL;

	isPE = false;
	isRE = false;

	(*V1).addEdge(this);
	(*V2).addEdge(this);

	id = n;
	n++;
}

void TEdge::setTriangle(Triangle* t){
	if(t1 == NULL) t1 = t;
	else if(t2 == NULL) t2 = t;
	else printf("This edge already has two triangles\n");
}

void TEdge::print(FILE* f){
	int w = 0;
	if(isPE) w = 2;
	if(isRE) w = w + 1;
	fprintf(f, "<edge vertex1=\"%d\" vertex2=\"%d\" weight=\"%d\" useWeight=\"true\"></edge>\n", (*v1).getID(), (*v2).getID(), w);
}

void TEdge::print(){
	printf("Edge from point %d to point %d is PE %d, is CHE %d\n", (*v1).getID(), (*v2).getID(), isPE, isRE);
}

Vertex* TEdge::getV1(){
	return v1;
}

Vertex* TEdge::getV2(){
	return v2;
}

int TEdge::n = 0;

#include "vertex.h"
#include "triangle.h"
#include "tedge.h"
#include "triangulation.h"

class Translation{

private:
	Triangulation* T;

	Vertex* oldV;
	Vertex* newV;
	Vertex* prevV;
	Vertex* nextV;

	TEdge* transPath;

	int index;

	int actualTime;
	bool split;

public:
	Translation(Triangulation* Tr, int i, double dx, double dy){
		T = Tr;
		index = i;
		actualTime = 0;
		split = false;

		oldV = (*T).getVertex(index);
		prevV = (*T).getPVertex(index - 1);
		nextV = (*T).getPVertex(index + 1);

		newV = (*oldV).getTranslated(dx, dy);

		transPath = new TEdge(oldV, newV);
	}

	Vertex* getOldV(){ return oldV;}
	Vertex* getNewV(){ return newV;}
	Vertex* getPrevV(){ return prevV;}
	Vertex* getNextV(){ return nextV;}

	void setSplit(){ split = true;}

	TEdge* getTranslationPath(){ return transPath;}

};
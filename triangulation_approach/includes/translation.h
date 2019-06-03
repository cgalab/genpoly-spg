#include "vertex.h"
#include "triangle.h"
#include "tedge.h"
#include "triangulation.h"
#include "customPriorityQueue.h"
#include <list>
#include <math.h>
#include <limits>
#include <string>
#include "eventQueue.h"


#ifndef __TRANSLATION_H_
#define __TRANSLATION_H_

enum class Executed {TRUE, FALSE, PARTIAL};

class Translation{

private:
	Triangulation* T;

	Vertex* original;
	Vertex* oldV;
	Vertex* newV;
	Vertex* prevV;
	Vertex* nextV;

	TEdge* transPath;
	TEdge* prevOldE;
	TEdge* nextOldE;
	TEdge* prevNewE;
	TEdge* nextNewE;

	int index;

	double dx, dy;

	double actualTime;
	bool split;
	EventQueue Q;

	bool generateInitialQueue();

	double signedArea(Vertex* v0, Vertex* v1, Vertex* v2);

	bool insideQuadrilateral(Vertex* v);

	bool checkEdge(Vertex* fromV, TEdge* newE);

public:
	Translation(Triangulation* Tr, int i, double dX, double dY);

	bool checkOverroll();

	enum Executed execute();

	bool flip(Triangle* t0, bool singleFlip);

	bool checkSimplicityOfTranslation();

	void checkSplit();

	~Translation();
};

#endif
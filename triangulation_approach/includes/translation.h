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
#include "settings.h"


#ifndef __TRANSLATION_H_
#define __TRANSLATION_H_

enum class Executed {FULL, REJECTED, PARTIAL};

enum class TranslationType {DEFAULT, SPLIT_PART_1, SPLIT_PART_2};

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
	TranslationType type;
	EventQueue Q;
	const double epsilon = 0.00001;
	const double epsilonInt = 0.000000000001; // 10⁻¹²

	Settings settings;

	bool generateInitialQueue();

	bool insideQuadrilateral(Vertex* v);

	bool checkEdge(Vertex* fromV, TEdge* newE);

	void repairEnd();

public:
	Translation(Triangulation* Tr, Settings &set, int i, double dX, double dY);
	Translation(Triangulation* Tr, Settings &set, int i, double dX, double dY, TranslationType tp);

	bool checkOverroll();

	enum Executed execute();

	bool flip(Triangle* t0, bool singleFlip);

	bool checkSimplicityOfTranslation();

	void checkSplit();

	~Translation();
};

#endif
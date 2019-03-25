#include "polygon.h"
#include "randomGenerator.h"
#include <stdio.h>
#include "edge.h"
#include "point.h"
#include "timer.h"

void transformPolygon(Polygon* p, int iterations, Timer t);
bool checkSimplicityOfTranslation(Polygon* p, int index);
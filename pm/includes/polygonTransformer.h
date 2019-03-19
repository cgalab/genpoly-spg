#include "polygon.h"
#include "randomGenerator.h"
#include <stdio.h>
#include "edge.h"
#include "point.h"

void transformPolygon(Polygon* p, int iterations);
bool checkSimplicityOfTranslation(Polygon* p, int index);
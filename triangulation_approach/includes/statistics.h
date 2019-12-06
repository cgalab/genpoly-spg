#include <vector>
#include <math.h>

#include "triangulation.h"
#include "vertex.h"


void calculateDistanceDistribution(Triangulation const * const T, const double width);

void calculateMaxTwist(Triangulation const * const T);

void countOrientationChanges(Triangulation const * const T);
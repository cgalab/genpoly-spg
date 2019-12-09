#include <vector>
#include <math.h>

#include "triangulation.h"
#include "vertex.h"


void calculateRadialDistanceDistribution(Triangulation const * const T, const double width);

void calculateRadialDistanceDeviation(Triangulation const * const T);

void calculateMaxTwist(Triangulation const * const T);

void countOrientationChanges(Triangulation const * const T);
#include "triangulation.h"
#include "randomGenerator.h"
#include <stdio.h>
#include "tedge.h"
#include "vertex.h"
#include "timer.h"
#include "triangle.h"
#include <list>
#include <vector>
#include <map>
#include "basicDefinitions.h"
#include <math.h>
#include "translation.h"
#include "insertion.h"
#include "settings.h"

int transformPolygonByMoves(Settings &settings, RandomGenerator &generator, Triangulation* T, int iterations, bool fiexd);
void growPolygon(Settings &settings, RandomGenerator &generator, Triangulation* T);
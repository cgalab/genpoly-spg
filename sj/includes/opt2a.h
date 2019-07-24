#ifndef __2OPTA_H_
#define __2OPTA_H_

#include <vector>
#include <utility> // for std::pair
#include <list>
#include "basicDefinitions.h"
#include "point.h"
#include "edge.h"

enum error opt2a(std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int randseed);

#endif

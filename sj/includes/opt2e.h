#ifndef __2OPTE_H_
#define __2OPTE_H_

#include <vector>
#include "basicDefinitions.h"
#include "point.h"
#include "edge.h"

enum error opt2e(std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int randseed);

#endif

#ifndef __2OPTD_H_
#define __2OPTD_H_

#include <vector>
#include "basicDefinitions.h"
#include "point.h"
#include "edge.h"

enum error opt2d(std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int randseed);

#endif

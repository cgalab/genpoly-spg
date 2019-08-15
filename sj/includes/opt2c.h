#ifndef __2OPTC_H_
#define __2OPTC_H_

#include <vector>
#include "basicDefinitions.h"
#include "point.h"
#include "edge.h"

enum error opt2c(std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int randseed);

#endif

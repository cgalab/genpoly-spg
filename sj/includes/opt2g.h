#ifndef __2OPTG_H_
#define __2OPTG_H_

#include <vector>
#include "basicDefinitions.h"
#include "point.h"
#include "edge.h"

enum error opt2g(std::vector<unsigned int>& polygon, std::vector<Point>& points);

#endif

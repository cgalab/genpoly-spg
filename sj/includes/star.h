#ifndef __STAR_H_
#define __STAR_H_

#include <vector>
#include "basicDefinitions.h"
#include "point.h"
#include "edge.h"

enum error star(std::vector<unsigned int>& polygon, std::vector<Point>& points);

#endif

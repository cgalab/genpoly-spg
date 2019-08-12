#ifndef __2OPTD_H_
#define __2OPTD_H_

#include <vector>
#include <utility> // for std::pair
#include <list>
#include "basicDefinitions.h"
#include "point.h"
#include "edge.h"

void update_lowest_index(Edge e1, Edge e2, unsigned int& lowest_index);
void update_lowest_index(Point *a, Point *b, Point *c, unsigned int& lowest_index);
enum edge_t removeEdgeFromSetd(Edge& e, unsigned int& lowest_index, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points);
std::pair<enum edge_t, std::set<Edge>::iterator> processEdged(Edge& e, unsigned int& lowest_index, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points);
enum error opt2d(std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int randseed);

#endif

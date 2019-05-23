#include <vector>
#include <utility> // for std::pair
#include <list>
#include "basicDefinitions.h"
#include "point.h"
#include "edge.h"

#ifndef __2OPT_H_
#define __2OPT_H_

bool sortbysec(const std::pair<Point,double> &a, const std::pair<Point,double> &b);
bool sortbyint(const int i, const int j);
void polSwap(Edge e, std::vector<unsigned int>& polygon);
void polSwap(Point* a, Point* b, std::vector<unsigned int>& polygon);
bool collSwap(Point *a, Point *b, Point *c, std::vector<unsigned int>& polygon);
bool collSwap (Edge& e1, Edge& e2, std::vector<unsigned int>& polygon);
enum edge_t edgeCheck(unsigned int& index, Edge& e1, std::list<Edge>& edges, std::vector<unsigned int>& polygon, std::vector<Point>& points);
enum error opt2(std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int randseed);

#endif

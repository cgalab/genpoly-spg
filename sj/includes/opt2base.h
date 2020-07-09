#ifndef __2OPT_BASE_H_
#define __2OPT_BASE_H_

void print_enum(enum planesweep_t val);
void print_enum(enum edge_t val);
void update_lowest_index(Edge e1, Edge e2, unsigned int& lowest_index);
void update_lowest_index(Edge2 e1, Edge2 e2, unsigned int& lowest_index);
void update_highest_index(Edge e1, Edge e2, unsigned int& highest_index);
void update_highest_index(Edge2 e1, Edge2 e2, unsigned int& highest_index);
void update_lowest_index(Point *a, Point *b, Point *c, unsigned int& lowest_index);
enum edge_t removeEdgeFromSet(Edge2& e, std::set<Edge2>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points);
enum edge_t removeEdgeFromSet(Edge2& e, std::set<Edge2>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int& lowest_index);
enum edge_t removeEdgeFromSet(Edge2& e, std::set<Edge2>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int& lowest_index, unsigned int& highest_index);
enum edge_t removeEdgeFromSetOld(Edge& e, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points);
enum edge_t removeEdgeFromSetb(Edge& e, unsigned int& lowest_index, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points);
enum edge_t removeEdgeFromSetb(Edge& e, unsigned int& lowest_index, unsigned int& highest_index, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points);
enum edge_t removeEdgeFromSetd(Edge& e, unsigned int& lowest_index, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points);
enum edge_t removeEdgeFromSete(Edge& e, unsigned int& lowest_index, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points);
enum edge_t removeEdgeFromSetf(Edge& e, Point *idx, unsigned int& lowest_index, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points);
enum edge_t removeEdgeFromSetf(Edge& e, Point *idx, unsigned int& lowest_index, unsigned int& highest_index, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points);
enum edge_t removeEdgeFromSetg(Edge& e, unsigned int& lowest_index, unsigned int& highest_index, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points);
enum edge_t removeEdgeFromSeth(Edge2& e, unsigned int& lowest_index, unsigned int& highest_index, std::set<Edge2>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points);
std::pair<enum edge_t, std::set<Edge2>::iterator> processEdge(Edge2& e, std::set<Edge2>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points);
std::pair<enum edge_t, std::set<Edge2>::iterator> processEdge(Edge2& e, std::set<Edge2>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int& lowest_index);
std::pair<enum edge_t, std::set<Edge2>::iterator> processEdge(Edge2& e, std::set<Edge2>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int& lowest_index, unsigned int& highest_index);
std::pair<enum edge_t, std::set<Edge>::iterator> processEdgeb(Edge& e, unsigned int& lowest_index, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points);
std::pair<enum edge_t, std::set<Edge>::iterator> processEdgeb(Edge& e, unsigned int& lowest_index, unsigned int& highest_index, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points);
std::pair<enum edge_t, std::set<Edge>::iterator> processEdged(Edge& e, Point *idx, unsigned int& lowest_index, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points);
std::pair<enum edge_t, std::set<Edge>::iterator> processEdgeOld(Edge& e, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points);
std::pair<enum edge_t, std::set<Edge>::iterator> processEdgee(Edge& e, Point *idx, unsigned int& lowest_index, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points);
std::pair<enum edge_t, std::set<Edge>::iterator> processEdgef(Edge& e, Point *idx, unsigned int& lowest_index, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points);
std::pair<enum edge_t, std::set<Edge>::iterator> processEdgef(Edge& e, Point *idx, unsigned int& lowest_index, unsigned int& highest_index, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points);
std::pair<enum edge_t, std::set<Edge>::iterator> processEdgeg(Edge& e, Point *idx, unsigned int& lowest_index, unsigned int& highest_index, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points);
//enum edge_t processEdgeh(Edge2& e, unsigned int& lowest_index, std::set<Edge2>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points);
std::pair<enum edge_t, std::set<Edge2>::iterator> processEdgeh(Edge2& e, unsigned int& lowest_index, unsigned int& highest_index, std::set<Edge2>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points);

#endif

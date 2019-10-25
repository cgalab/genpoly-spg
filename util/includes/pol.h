#ifndef __POL_H_
#define __POL_H_

#include "edge.h"

class Ends {
public:
  // a pair of I edges that define the beginning and end of an inner polygonal chain
  // between 2 incidental points on the convex hull, which could be used to form holes
  std::pair<I_Edge, I_Edge> par;

  // the theoretical upper limit of holes based on floor(points/3).
  unsigned int nr_holes;

  Ends() {std::pair<I_Edge, I_Edge>par; nr_holes=0;}
  Ends(std::pair<I_Edge, I_Edge> P) {par=P;nr_holes=0;}
  Ends(std::pair<I_Edge, I_Edge> P, unsigned int H) {par=P;nr_holes=H;}
  Ends(I_Edge e1, I_Edge e2) {par=std::make_pair(e1,e2);nr_holes=0;}

  friend std::ostream& operator << (std::ostream& os, Ends p) {
    os << "e1: " << p.par.first << ", e2: " << p.par.second << ", holes: " << p.nr_holes;
    return os;
  }
};

void update_lowest_index(Point *a, unsigned int& lowest_index, std::vector<unsigned int>& polygon, std::vector<Point>& points);
void update_highest_index(Point *a, unsigned int& highest_index, std::vector<unsigned int>& polygon, std::vector<Point>& points);
void polSwap(Edge e, std::vector<unsigned int>& polygon);
void polSwap(Point* a, Point* b, std::vector<unsigned int>& polygon);
bool collSwap (Point *a, Point *b, Point *c, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points);
bool coll3Swap(Point *a, Point *b, Point *c, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points);
bool coll3SwapOld(Point *a, Point *b, Point *c, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points);
bool coll3Sort(Point *a, Point *b, Point *c, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points);
bool coll3Sort(Point *a, Point *b, Point *c, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int& lowest_index);
bool coll3Sort(Point *a, Point *b, Point *c, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int& lowest_index, unsigned int& highest_index);
bool coll3Sort(Point *a, Point *b, Point *c, Point *idx, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int& lowest_index);
bool coll3Swap(Point *a, Point *b, Point *c, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int& lowest_index);
bool coll3Swap(Point *a, Point *b, Point *c, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int& lowest_index, unsigned int& highest_index);
bool coll4Swap (Edge& e1, Edge& e2, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points);
bool coll4SwapOld(Edge& e1, Edge& e2, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points);
bool coll4Swap (Edge& e1, Edge& e2, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int& lowest_index);
bool coll4Swap (Edge& e1, Edge& e2, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int& lowest_index, unsigned int& highest_index);
bool coll4Swap (Edge& e1, Edge& e2, Point *idx, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int& lowest_index);
bool collSwap (Edge& e1, Edge& e2, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points);
double pol_calc_circumference(std::vector<unsigned int>& polygon, std::vector<Point>& points);
double pol_calc_area(std::vector<unsigned int>& polygon, std::vector<Point>& points);
double pol_calc_area(std::vector<Point>& points);
bool is_ascending(I_Edge e);
bool is_ascending(unsigned int start, unsigned int stop, unsigned int cycle);
unsigned int get_lower_cyclic_difference(unsigned int a, unsigned int b, unsigned int cycle);
unsigned int get_chain_length(Ends ends, unsigned int cycle);
double pol_calc_chain_length(unsigned int start, unsigned int stop, std::vector<unsigned int>& polygon, std::vector<Point>& points);
bool is_2D(std::vector<unsigned int>& polygon, std::vector<Point>& points);
bool is_2D(Ends ends, std::vector<unsigned int>& polygon, std::vector<Point>& points);
void get_hole_and_new_pol(std::vector<unsigned int>& hole, std::vector<unsigned int>& new_polygon, E_Edge& h_e, std::vector<unsigned int>& polygon, std::vector<Point>& points);
bool get_inner_chain_polygon(std::vector<unsigned int>& inner_polygon, Ends& ends, std::vector<unsigned int>& polygon);
void get_inner_chain_points(std::vector<Point>& inner_points, std::vector<unsigned int>& inner_polygon, std::vector<Point>& points);
void createRandPol(std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int randseed);
void createCHRandPol(std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int randseed);
void get_inner_chains_to_ch(std::vector<Ends>& ends, std::vector<unsigned int>& ch, std::vector<unsigned int>& polygon, std::vector<Point>& points);
bool checkAllIntersections (std::vector<unsigned int>& polygon, std::vector<Point>& points);

#endif

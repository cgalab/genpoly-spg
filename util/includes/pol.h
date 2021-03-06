#ifndef __POL_H_
#define __POL_H_

#include "point.h"
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
    os << "e1: " << p.par.first << ", e2: " << p.par.second << ", inner_points: " << p.nr_holes;
    return os;
  }
};

void update_lowest_index(Point *a, unsigned int& lowest_index, std::vector<unsigned int>& polygon, std::vector<Point>& points);
void update_highest_index(Point *a, unsigned int& highest_index, std::vector<unsigned int>& polygon, std::vector<Point>& points);
void polSwap(Edge e, std::vector<unsigned int>& polygon);
void polSwap(Point* a, Point* b, std::vector<unsigned int>& polygon);
bool collSwap (Point *a, Point *b, Point *c, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points);
bool coll3Swap(Point *a, Point *b, Point *c, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points);
//bool coll3SwapOld(Point *a, Point *b, Point *c, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points);
bool coll3Sort(Point *a, Point *b, Point *c, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points);
bool coll3Sort(Point *a, Point *b, Point *c, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int& lowest_index);
bool coll3Sort(Point *a, Point *b, Point *c, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int& lowest_index, unsigned int& highest_index);
bool coll3Sort(Point *a, Point *b, Point *c, Point *idx, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int& lowest_index);
bool coll3Sort(Point *a, Point *b, Point *c, Point *idx, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int& lowest_index, unsigned int& highest_index);
bool coll3Sort2(Point *a, Point *b, Point *c, Point *idx, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int& lowest_index);
bool coll3Sort2(Point *a, Point *b, Point *c, Point *idx, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int& lowest_index, unsigned int& highest_index);
bool coll3Sort3(Point *a, Point *b, Point *c, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int& lowest_index);
bool coll3Sort3(Point *a, Point *b, Point *c, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int& lowest_index, unsigned int& highest_index);
bool coll3Sort4(Point *a, Point *b, Point *c, std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int& lowest_index);
bool coll3Sort5(Point *a, Point *b, Point *c, std::set<Edge2>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points);
bool coll3Sort5(Point *a, Point *b, Point *c, std::set<Edge2>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int& lowest_index);
bool coll3Sort5(Point *a, Point *b, Point *c, std::set<Edge2>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int& lowest_index, unsigned int& highest_index);

bool coll3Swap(Point *a, Point *b, Point *c, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int& lowest_index);
bool coll3Swap(Point *a, Point *b, Point *c, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int& lowest_index, unsigned int& highest_index);
bool coll4Swap (Edge& e1, Edge& e2, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points);
//bool coll4SwapOld(Edge& e1, Edge& e2, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points);
bool coll4Swap (Edge& e1, Edge& e2, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int& lowest_index);
bool coll4Swap (Edge& e1, Edge& e2, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int& lowest_index, unsigned int& highest_index);
bool coll4Swap (Edge& e1, Edge& e2, Point *idx, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int& lowest_index);
bool coll4Swap (Edge& e1, Edge& e2, Point *idx, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int& lowest_index, unsigned int& highest_index);
bool coll4Swap2 (Edge& e1, Edge& e2, Point *idx, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int& lowest_index);
bool coll4Swap2 (Edge& e1, Edge& e2, Point *idx, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int& lowest_index, unsigned int& highest_index);
bool coll4Swap3 (Edge& e1, Edge& e2, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int& lowest_index, unsigned int& highest_index);
bool coll4Swap4 (Edge& e1, Edge& e2, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int& lowest_index);
bool coll4Swap5 (Edge2& e1, Edge2& e2, std::set<Edge2>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points);
bool coll4Swap5 (Edge2& e1, Edge2& e2, std::set<Edge2>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int& lowest_index);
bool coll4Swap5 (Edge2& e1, Edge2& e2, std::set<Edge2>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int& lowest_index, unsigned int& highest_index);
bool collSwap (Edge& e1, Edge& e2, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points);
double pol_calc_perimeter(std::vector<unsigned int>& polygon, std::vector<Point>& points);
double pol_calc_normalised_perimeter(std::vector<unsigned int>& polygon, std::vector<Point>& points);
double pol_calc_area(std::vector<unsigned int>& polygon, std::vector<Point>& points);
double pol_calc_area(std::vector<Point>& points);
double calc_ch_area(std::vector<Point>& points);
double calc_ch_area_ratio(std::vector<unsigned int>& polygon, std::vector<Point>& points);
bool is_ascending(I_Edge e);
bool is_ascending(unsigned int start, unsigned int stop, unsigned int cycle);
unsigned int get_lower_cyclic_difference(unsigned int a, unsigned int b, unsigned int cycle);
unsigned int get_chain_length(Ends ends, unsigned int cycle);
double pol_calc_chain_length(unsigned int start, unsigned int stop, std::vector<unsigned int>& polygon, std::vector<Point>& points);
bool is_2D(std::vector<unsigned int>& polygon, std::vector<Point>& points);
bool is_2D(Ends ends, std::vector<unsigned int>& polygon, std::vector<Point>& points);
bool is_2D(E_Edge candidate, std::vector<unsigned int>& polygon, std::vector<Point>& points, bool is_hole);
bool is_2D(E_Edge candidate, std::vector<Point>& points, bool is_chain);
//void get_hole_and_new_pol(std::vector<unsigned int>& hole, std::vector<unsigned int>& new_polygon, E_Edge& e, std::vector<unsigned int>& polygon, std::vector<Point>& points);
//void get_hole_and_new_pol(std::vector<unsigned int>& hole, std::vector<unsigned int>& new_polygon,
//                        E_Edge& e, std::vector<unsigned int>& polygon, std::vector<Point>& points, bool is_hole);
bool get_hole_and_new_pol(E_Edge& e, std::vector<std::vector<unsigned int>>& sph, std::vector<Point>& points, unsigned int hole_selection, bool is_hole);
//void get_new_inner_polygon(Ends& end, std::vector<unsigned int>& new_inner_polygon, std::vector<unsigned int>& new_polygon, std::vector<Point>& points);
bool get_inner_chain_polygon(std::vector<unsigned int>& inner_polygon, Ends& ends, std::vector<unsigned int>& polygon);
void get_inner_chain_points(std::vector<Point>& inner_points, std::vector<unsigned int>& inner_polygon, std::vector<Point>& points);
void createRandPol(std::vector<unsigned int>& polygon, std::vector<Point>& points);
void createCHRandPol(std::vector<unsigned int>& polygon, std::vector<Point>& points);
void get_valid_inner_chains_to_ch(std::vector<Ends>& ends, std::vector<unsigned int>& ch, std::vector<unsigned int>& polygon, std::vector<Point>& points);
void update_end(unsigned int end_i, std::vector<Ends>& ends, std::vector<unsigned int>& ch, std::vector<unsigned int>& polygon, std::vector<Point>& points);
bool check_in_orientation (E_Edge e1, std::vector<Point>& points);
bool checkAllIntersections (std::vector<unsigned int>& polygon, std::vector<Point>& points);
void get_convex_hull(std::vector<unsigned int>& ch, std::vector<unsigned int>& polygon, std::vector<Point>& points);
bool shift_polygon(unsigned int point_index, unsigned int vertex_index, std::vector<std::vector<unsigned int>>& sph, std::vector<Point>& points);
void order_polygon(std::vector<std::vector<unsigned int>>& sph, std::vector<Point>& points);

#endif

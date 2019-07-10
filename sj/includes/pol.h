#ifndef __POL_H_
#define __POL_H_

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

double pol_calc_area(std::vector<unsigned int>& polygon, std::vector<Point>& points);
bool is_ascending(I_Edge e);
unsigned int get_lower_cyclic_difference(unsigned int a, unsigned int b, unsigned int cycle);
unsigned int get_chain_length(Ends ends, unsigned int cycle);
bool is_2D(std::vector<unsigned int>& polygon, std::vector<Point>& points);
bool is_2D(Ends ends, std::vector<unsigned int>& polygon, std::vector<Point>& points);
bool get_inner_polygon(std::vector<unsigned int>& inner_polygon, Ends& ends, std::vector<unsigned int>& polygon);
void get_convex_hull(std::vector<unsigned int>& ch, std::vector<Point>& points, bool enforceCCWOrder=false);
void get_inner_points(std::vector<unsigned int>& ip, std::vector<unsigned int>& ch, std::vector<Point>& points);
void createRandPol(std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int randseed);
void createCHRandPol(std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int randseed);
unsigned int get_lower_cyclic_difference(unsigned int a, unsigned int b, unsigned int cycle);
void get_inner_chains_to_ch(std::vector<Ends>& ends, std::vector<unsigned int>& ch, std::vector<unsigned int>& polygon, std::vector<Point>& points);
bool checkAllIntersections (std::vector<unsigned int>& polygon, std::vector<Point>& points);

#endif

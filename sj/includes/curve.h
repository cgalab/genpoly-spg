#ifndef __CURVE_H_
#define __CURVE_H_

//#include <vector>

class Curve {
private:
protected:
public:
  // index into 'points' vector, which is the lexicographical starting point
  // for the curves associated with this starting point
  unsigned int lsp;

  // if the left side of the curves with this lex. start. point is "outside" the
  // simple polygon and thus right side is "inside", this boolean is 'true'.
  bool bin;

  // a vector of 'E_Edge's (derived from D_Edge) that contain what edges are valid candidates.
  std::vector<E_Edge> edges;

  Curve() {lsp=0;bin=true;edges.clear();}

  friend std::ostream& operator<<(std::ostream& os, const Curve& c) {
		os << "lsp: " << c.lsp << ", bin: " << (c.bin ? "true" : "false") << "\n";
    os << "valid edges: \n";
    for (unsigned int i = 0; i < c.edges.size();++i) os << c.edges[i] << ", closest: " << c.edges[i].closest << "\n";
		return os;
	}
};

class s_curve {
public:
  // index into 'points' vector, which is the lexicographical starting point
  // for the curves associated with this starting point
  unsigned int lsp;

  // if the left side of the curves with this lex. start. point is "outside" the
  // simple polygon and thus right side is "inside", this boolean is 'true'.
  bool rin;

  // a vector of all curves that have the same lex. start. point.
  // this is defined as a pair of 'C_Edge's that represent the 'ends' of the curve.
  // end.first is the upper end and end.second is the lower end based on 'y' value.
  std::vector< std::pair<C_Edge, C_Edge> > ends;

  s_curve() {lsp=0;rin=true;}

  friend std::ostream& operator<<(std::ostream& os, const s_curve& c) {
		os << "lsp: " << c.lsp << ", rin: " << (c.rin ? "true" : "false") << "\n";
    for (unsigned int i = 0; i < c.ends.size();++i) os << "upper: " << c.ends[i].first << ", lower: " << c.ends[i].second << "\n";
		return os;
	}
};

enum error curve(std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int randseed);
enum error holes (std::vector<std::vector<unsigned int>>& sph, std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int nr_holes);
bool get_bin(E_Edge& e1, E_Edge& e2, std::set<E_Edge>::iterator& retval1, std::set<E_Edge>::iterator& retval2, std::set<E_Edge>& y_set, std::vector<Curve> curves);
bool check_ix_edges(E_Edge e1, E_Edge e2);
E_Edge get_inc_edge_from_set(E_Edge& e, std::vector<Curve>& curves, std::set<E_Edge>::iterator it);
void update_edge_in_set(E_Edge& e, std::pair<std::set<E_Edge>::iterator, bool>& retval, std::set<E_Edge>& y_set);
E_Edge inner_holes (std::vector<unsigned int>& polygon, std::vector<Point>& points);
enum error inner_holes_old(std::vector<std::vector<unsigned int>>& sph, std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int nr_holes, bool inner_bool);
enum error holes2(std::vector<std::vector<Point>>& sph, std::vector<Point>& points, std::vector<unsigned int>& polygon, unsigned int randseed, unsigned int nr_holes);

#endif

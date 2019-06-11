#ifndef __CURVE_H_
#define __CURVE_H_

//#include <vector>

class s_curve {
public:
  // index into 'points' vector, which is the lexicographical starting point
  // for the curves associated with this starting point
  unsigned int lsp;

  // if the left side of the curves with this lex. start. point is "outside" the
  // simple polygon and thus right side is "inside", this boolean is 'true'.
  bool rin;

  // a vector of all curves that have the same lex. start. point.
  // this is defined as a pair of indexes into 'points' that represent the 'ends' of the curve.
  // end.first is the upper end and end.second is the lower end based on 'y' value.
  std::vector< std::pair<unsigned int, unsigned int> > ends;

  friend std::ostream& operator<<(std::ostream& os, const s_curve& c) {
		os << "lsp: " << c.lsp << ", rin: " << ((c.rin) ? "true": "false") << "\n";
    for (unsigned int i = 0; i < c.ends.size();++i) os << "upper: " << c.ends[i].first << ", lower: " << c.ends[i].second << "\n";
		return os;
	}
};

enum error curve(std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int randseed);
enum error holes(std::vector<std::vector<unsigned int>>& sph, std::vector<unsigned int>& polygon, std::vector<Point>& points);

#endif

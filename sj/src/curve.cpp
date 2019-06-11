#include <iostream> // for endl
#include <vector>
#include <set>
#include <assert.h>
#include <utility> // for std::pair
#include "basicDefinitions.h"
#include "point.h"
#include "edge.h"
#include "curve.h"


enum error curve(std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int randseed) {
  ++randseed;

  //start with creating a vector for the lexicographically sorted indexes of 'points'
  std::vector<unsigned int> lex (points.size());
  fill_lex(lex, points); // fill 'lex' with the indexes

  std::cerr << "first point: " << points[lex[0]] << ", pol: " << polygon.size() << std::endl;


  return UNEXPECTED_ERROR;
}

// function that accepts a simple polygon and returns a array of a polygon with its holes.
// Input: 'polygon'     : a vector with vertices of 'points' set that is a simple polygon
//        'points'      : a vector of <Point> objects
// Output: 'pol_array'  : an array of polygons, the first index is the outermost simple polygon, the rest are simple holes inside that polygon
enum error holes(std::vector<std::vector<unsigned int>>& sph, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  std::vector<s_curve> sc;
  std::set<C_Edge> edgeS;
  std::pair<std::set<C_Edge>::iterator, bool> retval1, retval2; // return values for the 'edgeS' set.
  Point *m, *l, *r;
  bool isll, isrl;
  unsigned int count_open=0, count_cont=0, count_close=0;

  //start with creating a vector for the lexicographically sorted indexes of 'points'
  std::vector<unsigned int> lex (points.size());
  fill_lex(lex, points); // fill 'lex' with the indexes

  std::cerr << "sph: " << sph.size() << std::endl;

  // go through all the points in lex. order
  for (unsigned int i = 0; i < lex.size(); ++i) {
    m = &points[lex[i]];
    l = &points[polygon[(points.size() + (*m).v - 1) % points.size()]];
    r = &points[polygon[(points.size() + (*m).v + 1) % points.size()]];
    //std::cerr << "m: " << *m << ", l: " << *l << ", r: " << *r << std::endl;

    // create 2 new c_edge
    C_Edge e1 = C_Edge (m, l);
    C_Edge e2 = C_Edge (m, r);
    if (e1 < e2) {
      e1.lower = true;
      e2.lower = false;
    }
    else {
      e1.lower = false;
      e2.lower = true;
    }
    std::cerr << "e1: " << e1 << ", e2: " << e2 << std::endl;

    // check for 'o<', '-o-', '>o' condition
    (*m < *l) ? isll = false : isll = true;
    (*m < *r) ? isrl = false : isrl = true;

    if (isll && isrl) { // '>o'
      ++count_close;

      // find 'e1' and 'e2' in 'edgeS'
      // continue the old curve ends to 'm'
      // create a new pair of 'points' indexes with the open ends of the old s_curves
      // push the pair to the s_curve with lower 'lsp'
    }
    else if (isll ^ isrl) { // '-o-'
      ++count_cont;

      if (*e1.p1 < *e2.p2) {
        // find 'e1' in 'edgeS'
        retval1.first = edgeS.find(e1);
        assert(*(retval1.first) == e1);
        // copy values from iterator to 'e2'
        e2.sc = (*retval1.first).sc;
        e2.par = (*retval1.first).par;
        e2.lower = (*retval1.first).lower;
        //update s_curve.ends with new endpoint.
        std::cerr << "in here" << std::endl;
        std::cerr << "s_curve index: " << e2.sc << ", pair index: " << e2.par << std::endl;
        std::cerr << "current upper end: " << sc[e2.sc].ends[e2.par].first << std::endl;
        if (e2.lower) sc[e2.sc].ends[e2.par].second = (*e2.p2).i;
        else          sc[e2.sc].ends[e2.par].first  = (*e2.p2).i;
        // remove e1, add e2 into 'edgeS'
        edgeS.erase(retval1.first);
        retval2 = edgeS.insert(e2);
        assert(*retval2.first == e2);

      }
      else {
        // find 'e2' in 'edgeS'
        retval2.first = edgeS.find(e2);
        assert(*retval2.first == e2);
        // copy values from iterator to 'e2'
        e1.sc = (*retval2.first).sc;
        e1.par = (*retval2.first).par;
        e1.lower = (*retval2.first).lower;
        //update s_curve.ends with new endpoint.
        if (e1.lower) sc[e1.sc].ends[e1.par].second = (*e1.p2).i;
        else          sc[e1.sc].ends[e1.par].first  = (*e1.p2).i;
        // remove e1, add e2 into 'edgeS'
        edgeS.erase(retval2.first);
        retval1 = edgeS.insert(e1);
        assert(*retval1.first == e1);
      }
    }
    else { // 'o<'
      ++count_open;

      // create a new s_curve
      s_curve nu_c;
      nu_c.lsp = (*m).i;
      e1.sc = sc.size();
      e1.par = 0;
      e2.sc = sc.size();
      e2.par = 0; // opening a new curve means these edges are the first pair.

      // insert both c_edge into y-set
      retval1 = edgeS.insert(e1);
      assert(*retval1.first == e1);
      assert(retval1.second == true);
      retval2 = edgeS.insert(e2);
      assert(*retval2.first == e2);
      assert(retval2.second == true);
      // check edge above and below the 2 edges in 'y' set for 'rin' value and set for the s_curve
      if (e1.lower) {
        C_Edge before, after;
        bool bef=false;
        // check if e1 is the lowest edge in 'edgeS'
        if (retval1.first != edgeS.begin()) {
          before = *(std::prev(retval1.first));
          bef = true;
        }
        else nu_c.rin = true;
        // check if e2 is highest edge in 'edgeS'
        if (bef && (retval2.first != --edgeS.end())) {
          after = *(std::next(retval2.first));
          // if curve has other curves on both sides,
          // check upper/lower orientation of adjacent s_curves
          if (before.lower == after.lower) {
            if (before.lower == false) nu_c.rin = !sc[before.sc].rin;
            else if (after.lower == true) nu_c.rin = !sc[after.sc].rin;
          }
        }
        else nu_c.rin = true;

        // set 'l' and 'r' as endpoints of the s_curve
        std::pair<unsigned int, unsigned int> pp ((*r).i, (*l).i);
        nu_c.ends.push_back(pp);
      }
      else {
        C_Edge before, after;
        bool bef=false;
        // check if e2 is the lowest edge in 'edgeS'
        if (retval2.first != edgeS.begin()) {
          before = *(std::prev(retval2.first));
          bef = true;
        }
        else nu_c.rin = true;
        // check if e1 is highest edge in 'edgeS'
        if (bef && (retval1.first != --edgeS.end())) {
          after = *(std::next(retval1.first));
          // if curve has other curves on both sides,
          // check upper/lower orientation of adjacent s_curves
          if (before.lower == after.lower) {
            if (before.lower == false) nu_c.rin = !sc[before.sc].rin;
            else if (after.lower == true) nu_c.rin = !sc[after.sc].rin;
          }
        }
        else assert(nu_c.rin == true);

        // set 'l' and 'r' as endpoints of the s_curve
        std::pair<unsigned int, unsigned int> pp ((*l).i, (*r).i);
        nu_c.ends.push_back(pp);
      }

      // push s_curve to the 'sc' vector.
      sc.push_back(nu_c);
    }

    std::cout << "edges in 'edgeS':" << std::endl;
    for (std::set<C_Edge>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;
  }

  std::cerr << "opened: " << count_open << ", continued: " << count_cont << ", closed: " << count_close << std::endl;
  for (unsigned int i = 0; i < sc.size(); ++i) std::cerr << sc[i] << std::endl;

  return SUCCESS;
}

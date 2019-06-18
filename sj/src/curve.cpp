#include <iostream> // for endl
#include <vector>
#include <set>
#include <assert.h>
#include <utility> // for std::pair
#include <math.h>  // for signbit
#include "basicDefinitions.h"
#include "point.h"
#include "edge.h"
#include "curve.h"
#include "pol.h"


enum error curve(std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int randseed) {
  ++randseed;

  //start with creating a vector for the lexicographically sorted indexes of 'points'
  std::vector<unsigned int> lex (points.size());
  fill_lex(lex, points); // fill 'lex' with the indexes

  std::cerr << "first point: " << points[lex[0]] << ", pol: " << polygon.size() << std::endl;

  return UNEXPECTED_ERROR;
}

// Found a better way to find holes based on this theorem:
// Theorem of inner curves:  Every point on the convex hull is either connected to its incidental c.h. point directly,
// or via an inner curve that ends in the incidental c.h. point.
// This means we can traverse the c.h. points and find hole candidates from the start of all inner curves.
enum error holes2(std::vector<std::vector<unsigned int>>& sph, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  std::vector< std::pair<I_Edge,I_Edge> > ends;
  Point prev, p, next;
  bool is_left, inner;
  unsigned int diff;

  // start with getting all c.h. points.
  std::vector<unsigned int> ch;
  get_convex_hull(ch, points, true);

  double area = pol_calc_area(ch, points);
	std::cerr << "Area: " << area << std::endl;

  std::cerr << "c.h. points: " << ch.size() << ", inner points: " << points.size()-ch.size() << ", sph: " << sph.size() << ", p: " << polygon.size() << std::endl;

  // check to see how many inner points there are, if there are less than
  // the number of holes we want, just create the max amount of holes.

  // add the starting edges of all inner curves of the c.h. to 'ends' vector
  prev = points[ch[ch.size()-1]];
  for (unsigned int i = 0; i < ch.size(); ++i) {
    prev = points[ch[(ch.size() + i - 1) % ch.size()]];
    p = points[ch[i]];
    next = points[ch[(ch.size() + i + 1) % ch.size()]];

    // get the difference in index distance between 'prev' and 'p'
    diff = get_cyclic_difference(p.v, prev.v, polygon.size());
//    std::cerr << "diff: " << diff << std::endl;
    //diff = 1 means the 2 c.h. points are connected by an edge.
    if (diff > 1) {
//      std::cerr << "prev: " << prev << std::endl;
//      std::cerr << "p: " << p << std::endl;
//      std::cerr << "next: " << next << std::endl;

      // 'p' and 'prev' create a 'inner' polygonal chain and an 'outer' polygonal chain.
      // if 'next' is inside the 'inner' p. chain, then the inner curve defined by the 2 c.h. points is the 'outer' p. chain
      if (p.v > prev.v) {
        is_left = true;
        // check if 'next' is either lower than 'p' and higher than 'next'
        if ((next.v < p.v) && (next.v > prev.v)) inner = false; //do not use the inner boundary between 'p' and 'prev'
        else inner = true;
      }
      else {
        is_left = false;
        if ((next.v > p.v) && (next.v < prev.v)) inner = false; //do not use the inner boundary between 'p' and 'prev'
        else inner = true;
      }

      // create the edges
      I_Edge e1, e2;
      if (is_left ^ inner) {
        e1 = I_Edge (&points[polygon[prev.v]], &points[polygon[(polygon.size() + prev.v - 1) % polygon.size()]]);
        e2 = I_Edge (&points[polygon[p.v]], &points[polygon[(polygon.size() + p.v + 1) % polygon.size()]]);
      }
      else {
        e1 = I_Edge (&points[polygon[prev.v]], &points[polygon[(polygon.size() + prev.v + 1) % polygon.size()]]);
        e2 = I_Edge (&points[polygon[p.v]], &points[polygon[(polygon.size() + p.v - 1) % polygon.size()]]);
      }
      // set the l2ch boolean of the edges
      if (*e1.p1 == prev) e1.l2ch = true;
      if (*e2.p1 == p) e2.l2ch = true;
//      std::cerr << "e1: " << e1 << ", e2: " << e2 << std::endl;
      std::pair<I_Edge, I_Edge> par (e1, e2);
      ends.push_back(par);

    }


  }
  std::cerr << "ends: " << std::endl;
  for (unsigned int i=0; i < ends.size(); ++i) std::cerr << "e1: " << ends[i].first << ", e2: " << ends[i].second << std::endl;



  return SUCCESS;
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

  // go through all the points in lex. order, except the last.  We don't need to process the last vertex.
  for (unsigned int i = 0; i < lex.size()-1; ++i) {
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
//    std::cerr << "e1: " << e1 << ", e2: " << e2 << std::endl;

    // check for 'o<', '-o-', '>o' condition
    (*m < *l) ? isll = false : isll = true;
    (*m < *r) ? isrl = false : isrl = true;

    if (isll && isrl) {
//      std::cerr << "=== >o ===" << std::endl;
      ++count_close;
      C_Edge new_end1, new_end2;
      std::pair<C_Edge, C_Edge> nu_p;
      unsigned int nu_sc;

      // find 'e1' and 'e2' in 'edgeS'
      retval1.first = edgeS.find(e1);
      assert(*(retval1.first) == e1);
      retval2.first = edgeS.find(e2);
      assert(*(retval2.first) == e2);
      // e1 and e2 only have vertex info, copy the rest from iterators
      e1 = *retval1.first;
      e2 = *retval2.first;
//      std::cerr << "first copy: e1: " << e1 << ", e2: " << e2 << std::endl;
      // continue the old curve ends to 'm', grab the open ends for a new curve
//      std::cerr << "sc[e1.sc].ends[e1.par].first: " << sc[e1.sc].ends[e1.par].first << std::endl;
//      std::cerr << "sc[e1.sc].ends[e1.par].second: " << sc[e1.sc].ends[e1.par].second << std::endl;
//      std::cerr << "sc[e2.sc].ends[e2.par].first: " << sc[e2.sc].ends[e2.par].first << std::endl;
//      std::cerr << "sc[e2.sc].ends[e2.par].second: " << sc[e2.sc].ends[e2.par].second << std::endl;
      if (e1.lower) new_end1 = sc[e1.sc].ends[e1.par].first;
      else new_end1 = sc[e1.sc].ends[e1.par].second;
      if (e2.lower) new_end2 = sc[e2.sc].ends[e2.par].first;
      else new_end2 = sc[e2.sc].ends[e2.par].second;

//      std::cerr << "new end 1: " << new_end1 << ", new end2: " << new_end2 << std::endl;
      // finished with the 2 edges, can be removed from 'edgeS'
      edgeS.erase(retval1.first);
      edgeS.erase(retval2.first);
//      std::cout << "1dges in 'edgeS':" << std::endl;
//      for (std::set<C_Edge>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;
      // set the 'sc' and 'par' indices
      if (e1.sc < e2.sc) nu_sc = e1.sc;
      else nu_sc = e2.sc;
      new_end1.sc = nu_sc;
      new_end1.par = sc[nu_sc].ends.size();
      new_end2.sc = nu_sc;
      new_end2.par = sc[nu_sc].ends.size();
      // set a pair of 'C_Edge's with the open ends of the old s_curves
      if (new_end1 < new_end2) {
        new_end1.lower = true;
        new_end2.lower = false;
        nu_p.first = new_end2;
        nu_p.second = new_end1;
      }
      else {
        new_end1.lower = false;
        new_end2.lower = true;
        nu_p.first = new_end1;
        nu_p.second = new_end2;
      }
      // push the pair to the s_curve with lower 'lsp'
      sc[nu_sc].ends.push_back(nu_p);
      //find the 2 edges in 'edgeS'
      retval1.first = edgeS.find(new_end1);
      assert(*retval1.first == new_end1);
      retval2.first = edgeS.find(new_end2);
      assert(*retval2.first == new_end2);
      // remove the edges from 'edgeS'
      edgeS.erase(retval1.first);
      edgeS.erase(retval2.first);
//      std::cout << "3dges in 'edgeS':" << std::endl;
//      for (std::set<C_Edge>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;
      retval1 = edgeS.insert(new_end1);
      retval2 = edgeS.insert(new_end2);
//      std::cout << "3dges in 'edgeS':" << std::endl;
//      for (std::set<C_Edge>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;
      assert (*retval1.first == new_end1);
      assert (retval1.second);
      assert (*retval2.first == new_end2);
      assert (retval2.second);
    }
    else if (isll ^ isrl) { // === '-o-' ===
//      std::cerr << "=== -o- ===" << std::endl;
      ++count_cont;

      if (*e1.p1 < *e2.p2) {
        // find 'e1' in 'edgeS'
        retval1.first = edgeS.find(e1);
        assert(*(retval1.first) == e1);
        // copy values from iterator to 'e2'
        e2.sc = (*retval1.first).sc;
        e2.par = (*retval1.first).par;
        e2.lower = (*retval1.first).lower;
        //update s_curve.ends with new edge.
        if (e2.lower) sc[e2.sc].ends[e2.par].second = e2;
        else          sc[e2.sc].ends[e2.par].first  = e2;
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
        if (e1.lower) sc[e1.sc].ends[e1.par].second = e1;
        else          sc[e1.sc].ends[e1.par].first  = e1;
        // remove e1, add e2 into 'edgeS'
        edgeS.erase(retval2.first);
        retval1 = edgeS.insert(e1);
        assert(*retval1.first == e1);
      }
    }
    else {
//      std::cerr << "=== o< ===" << std::endl;
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
          if (before.lower == after.lower) { // 2 inner curves inside an outer curve
            if (before.lower == false) nu_c.rin = !sc[before.sc].rin; // the inner upper curve
            else nu_c.rin = sc[before.sc].rin; // the inner lower curve
          }
          else if (before.sc == after.sc) nu_c.rin = !sc[before.sc].rin; // one inner curve encapsulated by an outer curve
          else nu_c.rin = sc[before.sc].rin; // between 2 incidental inner curves
        }
        else nu_c.rin = true;

        // set 'l' and 'r' as endpoints of the s_curve
        std::pair<C_Edge, C_Edge> pp (e2, e1);
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
            else nu_c.rin = sc[before.sc].rin;
          }
          else if (before.sc == after.sc) nu_c.rin = !sc[before.sc].rin;
          else nu_c.rin = sc[before.sc].rin;
        }
        else assert(nu_c.rin == true);

        // set 'l' and 'r' as endpoints of the s_curve
        std::pair<C_Edge, C_Edge> pp (e1, e2);
        nu_c.ends.push_back(pp);
      }

      // push s_curve to the 'sc' vector.
      sc.push_back(nu_c);
    }

//    std::cout << "edges in 'edgeS':" << std::endl;
//    for (std::set<C_Edge>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;
  }

  // Analysis of found curves in 'sc'
  std::cerr << "opened: " << count_open << ", continued: " << count_cont << ", closed: " << count_close << std::endl;
  unsigned int valid_curves = 0;
  for (unsigned int i = 0; i < sc.size(); ++i) {
    if (sc[i].rin == false) {
      ++valid_curves;
      std::cerr << sc[i] << std::endl;
    }
  }

  std::cerr << "valid curves to make holes out of: " << valid_curves << std::endl;


  return SUCCESS;
}

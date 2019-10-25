#include <iostream> // for endl
#include <vector>
#include <set>
#include <assert.h>
#include <utility> // for std::pair
#include <math.h>  // for signbit
#include <iterator>     // std::next
#include "basicDefinitions.h"
#include "point.h"
#include "edge.h"
#include "curve.h"
#include "pol.h"
#include "opt2g.h" //needed to create polygons.
#include "rand.h"



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
enum error holes2(std::vector<std::vector<Point>>& sph, std::vector<Point>& points, std::vector<unsigned int>& polygon, unsigned int randseed, unsigned int nr_holes) {
  assert(sph.size() == 0);

  // start with getting all c.h. points.
  std::vector<Point> ch;
  get_convex_hull(ch, points);

  //double area = pol_calc_area(ch, points);

  unsigned int nr_inner = points.size()-ch.size();
	std::cerr << "holes: " << nr_holes << ", pol.size: " << polygon.size() << ", randseed: " << randseed << std::endl;

  std::cerr << "c.h. points: " << ch.size() << ", inner points: " << nr_inner << ", sph: " << sph.size() << std::endl;

  //if there are less than 3 inner points
  if (points.size()-ch.size() < 3) return TOO_FEW_INNER_POINTS_FOR_HOLE;
  if (points.size()-ch.size() == 3) {
    // get inner points
    std::vector<Point> ip;
    get_inner_points(ip, ch, points);
    // append ch as the first vector of indexes to sph
    sph.push_back(ch);
    // append the inner points vector of indexes to sph
    sph.push_back(ip);
    return SUCCESS;
  }

  if (points.size()-ch.size() > 3) {
/*
    //std::vector<unsigned int> polygon;
    std::vector<Ends> ends;

    bool strict;
    unsigned int total_holes = 0;

    // if true, any result that has 1 or more holes will be returned
    // else try and return exactly 'nr_holes' holes
    if ((unsigned int)(nr_inner/3) < nr_holes) strict = false;
    else strict = true;

    do {
      // get a simple polygon to work with.
      if (polygon.size() == 0) opt2g(polygon, points, randseed);
      //set the simple polygon as the first polygon in 'sph'
      for (unsigned int i = 0; i < polygon.size(); ++i) sph[0].push_back(points[polygon[i]]);
      std::cerr << "=== The points in the simple polygon permutation ===" << std::endl;
      pdisplay(sph[0]);

      // add the starting edges of all inner curves of the c.h. to 'ends' vector
      get_inner_chains_to_ch(ends, ch, polygon, points);

      std::cerr << "ends: " << std::endl;
      // now I have to go through the ends and make sure that there are enough points in each inner polygonal chain to create desired # of holes
      for (unsigned int i=0; i < ends.size(); ++i) {
        // is_2D checks to see if the chain is somewhere non-collinear.
        if (is_2D(ends[i], polygon, points)) {
//          std::cerr << "is 2D" << std::endl;
          // get length of inner polygonal chain
          unsigned int diff = get_chain_length(ends[i], polygon.size());
          ends[i].nr_holes = (int)(diff/3);
          std::cerr << ends[i] << std::endl;
          total_holes = total_holes + ends[i].nr_holes;
        }
        else {
//          std::cerr << "not 2D" << std::endl;
          ends.erase(std::next(ends.begin(),i));
        }
      }

      std::cerr << "total (theoretically) possible holes " << total_holes << std::endl;
      if (strict) {
        if (total_holes < nr_holes) {
          ends.clear();
          continue;
        }
      }
      else {
        if (total_holes == 0) {
          ends.clear();
          continue;
        }
      }



      for (unsigned int i=0; i < ends.size(); ++i) {
        std::cerr << "== Ends: " << i << "==" << std::endl;
        //create a vector of indexes that form the points of the inner pol. chain.
        std::vector<unsigned int> inner_polygon;
        get_inner_chain_polygon(inner_polygon, ends[i], polygon);
        //std::cerr << "== inner polygon: " << "==" << std::endl;
        //pdisplay(inner_polygon, points);
        std::vector<Point> inner_points;
        get_inner_chain_points(inner_points, inner_polygon, points);
        //std::cerr << "== inner points: " << "==" << std::endl;
        //pdisplay(inner_polygon, inner_points);
        // now we have the p.chain in inner_points and inner_polygon

        E_Edge h_e = inner_holes(inner_polygon, inner_points);
        std::cerr << "hole-edge: " << h_e << std::endl;

        std::vector<unsigned int> hole;
        std::vector<unsigned int> new_polygon;
        get_hole_and_new_pol(hole, new_polygon, h_e, polygon, points);
        sph.push_back(new_polygon);
        sph.push_back(hole);

        //std::cerr << "new polygon" << std:: endl;
        //pdisplay (new_polygon, points);
        //std::cerr << "hole:" << std::endl;
        //pdisplay(hole, points);
        break;
      }



      // future thoughts:
      // * if I always start with creating holes from the ends, what about creating
      //   more than one hole from an inner pol. chain?
      // * if I know how many holes I can theoretically pick from each unique primal edge pair,
      //   I can randomly pick between all holes the primal pairs can generate
      // * Let's say a primal pair has enough points to theoretically generate 5 holes
      //



    } while ((strict && total_holes < nr_holes) || total_holes == 0);
*/
    return SUCCESS;
  }
  return UNEXPECTED_ERROR;
}

// Function to return if "below" a polygonal chain is the "inside" side when creating a new curve.
// INPUT:
//  e1                : The 'E_Edge' you need the value of 'bin' returned
//  e2                : the incidental E_Edge being created which is connected to first point in e1.
//  y_set             : the linesweep structure for edges.
//  retval1, retval2  : iterators to the edges 'e1' and 'e2' in 'y_set'
//  inner_bool        : boolean for the polygon, whether it's an "inner polygonal chain" (true) or not.
bool get_bin(E_Edge& e1, E_Edge& e2, std::set<E_Edge>::iterator& retval1, std::set<E_Edge>::iterator& retval2, std::set<E_Edge>& y_set, std::vector<Curve> curves) {
  E_Edge before, after;
  bool bef;
  if (e1 < e2) {
    // check if e1 is the lowest edge in 'edgeS'
    if (retval1 != y_set.begin()) {
      before = *(std::prev(retval1));
      bef = true;
    }
    else return false; // at bottom, bin must be false.
    // check if e2 is highest edge in 'edgeS'
    if (bef && (retval2 != --y_set.end())) {
      after = *(std::next(retval2));
    }
    else return false; // curve at top, e1 the bottom chain, bin must be false.
  }
  else {
    // check if e2 is the lowest edge in 'edgeS'
    if (retval2 != y_set.begin()) {
      before = *(std::prev(retval2));
      bef = true;
    }
    else return true;
    // check if e1 is highest edge in 'edgeS'
    if (bef && (retval1 != --y_set.end())) {
      after = *(std::next(retval1));
    }
    else return true;
  }

  if (e1 < e2) {
    return !curves[before.curve_id].bin;
  }
  else {
    return !curves[after.curve_id].bin;
  }
}

// function to check the edge between p1 points of 'e' and 'e.closest'
// (as well as p2 points) for intersection with 'e2'.
bool check_ix_edges(E_Edge e1, E_Edge e2) {
  E_Edge ix;
  enum intersect_t isval; // return value for intersect check.

  ix = E_Edge(e1.p1, e1.closest.p1);
  isval = checkIntersection(ix, e2);
  if (isval >= IS_TRUE) return true;

  ix = E_Edge(e1.p2, e1.closest.p2);
  isval = checkIntersection(ix, e2);
  if (isval >= IS_TRUE) return true;
  return false;
}

//function to return the incidental edge of 'e' from the 'y_set'
E_Edge get_inc_edge_from_set(E_Edge& e, std::vector<Curve>& curves, std::set<E_Edge>::iterator it) {
  E_Edge inc_e;
  if (curves[e.curve_id].bin) inc_e = *(std::prev(it));
  else inc_e = *(std::next(it));
  return inc_e;
}

// function to update edge in set, i.e. find it, remove it and insert it again.
void update_edge_in_set(E_Edge& e, std::pair<std::set<E_Edge>::iterator, bool>& retval, std::set<E_Edge>& y_set) {
  assert(e == *retval.first);
  y_set.erase(retval.first);
  retval = y_set.insert(e);
  assert(*retval.first == e);
  assert(retval.second == true);
//  std::cerr << "r: e1: " << (*retval1.first) << ", e1.closest: " << (*retval1.first).closest << std::endl;
}

E_Edge inner_holes(std::vector<unsigned int>& polygon, std::vector<Point>& points) {

  //start with creating a vector for the lexicographically sorted indexes of 'points'
  std::vector<unsigned int> lex (polygon.size());
  fill_lex(lex, polygon, points);
//  pdisplay(lex, points);

  // I need to do a linesweep over the lex points
  // When an edge 'e' is freshly inserted, check which edge is 'before' and 'after'
  // for the 'before' and 'after' edges, add 'e' as the 'closest_before' or 'closest_after'
  // I want to skip the edge between [0] and [n-1] as that's the edge between the convex hull points.

  std::vector<Curve> curves; // container for curves and their valid edges to create holes from.
  std::set<E_Edge> y_set; // container for the planesweep structure.
  std::pair<std::set<E_Edge>::iterator, bool> retval1, retval2; // return values from the y_set 'insert' or 'erase' functions.
  Point *m, *l, *r; // m is the current point at lex. position 'i' and 'l' and 'r' are the 2 points it connects to by edges.
  bool isll, isrl; // are 'l' and 'r' points left of 'm'
  unsigned int count_open=0, count_cont=0, count_close=0; // just for development verification.

  for (unsigned int i = 0; i < lex.size()-1; ++i) {
    m = &points[lex[i]];
    l = &points[polygon[(polygon.size() + (*m).v - 1) % polygon.size()]];
    r = &points[polygon[(polygon.size() + (*m).v + 1) % polygon.size()]];
//    std::cerr << "m: " << *m << ", l: " << *l << ", r: " << *r << std::endl;

    // create 2 new 'E_Edge's
    E_Edge e1 = E_Edge (m, l);
    E_Edge e2 = E_Edge (m, r);

//    std::cerr << std::endl << "e1: " << e1 << ", e2: " << e2 << std::endl;
    // check for 'o<', '-o-', '>o' condition
    (*m < *l) ? isll = false : isll = true;
    (*m < *r) ? isrl = false : isrl = true;

    if (isll && isrl) {
    //  std::cerr << "=== >o ===" << std::endl;
      ++count_close;

      // Here we need to make a big decision..
      // Originally the inner polygonal chains were defined as 'curves' with
      // a starting point at the first lexicographical point of the curve (i.e. at a 'o<')
      // So when 2 curves joined, we had the end pairs saved so we can create
      // a new curve with the open ends of the 2 now joined curves.

      // If instead we create 2 chains from 'o<', I don't need to hunt down the ends.
      // The chain just has to know if "inner" is above or below it
      // so instead of 'rin' we can have 'bin' for "is below the chain the inner part"
      // and we create 2 chains at a 'o<' condition.

      // But.. bin is redundant with 'lower' of an D/E_Edge..
      // So we can remove 'lower' and just save 'bin' as a part of the new curve class.

      // so we just need to remove the 2 edges from the 'y_set'

      // Why not do any checks?
      // We do not need to do closest incidental edge check, see below.
      // We do not need to push it to valid edges to make curves as we know that
      // as the 2 edges are connecting in a point, it can never create a hole.

      // find 'e1' in 'y_set' and update with values of the edge in 'y_set'
      retval1.first = y_set.find(e1);
      assert(*(retval1.first) == e1);
      e1 = (*retval1.first);

      // find 'e2' in 'y_set'
      retval2.first = y_set.find(e2);
      assert(*(retval2.first) == e2);
      e2 = (*retval2.first);

      // need to check which is higher so I can assign them as each others incidental edges.
      if (e1 < e2) {
        if (curves[e2.curve_id].bin) {
          e1.closest = e2;
          e2.closest = e1;
        }
      } else {
        if (curves[e1.curve_id].bin) {
          e1.closest = e2;
          e2.closest = e1;
        }
      }

      // if the 2 edges have each other as closest edges, no need for processing
      if (e1.closest != e2 || e2.closest != e1) {
        if ((get_lower_cyclic_difference((*e1.p1).v, (*e1.closest.p1).v, polygon.size()) > 2) && (get_lower_cyclic_difference((*e1.p2).v, (*e1.closest.p2).v, polygon.size()))) {
          curves[e1.curve_id].edges.push_back(e1);
        }

        if ((get_lower_cyclic_difference((*e2.p1).v, (*e2.closest.p1).v, polygon.size()) > 2) && (get_lower_cyclic_difference((*e2.p2).v, (*e2.closest.p2).v, polygon.size()))) {
          curves[e2.curve_id].edges.push_back(e2);
        }
      }
      y_set.erase(e1);
      y_set.erase(e2);
    }
    else if (isll ^ isrl) {
      //std::cerr << "=== -o- ===" << std::endl;
      ++count_cont;

      E_Edge old_e, new_e;
      if (*e1.p1 < *e2.p2) {
        old_e = e1;
        new_e = e2;
      }
      else {
        old_e = e2;
        new_e = e1;
      }

      // find 'old_e' in 'y_set'
      retval1.first = y_set.find(old_e);
      assert(*(retval1.first) == old_e);

      // copy values from iterator to 'new_e'
      old_e = (*retval1.first);
      new_e.curve_id = (*retval1.first).curve_id;
//      std::cerr << "old: " << old_e << ", new: " << new_e << std::endl;
      //std::cerr << "begin(): " << *(y_set.begin()) << ", end()-1: " << *(std::prev(y_set.end())) << std::endl;

      // old_e needs to be removed, i.e. first find the incidental edges and
      // check conditions for 'last' and whether it's still the same curve.

      // I need to find which side (bef or aft) is the "inside"
      //bool inc_found = false; // NOT NECESSARY.
      E_Edge inc_e; // incidental edge of the old_e.
      inc_e = get_inc_edge_from_set(old_e, curves, retval1.first);
//      std::cerr << "inc_e: " << inc_e << std::endl;

      // Q: Do I have to make sure that incidental edges continuously only see the same curve?
      // If yes: that means a curve_id could start as an incidental curve,
      // then another curve_id could pop in, and then the original curve could be assigned as the end.
      // For really long edges that could happen.
      // So, the ultimate check would go through each curve pieces' [start, end] that was incidental to an edge,
      // This could mean that one edge has to be compared to all the others (but all the others had only this to be compared to)
      // 3 ways we can do it:
      // 1) each edge has a vector of [start,end] of all the curves it is incidental to, and we can randomly pick one and check validity
      // 2) grab the first or last curve it sees only (could be a nice first step that leads to 1) )
      // 3) at each incidental edge, compare the edges, and only the closest edge gets saved as a candidate for a hole intersection.
      // 3) might guarantee we do not need to check any intersections with the reset of the curve,
      // but it possibly throws out a lot of possible valid candidates.
      // As 3) seems the most simple currently, let's implement that.

      // with the old edge, if enough points are between the 2 edges, push oldest to curves.edges[]
      // with the new edge, it gets the incidental edge as closest edge.
      // with the incidental edge, compare current closest and new and pick the closer edge.

      // Do I need to update old_e.closest?
      // If yes: that means the current incidental edge had an edge between them
      // which is why it wasn't processed for the old edge
      // because new edges get the incidental edge as closest, and incidental edges
      // get old/new comparison as closest, so if the current incidental edge of old edge
      // wasn't processed with a comparison as a new or incidental edge, the current inc_e
      // was never the closest edge, so old_edge only needs a length check..

      // process 'old_e'
      if ((get_lower_cyclic_difference((*old_e.p1).v, (*old_e.closest.p1).v, polygon.size()) > 2) && (get_lower_cyclic_difference((*old_e.p2).v, (*old_e.closest.p2).v, polygon.size()) > 2)) {
        //std::cerr << "old_e: " << old_e << ", closest: " << old_e.closest << std::endl;
//        std::cerr << "pushing old edge to curve, lcd is: " << get_lower_cyclic_difference((*old_e.p1).v, (*inc_e.p1).v, polygon.size()) << std::endl;
        curves[old_e.curve_id].edges.push_back(old_e);
      }
      y_set.erase(retval1.first);

      // process 'new_e'
      new_e.closest = inc_e; // Is this ok?  Shouldn't I find where to put it first and get the real inc_e?

      // process 'inc_e' - reuse retval1 to find inc_e and update it if necessary.
      // need to add an intersection check to make sure that 'inc_e.closest' is
      // valid and doesn't have intersections with 'new_e'. if not then new_e is closest.

      retval1.first = y_set.find(inc_e);
      assert(*retval1.first == inc_e);
      if (check_ix_edges(inc_e, new_e)) {
        inc_e.closest = new_e;
        update_edge_in_set(inc_e, retval1, y_set);
//        std::cerr << "inc_e: " << inc_e << ", inc_e.closest: " << inc_e.closest << std::endl;
      }

      // then add new_e
      retval2 = y_set.insert(new_e);
      assert(*retval2.first == new_e);
      assert(retval2.second == true);
//      std::cerr << "new_e: " << new_e << ", new_e.closest: " << new_e.closest << std::endl;
    }
    else {
      //std::cerr << "=== o< ===" << std::endl;
      ++count_open;

      Curve new_curve1, new_curve2; // create a new Curve
      new_curve1.lsp = (*m).i; // assign index of first lex. point of curve
      new_curve2.lsp = (*m).i;
      e1.curve_id = curves.size(); // assign curve index to edge.
      e2.curve_id = curves.size()+1; // assign curve index to edge.

      // insert both 'E_Edge's into y_set
      retval1 = y_set.insert(e1);
      assert(*retval1.first == e1);
      assert(retval1.second == true);
      retval2 = y_set.insert(e2);
      assert(*retval2.first == e2);
      assert(retval2.second == true);

      // as we're still dealing with complexities of "in" in regards to curves, it's still dependent on incidental 'bin' values.
      new_curve1.bin = get_bin(e1, e2, retval1.first, retval2.first, y_set, curves);
      new_curve2.bin = get_bin(e2, e1, retval2.first, retval1.first, y_set, curves);
//      std::cerr << "curve1: " << new_curve1 << std::endl;
//      std::cerr << "curve2: " << new_curve2 << std::endl;
      curves.push_back(new_curve1);
      curves.push_back(new_curve2);

      // dependant on "inside" not being inside the 2 joined edges, we need to find the right 'closest' edges.
      e1.closest = get_inc_edge_from_set(e1, curves, retval1.first);
      update_edge_in_set(e1, retval1, y_set);
      e2.closest = get_inc_edge_from_set(e2, curves, retval2.first);
      update_edge_in_set(e2, retval2, y_set);

      // if closest edge isn't one of the 2 new edges, for 'closest' edge:
      // do a check to see whether e1/e2 is closer than its 'closest' edge.
      // possibilities are determinant check, intersect check, or just euclidean metric.
      // - Technically I don't care what is closer, I care if the new edges intersect with the old closest.
      if ((*retval1.first).closest != e2) {
        E_Edge inc_e = get_inc_edge_from_set(e1, curves, retval1.first);
//        std::cerr << "inc_e: " << inc_e << ", inc_e.closest: " << inc_e.closest << std::endl;
        if (check_ix_edges(inc_e, e1)) {
          // e1 should be assigned closest as it intersects.
          inc_e.closest = e1;
          // need to update the inc_e edge in y_set:
          retval1.first = y_set.find(inc_e);
          update_edge_in_set(inc_e, retval1, y_set);
        }
        // check the closest edge of e2:
        inc_e = get_inc_edge_from_set(e2, curves, retval2.first);
        if (check_ix_edges(inc_e, e2)) {
          // e2 should be assigned closest as it intersects
          inc_e.closest = e2;
          // need to update the inc_e edge in y_set:
          retval2.first = y_set.find(inc_e);
          update_edge_in_set(inc_e, retval2, y_set);
        }
      }
      //  I could add a metric if no intersection, which has larger lcd value.. might get confusing with 'closest.closest.p1'
    }

//    std::cout << "edges in 'y_set':" << std::endl;
//    for (std::set<E_Edge>::iterator it=y_set.begin(); it!=y_set.end(); ++it) std::cerr << *it << ", closest: " << (*it).closest << std::endl;
  }

//  std::cerr << "curves: " << std::endl;
  unsigned int count = 0;
  for (unsigned int i = 0; i < curves.size();++i) count = count + curves[i].edges.size(); //std::cerr << curves[i] << std::endl;

  // a hole still needs to be checked for 2D
  // if many edges have the same 'closest' edge, all of them can be checked for det() size, and the smallest picked.

  // first, pick a random hole, check for is2D, make it, make the polygon, update sph.
  std::cerr << "total possible candidates: " << count << std::endl;
  // these are candidates for a single inner polygon..

  unsigned int r_c=0, r_e=0;

  if (curves.size() > 0) {
    do {
      UniformRandomI(r_c, 0, curves.size()-1);
      if (curves[r_c].edges.size() == 0) {
        curves.erase(curves.begin()+r_c);
        continue;
      }
      else {
        std::cerr << "total edges: " << curves[r_c].edges.size() << std::endl;
        UniformRandomI(r_e, 0, (curves[r_c].edges).size()-1);
        break;
      }
    } while (curves.size() > 0);
  }
  std::cerr << "r_c: " << r_c << ", r_e: " << r_e << std::endl;
  E_Edge r_edge = curves[r_c].edges[r_e];
  std::cerr << "random edge: " << r_edge << std::endl;

  return r_edge;
}



// function that accepts a simple polygon that is assumed to be the inner polygonal chain defined by 2 incidental convex hull points,
// and returns a array of a polygon with its holes.
// Input:
//        'sph'         : A vector of vectors of indices.  sph[0] is the original polygon
//        'polygon'     : a vector with vertices of 'points' set that is a simple polygon, is an inner polygonal chain, but must be simple.
//        'points'      : a vector of <Point> objects
// Output: 'pol_array'  : an array of polygons, the first index is the outermost simple polygon, the rest are simple holes inside that polygon
enum error inner_holes_old(std::vector<std::vector<unsigned int>>& sph, std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int nr_holes, bool inner_bool) {
  std::vector<s_curve> sc;
  std::set<C_Edge> edgeS;
  std::pair<std::set<C_Edge>::iterator, bool> retval1, retval2; // return values for the 'edgeS' set.
  Point *m, *l, *r;
  bool isll, isrl;
  unsigned int count_open=0, count_cont=0, count_close=0;

//  pdisplay(polygon, points);
  //start with creating a vector for the lexicographically sorted indexes of 'points'
  std::vector<unsigned int> lex (polygon.size());
  fill_lex(lex, polygon, points); // fill 'lex' with the indexes
//  std::cerr << "lex: " << std::endl;
//  pdisplay(lex, points);

  std::cerr << "sph: " << sph.size() << ", holes: " << nr_holes << std::endl;
//  pdisplay(polygon, points);

  // go through all the points in lex. order, except the last.  We don't need to process the last vertex.
  for (unsigned int i = 0; i < lex.size()-1; ++i) {
    m = &points[lex[i]];
    // can't use sph, as the polygon is in 'polygon' and I have to traverse that..
//    std::cerr << "lex[i]: " << lex[i] << ", m.v: " << (*m).v << ", next: " << (polygon.size() + lex[i] + 1) % polygon.size() << std::endl;
    l = &points[polygon[(polygon.size() + (*m).v - 1) % polygon.size()]];
    r = &points[polygon[(polygon.size() + (*m).v + 1) % polygon.size()]];
//    std::cerr << "m: " << *m << ", l: " << *l << ", r: " << *r << std::endl;

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
//      std::cerr << "e1: " << e1 << ", retval: " << ((retval1.first == edgeS.end()) ? "not found" : "found") << std::endl;
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
      if (e1.sc < e2.sc) nu_sc = e1.sc; // new ends are designated as a new pair in the lower index curve, no need to add to both older curves
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
    else if (isll ^ isrl) {
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
        else nu_c.rin = !inner_bool; // UPDATE: inner polygons will have 'rin' as false as default, normal polygons have true.
        // check if e2 is highest edge in 'edgeS'
        if (bef && (retval2.first != --edgeS.end())) {
          after = *(std::next(retval2.first));
          // if curve has other curves on both sides,
          // check upper/lower orientation of adjacent s_curves
          if (before.lower == after.lower) { // 2 inner curves inside an outer curve
            if (before.lower == false) nu_c.rin = sc[before.sc].rin; // the inner upper curve
            else nu_c.rin = !sc[before.sc].rin; // the inner lower curve
          }
          else if (before.sc == after.sc) nu_c.rin = !sc[before.sc].rin; // one inner curve encapsulated by an outer curve
          else nu_c.rin = sc[before.sc].rin; // between 2 incidental inner curves
        }
        else nu_c.rin = !inner_bool; // UPDATE: same as above

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
        else nu_c.rin = !inner_bool; // UPDATE: same as above
        // check if e1 is highest edge in 'edgeS'
        if (bef && (retval1.first != --edgeS.end())) {
          after = *(std::next(retval1.first));
          // if curve has other curves on both sides,
          // check upper/lower orientation of adjacent s_curves
          if (before.lower == after.lower) {
            if (before.lower == false) nu_c.rin = sc[before.sc].rin;
            else nu_c.rin = !sc[before.sc].rin;
          }
          else if (before.sc == after.sc) nu_c.rin = !sc[before.sc].rin;
          else nu_c.rin = sc[before.sc].rin;
        }
        else assert(nu_c.rin == !inner_bool); // UPDATE: same as above

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

// function that accepts a simple polygon and returns a array of a polygon with its holes.
// Input:
//        'sph'         : A vector of vectors of indices.  the first sph is the original polygon
//        'polygon'     : a vector with vertices of 'points' set that is a simple polygon, can be an inner polygonal chain, but must be simple.
//        'points'      : a vector of <Point> objects
// Output: 'pol_array'  : an array of polygons, the first index is the outermost simple polygon, the rest are simple holes inside that polygon
enum error holes(std::vector<std::vector<unsigned int>>& sph, std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int nr_holes) {
  std::vector<s_curve> sc;
  std::set<C_Edge> edgeS;
  std::pair<std::set<C_Edge>::iterator, bool> retval1, retval2; // return values for the 'edgeS' set.
  Point *m, *l, *r;
  bool isll, isrl;
  unsigned int count_open=0, count_cont=0, count_close=0;

//  pdisplay(polygon, points);
  //start with creating a vector for the lexicographically sorted indexes of 'points'
  std::vector<unsigned int> lex (polygon.size());
  fill_lex(lex, polygon, points); // fill 'lex' with the indexes
//  std::cerr << "lex: " << std::endl;
//  pdisplay(lex, points);

  std::cerr << "sph: " << sph[0].size() << ", holes: " << nr_holes << std::endl;
//  pdisplay(polygon, points);

  // go through all the points in lex. order, except the last.  We don't need to process the last vertex.
  for (unsigned int i = 0; i < lex.size()-1; ++i) {
    m = &points[lex[i]];
    // can't use sph, as the polygon is in 'polygon' and I have to traverse that..
//    std::cerr << "lex[i]: " << lex[i] << ", m.v: " << (*m).v << ", next: " << (polygon.size() + lex[i] + 1) % polygon.size() << std::endl;
    l = &points[polygon[(polygon.size() + (*m).v - 1) % polygon.size()]];
    r = &points[polygon[(polygon.size() + (*m).v + 1) % polygon.size()]];
//    std::cerr << "m: " << *m << ", l: " << *l << ", r: " << *r << std::endl;

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
//      std::cerr << "e1: " << e1 << ", retval: " << ((retval1.first == edgeS.end()) ? "not found" : "found") << std::endl;
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

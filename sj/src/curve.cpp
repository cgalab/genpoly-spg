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
#include "opt2.h" //needed to create polygons.
#include "rand.h"
#include "simple_pol_check.h"

enum error curve(std::vector<unsigned int>& polygon, std::vector<Point>& points) {
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
enum error holes2(std::vector<std::vector<unsigned int>>& sph, std::vector<Point>& points, unsigned int nr_holes) {
  assert(sph.size() == 0);
  // variable for how many times you should generate a polygon to check for holes, skipped if a polygon is given.
  unsigned int max_iterations = 10;
  unsigned int count_iterations = 0;
  unsigned int count_holes;
  std::vector<E_Edge> backup_holes;
  bool end_erase_flag;

  // start with getting all c.h. points.
  std::vector<unsigned int> ch;
  get_convex_hull(ch, points);
//  std::cerr << "convex hull: " << std::endl;
//  pdisplay(ch, points);

//	std::cerr << "holes: " << nr_holes << ", pol.size: " << sph[0].size() << std::endl;
  unsigned int nr_inner = sph[0].size()-ch.size();
//  std::cerr << "c.h. points: " << ch.size() << ", inner points: " << nr_inner << ", sph: " << sph.size() << std::endl;

  //if there are less than 3 inner points
  if (nr_inner < 3) return TOO_FEW_INNER_POINTS_FOR_HOLE;
  if (nr_inner == 3) {
    // get inner points
    std::vector<unsigned int> ip;
    get_inner_points(ip, ch, points);
    // append ch as the first vector of indexes to sph
    sph.push_back(ch);
    // append the inner points vector of indexes to sph
    sph.push_back(ip);
    if (nr_holes > 1) return HOLES_INADEQUATE;
    return SUCCESS;
  }

  if (nr_inner > 3) {
    //std::vector<unsigned int> polygon;
    std::vector<Ends> ends;
    unsigned int total_holes = 0;

    if (sph[0].size() == 0) return UNEXPECTED_ERROR;
//    std::cerr << "generate polygons: " << ((generate_polygons) ? "true" : "false") << std::endl;

    // loop that would iterate over seeds to generate different simple polygons until nr_holes is reached,
    // is skipped if a polygon is given.
    do {
      //if (sph.size() > 0) sph.clear();
      // get a simple polygon to work with.

      // assign the polygon to sph[0] as sph[0] should always contain the simple polygon, and any/all holes are pushed below it.
      //sph.push_back(polygon);
//      std::cerr << "=== The points in the simple polygon permutation ===" << std::endl;
//      for (unsigned int i = 0; i < sph[0].size();++i) std::cerr << sph[0][i] << std::endl;

      count_holes = 0;
      ends.erase(ends.begin(), ends.end());
      // add the starting edges of all inner curves of the c.h. to 'ends' vector
      get_inner_chains_to_ch(ends, ch, sph[0], points);
      do {
        end_erase_flag = false;
//        std::cerr << "ends: " << ends.size() << std::endl;
        if (ends.size() == 0) break;

        // now I have to go through the ends and make sure that there are enough points in each inner polygonal chain to create desired # of holes
        unsigned int i = 0;
        total_holes = 0;
        do {
  //        std::cerr << "current end: "<< ends[i] << std::endl;
          // is_2D checks to see if the chain is somewhere non-collinear as well as check no. of points.
          if (is_2D(ends[i], sph[0], points)) {
  //          std::cerr << "is 2D" << std::endl;
            // get length of inner polygonal chain
            unsigned int diff = get_chain_length(ends[i], sph[0].size());
            ends[i].nr_holes = (int)(diff/3);

            total_holes = total_holes + ends[i].nr_holes;
            ++i;
          }
          else {
  //          std::cerr << "not 2D" << std::endl;
            //std::cerr << "end: " << *(std::next(ends.begin(),i)) << std::endl;
            ends.erase(std::next(ends.begin(),i));
            //std::cerr << "after erase: " << *(std::next(ends.begin(),i)) << std::endl;
          }
        } while (i < ends.size());
//        std::cerr << "total (theoretically possible) holes " << total_holes << std::endl;

//        std::cerr << std::endl << "ends: " << ends.size() << std::endl;
//        if (ends.size() == 0 ) break;
//        for (unsigned int i=0; i < ends.size(); ++i) std::cerr << ends[i] << std::endl;

        // select a random end
        unsigned int r_end = 0;
        UniformRandomI(r_end, 0, ends.size()-1);
//        std::cerr << "random index: " << r_end << std::endl;
//        std::cerr << "Random end: " << ends[r_end] << std::endl;

        //create a vector of indexes that form the points of the inner pol. chain.
        std::vector<unsigned int> inner_polygon;
        get_inner_chain_polygon(inner_polygon, ends[r_end], sph[0]);
//        std::cerr << "== inner polygon: " << "==" << std::endl;
//        pdisplay(inner_polygon, points);

        std::vector<Point> inner_points;
        get_inner_chain_points(inner_points, inner_polygon, points);
//        std::cerr << "== inner points: " << "==" << std::endl;
//        pdisplay(inner_points);
        // now we have the points of the chain in 'inner_points' and the indices into 'points' in 'inner_polygon'

        // an E_Edge has a class property 'vector of 'E_Edge's': 'closest' as its closest edge, these 2 edges define 4 points we can use to make a hole.
        E_Edge inner_hole = inner_holes2(inner_points, false);
        if (inner_hole != inner_hole.closest[0]) {
          if (((*inner_hole.p1).i == 0 && (*inner_hole.p2).i == inner_points.size()-1) || ((*inner_hole.p2).i == 0 && (*inner_hole.p1).i == inner_points.size()-1)) {
//            std::cerr << "edge on c.h., use ends" << std::endl;
            inner_hole = E_Edge(&inner_points[0], &inner_points[1]);
            inner_hole.closest.push_back(D_Edge(&inner_points[inner_points.size()-2], &inner_points[inner_points.size()-1]));
            // used the end to make a hole, must erase it
            end_erase_flag = true;
          }
//          std::cerr << "inner_hole: " << inner_hole << std::endl;
//          std::cerr << "inner_hole.closest: " << inner_hole.closest[0] << std::endl;
//          std::cerr << "original index edge #1.p1: " << points[inner_polygon[(*inner_hole.p1).i]] << std::endl;
//          std::cerr << "original index edge #1.p2: " << points[inner_polygon[(*inner_hole.p2).i]] << std::endl;
//          std::cerr << "original index edge #2.p1: " << points[inner_polygon[(*inner_hole.closest[0].p1).i]] << std::endl;
//          std::cerr << "original index edge #2.p2: " << points[inner_polygon[(*inner_hole.closest[0].p2).i]] << std::endl;
//          std::cerr << "found a hole candidate!" << std::endl;
          E_Edge new_hole = E_Edge(&points[inner_polygon[(*inner_hole.p1).i]], &points[inner_polygon[(*inner_hole.p2).i]]);
          new_hole.closest.push_back(D_Edge(&points[inner_polygon[(*inner_hole.closest[0].p1).i]], &points[inner_polygon[(*inner_hole.closest[0].p2).i]]));
          // the hole can be represented as a <unsigned int> vector of indices into points,
          // as I don't do anything with the hole after making it,
          // this could then be pushed into 'sph'.
          // the new polygon can be a <Point> vector which overwrites 'polygon'
          std::vector<unsigned int> hole;
          std::vector<Point> p_hole;
          std::vector<unsigned int> new_polygon;
          get_hole_and_new_pol(hole, new_polygon, new_hole, sph[0], points);
//          std::cerr << "new hole:" << std::endl;
//          pdisplay(hole, points);
          for (unsigned int i = 0; i < hole.size(); ++i) {
            p_hole.push_back(points[hole[i]]);
            p_hole[i].v = i;
          }
//          std::cerr << "new p_hole:" << std::endl;
//          pdisplay(p_hole);

          // need to make a function that can take the end and the new polygon and return the new chain
          // then verify that the hole and the new inner chain are both still simple.
          //std::vector<unsigned int> new_inner_polygon;
          //get_new_inner_polygon(ends[r_end], new_inner_polygon, new_polygon, points);
          enum error simple = SUCCESS;
          simple = simple_pol_check(p_hole);
          if (simple == SUCCESS) {
//            std::cerr << "p_hole is simple" << std::endl;
            std::vector<Point> p_pol;
            for (unsigned int i = 0; i < new_polygon.size(); ++i) {
              p_pol.push_back(points[new_polygon[i]]);
              p_pol[i].v = i;
            }
            simple = simple_pol_check(p_pol);
            if (simple == SUCCESS) {
//              std::cerr << "p_pol is simple" << std::endl;
              // need to reindex the points in new polygon.
              for (unsigned int i = 0; i < new_polygon.size(); ++i) points[new_polygon[i]].v = i;
              sph[0] = new_polygon;
              sph.push_back(hole);
              ++count_holes;
//              std::cerr << "holes so far: " << count_holes << std::endl;

//              std::cerr << "new polygon" << std:: endl;
//              pdisplay (sph[0], points);
            }
            else {
//              std::cerr << "no hole found, removing: " << ends[r_end] << std::endl;
              // no candidates were found, as the inner chain was 2D though,
              // assign it as a backup hole
//              std::cerr << "original index edge #1.p1: " << points[inner_polygon[inner_points[0].i]] << std::endl;
//              std::cerr << "original index edge #1.p2: " << points[inner_polygon[inner_points[1].i]] << std::endl;
//              std::cerr << "original index edge #2.p1: " << points[inner_polygon[inner_points[inner_points.size()-1].i]] << std::endl;
//              std::cerr << "original index edge #2.p2: " << points[inner_polygon[inner_points[inner_points.size()-2].i]] << std::endl;
              E_Edge backup = E_Edge(&points[inner_polygon[inner_points[0].i]], &points[inner_polygon[inner_points[1].i]]);
              backup.closest.push_back(D_Edge(&points[inner_polygon[inner_points[inner_points.size()-1].i]], &points[inner_polygon[inner_points[inner_points.size()-2].i]]));
              backup_holes.push_back(backup);
              end_erase_flag = true;
            }
          }
          else {
//            std::cerr << "no hole found, removing: " << ends[r_end] << std::endl;
            // no candidates were found, as the inner chain was 2D though,
            // assign it as a backup hole
//            std::cerr << "original index edge #1.p1: " << points[inner_polygon[inner_points[0].i]] << std::endl;
//            std::cerr << "original index edge #1.p2: " << points[inner_polygon[inner_points[1].i]] << std::endl;
//            std::cerr << "original index edge #2.p1: " << points[inner_polygon[inner_points[inner_points.size()-1].i]] << std::endl;
//            std::cerr << "original index edge #2.p2: " << points[inner_polygon[inner_points[inner_points.size()-2].i]] << std::endl;
            E_Edge backup = E_Edge(&points[inner_polygon[inner_points[0].i]], &points[inner_polygon[inner_points[1].i]]);
            backup.closest.push_back(D_Edge(&points[inner_polygon[inner_points[inner_points.size()-1].i]], &points[inner_polygon[inner_points[inner_points.size()-2].i]]));
            backup_holes.push_back(backup);
            end_erase_flag = true;
          }
        }
        else {
//          std::cerr << "no hole found, removing: " << ends[r_end] << std::endl;
          // no candidates were found
          end_erase_flag = true;
        }
//        std::cerr << "gen_pol: " << ((generate_polygons) ? "true" : "false") << ", iter: " << count_iterations << std::endl;
        if (end_erase_flag) {
          ends.erase(ends.begin()+r_end);
        }
      } while (count_holes < nr_holes && ends.size() > 0);



      // future thoughts:
      // * if I always start with creating holes from the ends, what about creating
      //   more than one hole from an inner pol. chain?
      // * if I know how many holes I can theoretically pick from each unique primal edge pair,
      //   I can randomly pick between all holes the primal pairs can generate
      // * Let's say a primal pair has enough points to theoretically generate 5 holes
      //



      ++count_iterations;
//      std::cerr << "generate polygons?: " << ((generate_polygons) ? "true" : "false") << std::endl;
//      std::cerr << "iteration: " << count_iterations << std::endl;
//      std::cerr << "found holes: " << count_holes << std::endl;
    } while ((count_iterations < max_iterations) && (count_holes < nr_holes));

//    std::cerr << "wanted holes:" << nr_holes << ", found holes: " << count_holes << std::endl;
//    std::cerr << "backup holes: " << backup_holes.size() << std::endl;

    // in case we are missing some holes, let's use inner chains as holes.
    ends.erase(ends.begin(), ends.end());
    get_inner_chains_to_ch(ends, ch, sph[0], points);
    unsigned int i = 0;
    do {
      if (is_2D(ends[i], sph[0], points)) ++i;
      else ends.erase(std::next(ends.begin(),i));
    } while (i < ends.size());
//    std::cerr << "ends:" << std::endl;
//    for (unsigned int i=0; i < ends.size(); ++i) std::cerr << ends[i] << std::endl;

    i = 0;
    while ((count_holes < nr_holes) && (ends.size() > 0)) {
      // backup hole edges might have changed since they were added.
      std::vector<unsigned int> hole;
      std::vector<Point> p_hol;
      std::vector<unsigned int> new_polygon;
      // select a random end
      unsigned int r_end = 0;
      UniformRandomI(r_end, 0, ends.size()-1);
//      std::cerr << "random index: " << r_end << std::endl;
//      std::cerr << "Random end: " << ends[r_end] << std::endl;
//      std::cerr << "re1.p1: " <<  *ends[r_end].par.first.p1 << std::endl;
//      std::cerr << "re1.p2: " <<  *ends[r_end].par.first.p2 << std::endl;
//      std::cerr << "re2.p1: " <<  *ends[r_end].par.second.p1 << std::endl;
//      std::cerr << "re2.p2: " <<  *ends[r_end].par.second.p2 << std::endl;

      E_Edge backup = E_Edge(ends[r_end].par.first.p1, ends[r_end].par.first.p2);
      backup.closest.push_back(D_Edge(ends[r_end].par.second.p1, ends[r_end].par.second.p2));
      ends.erase(ends.begin()+r_end);

      get_hole_and_new_pol(hole, new_polygon, backup, sph[0], points);

      // populate 'p_hol' with points from 'points' defined by polygon 'hole'
      for (unsigned int i = 0; i < hole.size(); ++i) {
        p_hol.push_back(points[hole[i]]);
        p_hol[i].v = i;
      }
      enum error simple = SUCCESS;
      simple = simple_pol_check(p_hol);
      if (simple == SUCCESS) {
        std::vector<Point> p_pol;
        for (unsigned int i = 0; i < new_polygon.size(); ++i) {
          p_pol.push_back(points[new_polygon[i]]);
          p_pol[i].v = i;
        }
        simple = simple_pol_check(p_pol);
        if (simple == SUCCESS) {
          // need to reindex the points in new polygon.
          for (unsigned int i = 0; i < new_polygon.size(); ++i) points[new_polygon[i]].v = i;


//      std::cerr << "new polygon" << std:: endl;
//      pdisplay (sph[0], points);
//          std::cerr << "new hole:" << std::endl;
//          pdisplay(hole, points);

          sph[0] = new_polygon;
          sph.push_back(hole);
          ++count_holes;
        }
      }
    }
    std::cout << "total holes: " << count_holes << std::endl;
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
bool get_bin(E_Edge& e1, E_Edge& e2, std::set<E_Edge>::iterator& retval1, std::set<E_Edge>::iterator& retval2, std::set<E_Edge>& y_set) {
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
    return !before.bin;
  }
  else {
    return !after.bin;
  }
}

// function to check the edge between p1 points of 'e' and 'e.closest'
// (as well as p2 points) for intersection with 'e2'.
bool check_ix_edges(E_Edge e1, E_Edge e2) {
  E_Edge ix;
  enum intersect_t isval; // return value for intersect check.

  ix = E_Edge(e1.p1, e1.closest[0].p1);
  isval = checkIntersection(ix, e2);
  if (isval >= IS_TRUE) return true;

  ix = E_Edge(e1.p2, e1.closest[0].p2);
  isval = checkIntersection(ix, e2);
  if (isval >= IS_TRUE) return true;
  return false;
}

//function to return the incidental edge of 'e' from the 'y_set'
// There are of course 2 incidental edges to most edges in a set, but as one side of an edge is always "inside"
// the polygon and the other is then always "outside", only one edge is an incidental inside edge and thus usable to make a hole.
E_Edge get_inc_edge_from_set(E_Edge& e, std::vector<Curve>& curves, std::set<E_Edge>::iterator it, std::set<E_Edge>& edges) {
  E_Edge inc_e;
  if (curves[e.curve_id].bin) {
    if (it != edges.begin()) inc_e = *(std::prev(it));
    else inc_e = e;
  }
  else {
    if (it != --edges.end()) inc_e = *(std::next(it));
    else inc_e = e;
  }
  return inc_e;
}

// function to update edge in set, i.e. find it, remove it and insert it again.
void update_edge_in_set(E_Edge& e, std::pair<std::set<E_Edge>::iterator, bool>& retval, std::set<E_Edge>& y_set) {
//  std::cerr << "=== update_edge_in_set ===" << std::endl;
  if (e == *retval.first) {
    y_set.erase(retval.first);
    retval = y_set.insert(e);
    if ((*retval.first != e) || (retval.second != true)) {
      std::cerr << "Error!  update_edge_in_set failed to insert edge!" << std::endl;
      std::cerr << "e: " << e << std::endl;
      std::cerr << "iterator: " << *retval.first << std::endl;
    }
  }
  else {
    std::cerr << "Error.  update_edge_in_set got an edge and iterator that did not match" << std::endl;
    std::cerr << "e: " << e << std::endl;
    std::cerr << "iterator: " << *retval.first << std::endl;
  }
//  std::cerr << "r: e1: " << (*retval1.first) << ", e1.closest: " << (*retval1.first).closest << std::endl;
}

// A function to find and return an E edge which (along with it's 'closest' property) define a pair of edges that can be used to make a hole.
// input:
//        'points': assumed to be in the arrangement of the polygonal chain between 2 convex hull points.
// return:
//         a 'Ends' variable which defines 4 points in the polygon that can be split to make a hole.
E_Edge inner_holes(std::vector<Point>& points) {

  //start with creating a vector for the lexicographically sorted indexes of 'points'
  std::vector<unsigned int> lex (points.size());
  fill_lex(lex, points);
//  std::cerr << "lex: " << std::endl;
//  pdisplay(lex, points);

  // I need to do a linesweep over the lex points
  // When an edge 'e' is freshly inserted, check which edge is 'before' and 'after'
  // for the 'before' and 'after' edges, add 'e' as the 'closest_before' or 'closest_after'
  // I want to skip the edge between [0] and [n-1] as that's the edge between the convex hull points.

  std::vector<Curve> curves; // container for curves and their valid edges to create holes from.
  std::vector<E_Edge> valid_edges;  // all edges with a valid 'closest' edge, edges are added when they are removed from y_set'
  std::set<E_Edge> y_set; // container for the planesweep structure.
  std::pair<std::set<E_Edge>::iterator, bool> retval1, retval2; // return values from the y_set 'insert' or 'erase' functions.
  Point *m, *l, *r; // m is the current point at lex. position 'i' and 'l' and 'r' are the 2 points it connects to by edges in the polygon.
  bool isll, isrl; // are 'l' and 'r' points left of 'm' in the euclidian planar sense.
  unsigned int count_open=0, count_cont=0, count_close=0; // just for development verification.
  D_Edge temp = D_Edge(&points[0], &points[points.size()-1]);

  // a sweep through the lex indices to check all edges for its 'closest' edge.
  for (unsigned int i = 0; i < lex.size()-1; ++i) {
    std::cerr << std::endl << "i: " << i << std::endl;
    m = &points[lex[i]];
    l = &points[(points.size() + (*m).v - 1) % points.size()];
    r = &points[(points.size() + (*m).v + 1) % points.size()];
    std::cerr << "m: " << *m << ", l: " << *l << ", r: " << *r << std::endl;

    // create 2 new 'E_Edge's
    E_Edge e1 = E_Edge (m, l);
    E_Edge e2 = E_Edge (m, r);
    e1.closest[0] = temp;
    e2.closest[0] = temp;
    std::cerr << "e1: " << e1 << std::endl;
    std::cerr << "e2: " << e2 << std::endl;

    // check for 'o<', '-o-', '>o' condition
    (*m < *l) ? isll = false : isll = true;
    (*m < *r) ? isrl = false : isrl = true;

    if (isll && isrl) {
      std::cerr << "=== >o ===" << std::endl;
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
      e1 = (*retval1.first); // there are properties in the edge in the set that are needed.

      // find 'e2' in 'y_set'
      retval2.first = y_set.find(e2);
      assert(*(retval2.first) == e2);
      e2 = (*retval2.first);

      // need to check which is higher so I can assign them as each others incidental edges,
      // but only if the "below edge is inside" boolean: 'bin' is true.  If it is false, they aren't each others closest edges.
      if (e1 < e2) {
        if (curves[e2.curve_id].bin) {
          e1.closest[0] = e2;
          e2.closest[0] = e1;
        }
      } else {
        if (curves[e1.curve_id].bin) {
          e1.closest[0] = e2;
          e2.closest[0] = e1;
        }
      }
      std::cerr << "e1 in set: " << e1 << std::endl;
      std::cerr << "e2 in set: " << e2 << std::endl;

      // if the 2 edges have each other as closest edges, no need for processing
      if (e1.closest[0] != e2 || e2.closest[0] != e1) {
//        std::cerr << " lower cyclic diff. of e1.p1 and e1.closest.p1: " << get_lower_cyclic_difference((*e1.p1).v, (*e1.closest.p1).v, points.size()) << std::endl;
//        std::cerr << " lower cyclic diff. of e1.p2 and e1.closest.p2: " << get_lower_cyclic_difference((*e1.p2).v, (*e1.closest.p2).v, points.size()) << std::endl;
        if ((get_lower_cyclic_difference((*e1.p1).v, (*e1.closest[0].p1).v, points.size()) > 2) && (get_lower_cyclic_difference((*e1.p2).v, (*e1.closest[0].p2).v, points.size()) > 2)) {
//          std::cerr << "true" << std::endl;
          if (e1 != temp && e1.closest[0] != temp) curves[e1.curve_id].edges.push_back(e1);
        }
//        std::cerr << " lower cyclic diff. of e2.p1 and e2.closest.p1: " << get_lower_cyclic_difference((*e2.p1).v, (*e2.closest.p1).v, points.size()) << std::endl;
//        std::cerr << " lower cyclic diff. of e2.p2 and e2.closest.p2: " << get_lower_cyclic_difference((*e2.p2).v, (*e2.closest.p2).v, points.size()) << std::endl;
        if ((get_lower_cyclic_difference((*e2.p1).v, (*e2.closest[0].p1).v, points.size()) > 2) && (get_lower_cyclic_difference((*e2.p2).v, (*e2.closest[0].p2).v, points.size()) > 2)) {
//          std::cerr << "true" << std::endl;
          if (e2 != temp && e2.closest[0] != temp) curves[e2.curve_id].edges.push_back(e2);
        }
      }
      y_set.erase(e1);
      y_set.erase(e2);
    }
    else if (isll ^ isrl) {
      std::cerr << "=== -o- ===" << std::endl;
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

      // Here I need to find the 'closest' edge for the 'new' edge, candidates are
      // the 'closest' edge of 'old' and the incidental edges in 'y_set'
      // I also need to check if 'new' is a new 'closest' for the incidental edges.

      // find 'old_e' in 'y_set'
      retval1.first = y_set.find(old_e);
      assert(*(retval1.first) == old_e);

      // copy values from iterator to 'new_e'
      old_e = (*retval1.first);
      new_e.curve_id = old_e.curve_id;
      new_e.bin = old_e.bin;
      std::cerr << "old: " << old_e << std::endl;
      std::cerr << "new: " << new_e << std::endl;
      //std::cerr << "begin(): " << *(y_set.begin()) << ", end()-1: " << *(std::prev(y_set.end())) << std::endl;

      // old_e needs to be removed, i.e. first find the incidental edges and
      // check conditions for 'last' and whether it's still the same curve.

      // I need to find which side (bef or aft) is the "inside"
      E_Edge inc_e; // incidental edge of the old_e.
      inc_e = get_inc_edge_from_set(old_e, curves, retval1.first, y_set);
      if (old_e == inc_e) {inc_e = temp;inc_e.closest[0] = temp;std::cerr<<"old_e == inc_e" << std::endl;}
      std::cerr << "inc_e: " << inc_e << std::endl;

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
      // 3) might guarantee we do not need to check any intersections with the rest of the curve,
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
      if ((get_lower_cyclic_difference((*old_e.p1).v, (*old_e.closest[0].p1).v, points.size()) > 2) && (get_lower_cyclic_difference((*old_e.p2).v, (*old_e.closest[0].p2).v, points.size()) > 2)) {
        //std::cerr << "old_e: " << old_e << ", closest: " << old_e.closest << std::endl;
//        std::cerr << "pushing old edge to curve, lcd is: " << get_lower_cyclic_difference((*old_e.p1).v, (*inc_e.p1).v, polygon.size()) << std::endl;
        // we also have to check if the closest edge is the convex hull edge, and if so, not push it into the vector.
        if (old_e != temp && old_e.closest[0] != temp) curves[old_e.curve_id].edges.push_back(old_e);
      }
      y_set.erase(retval1.first);

      // process 'new_e'
      // This is the first candidate for a 'closest edge', it's fine to check and set it as closest.
      //new_e.update_closest(inc_e, true); // true for lex-low-to-high
      //std::cerr << "new_e: " << new_e << std::endl;

      // process 'inc_e' - reuse retval1 to find inc_e and update it if necessary.
      // need to add an intersection check to make sure that 'inc_e.closest' is
      // valid and doesn't have intersections with 'new_e'. if not then new_e is closest.

      retval1.first = y_set.find(inc_e);
      assert(*retval1.first == inc_e);
      std::cerr << "found inc in set: " << *retval1.first << std::endl;
      if (check_ix_edges(inc_e, new_e)) {
        std::cerr << "new_e is closer to inc_e than inc_e.closest" << std::endl;
        inc_e.closest[0] = new_e;
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
      std::cerr << "=== o< ===" << std::endl;
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
      new_curve1.bin = get_bin(e1, e2, retval1.first, retval2.first, y_set);
      new_curve2.bin = get_bin(e2, e1, retval2.first, retval1.first, y_set);
//      std::cerr << "curve1: " << new_curve1 << std::endl;
//      std::cerr << "curve2: " << new_curve2 << std::endl;
      curves.push_back(new_curve1);
      curves.push_back(new_curve2);

      // dependant on "inside" not being inside the 2 joined edges, we need to find the right 'closest' edges.
      e1.closest[0] = get_inc_edge_from_set(e1, curves, retval1.first, y_set);
      if (e1 == e1.closest[0]) e1.closest[0] = temp;
      update_edge_in_set(e1, retval1, y_set);
      e2.closest[0] = get_inc_edge_from_set(e2, curves, retval2.first, y_set);
      if (e1 == e1.closest[0]) e1.closest[0] = temp;
      update_edge_in_set(e2, retval2, y_set);
      std::cerr << "e1: " << e1 << std::endl;
      std::cerr << "e2: " << e2 << std::endl;

      // if closest edge isn't one of the 2 new edges, for 'closest' edge:
      // do a check to see whether e1/e2 is closer than its 'closest' edge.
      // possibilities are determinant check, intersect check, or just euclidean metric.
      // - Technically I don't care what is closer, I care if the new edges intersect with the old closest.
      if ((*retval1.first).closest[0] != e2) {
        E_Edge inc_e = get_inc_edge_from_set(e1, curves, retval1.first, y_set);
        if (e1 == inc_e) {inc_e = temp;inc_e.closest[0] = temp;}
        std::cerr << "inc_e: " << inc_e << ", inc_e.closest: " << inc_e.closest[0] << std::endl;
        if (check_ix_edges(inc_e, e1)) {
          // e1 should be assigned closest as it intersects.
          inc_e.closest[0] = e1;
          // need to update the inc_e edge in y_set:
          retval1.first = y_set.find(inc_e);
          update_edge_in_set(inc_e, retval1, y_set);
        }
        // check the closest edge of e2:
        inc_e = get_inc_edge_from_set(e2, curves, retval2.first, y_set);
        if (e1 == inc_e) {inc_e = temp;inc_e.closest[0] = temp;}
        std::cerr << "inc_e: " << inc_e << ", inc_e.closest: " << inc_e.closest[0] << std::endl;
        std::cerr << "in here" << std::endl;
        if (check_ix_edges(inc_e, e2)) {
          // e2 should be assigned closest as it intersects
          inc_e.closest[0] = e2;
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

  std::cerr << "curves: " << std::endl;
  unsigned int count = 0;
  for (unsigned int i = 0; i < curves.size();++i) {
    count = count + curves[i].edges.size();
    std::cerr << curves[i] << std::endl;
  }

  // a hole still needs to be checked for 2D
  // if many edges have the same 'closest' edge, all of them can be checked for det() size, and the smallest picked.

  // first, pick a random hole, check for is2D, make it, make the polygon, update sph.
  std::cerr << "total possible candidates: " << count << std::endl;
  // these are candidates for a single inner polygon..

  if (count > 0) {
    // r_c: a random curve selection
    // r_e: a random edge selection
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
  else {
    E_Edge r_edge = E_Edge(temp);
    r_edge.closest[0] = temp;
    return r_edge;
  }
}

// function that takes a vector of Points and boolean is_hole which defines whether points is a hole or an inner polygonal chain
// and returns an edge with a single 'closest' edge which can be used to split the hole/chain.
E_Edge inner_holes2(std::vector<Point>& points, bool is_hole) {
//  std::cerr << "=== inner holes function ===" << std::endl;
//  std::cerr << "is_hole: " << is_hole << std::endl;
  E_Edge return_edge;

  std::vector<E_Edge> valid_edges;  // all edges with a valid 'closest' edge, edges are added when they are removed from y_set'
  std::set<E_Edge> y_set; // container for the planesweep structure.
  std::pair<std::set<E_Edge>::iterator, bool> retval1, retval2; // return values from the y_set 'insert' or 'erase' functions.
  Point *m, *l, *r; // m is the current point at lex. position 'i' and 'l' and 'r' are the 2 points it connects to by edges in the polygon.
  bool isll, isrl; // are 'l' and 'r' points left of 'm' in the euclidian planar sense.
  unsigned int count_open=0, count_cont=0, count_close=0; // just for development verification.

  // start with creating a vector for the lexicographically sorted indexes of 'points'
  std::vector<unsigned int> lex (points.size());
  fill_lex(lex, points);
//  std::cerr << "lex: " << std::endl;
//  pdisplay(lex, points);

  // a sweep through the lex indices to check all edges for its 'closest' edge.
  for (unsigned int i = 0; i < lex.size(); ++i) {
//    std::cerr << std::endl << "i: " << i << std::endl;
    m = &points[lex[i]];
    l = &points[(points.size() + (*m).v - 1) % points.size()];
    r = &points[(points.size() + (*m).v + 1) % points.size()];
//    std::cerr << "m: " << *m << ", l: " << *l << ", r: " << *r << std::endl;

    // create 2 new 'E_Edge's
    E_Edge e1 = E_Edge (m, l);
    E_Edge e2 = E_Edge (m, r);
//    std::cerr << "e1: " << e1 << std::endl;
//    std::cerr << "e2: " << e2 << std::endl;

    // check for 'o<', '-o-', '>o' condition
    (*m < *l) ? isll = false : isll = true;
    (*m < *r) ? isrl = false : isrl = true;

    if (isll && isrl) {
//      std::cerr << "=== >o ===" << std::endl;
      ++count_close;

      // removing both edges from the set, and going over their 'closest' from the other direction.
      // find 'e1' in 'y_set' and update with values of the edge in 'y_set'
      retval1.first = y_set.find(e1);
      assert(*(retval1.first) == e1);
//      std::cerr << "e1: " << e1 << ", found: " << *(retval1.first) << std::endl;
      e1 = (*retval1.first); // there are properties in the edge in the set that are needed.
      find_update_closest(e1, retval1.first, y_set);

      // find 'e2' in 'y_set'
      retval2.first = y_set.find(e2);
      assert(*(retval2.first) == e2);
//      std::cerr << "e2: " << e2 << ", found: " << *(retval2.first) << std::endl;
      e2 = (*retval2.first);
      find_update_closest(e2, retval2.first, y_set);
      y_set.erase(e1);
      y_set.erase(e2);

//      std::cerr << "e1: " << e1 << std::endl;
//      std::cerr << "e2: " << e2 << std::endl;

      if (e1.closest.size() > 0) validate_closest(e1);
      if (e2.closest.size() > 0) validate_closest(e2);

      if (e1.closest.size() > 0) valid_edges.push_back(e1);
      if (e2.closest.size() > 0) valid_edges.push_back(e2);

    }
    else if (isll ^ isrl) {
//      std::cerr << "=== -o- ===" << std::endl;
      ++count_cont;

      //one edge is 'old' and needs to be removed from 'y_set' and the other is 'new' and needs to be added to 'y_set'.
      E_Edge old_e, new_e;
      if (*e1.p1 < *e2.p2) {
        old_e = e1;
        new_e = e2;
      }
      else {
        old_e = e2;
        new_e = e1;
      }

      // find 'old' in 'y_set'
      retval1.first = y_set.find(old_e);
      assert(*(retval1.first) == old_e);

      // copy values from iterator to 'new_e'
      old_e = *retval1.first;
      new_e.bin = old_e.bin;
//      std::cerr << "old: " << old_e << std::endl;
//      std::cerr << "new: " << new_e << std::endl;
      //std::cerr << "begin(): " << *(y_set.begin()) << ", end()-1: " << *(std::prev(y_set.end())) << std::endl;

      // erase 'old'
      y_set.erase(retval1.first);
      // check if 'old' is a "valid edge"
      if (old_e.closest.size() > 0) validate_closest(old_e);
      if (old_e.closest.size() > 0) valid_edges.push_back(old_e);

      //insert 'new'
      retval2 = y_set.insert(new_e);
      assert(*retval2.first == new_e);
      assert(retval2.second == true);

      find_update_closest(new_e, retval2.first, y_set);
    }
    else {
//      std::cerr << "=== o< ===" << std::endl;
      ++count_open;

      // insert both 'E_Edge's into y_set
      retval1 = y_set.insert(e1);
      assert(*retval1.first == e1);
      assert(retval1.second == true);
      retval2 = y_set.insert(e2);
      assert(*retval2.first == e2);
      assert(retval2.second == true);

      //after inserting, get the 'bin' value from incidental edges in 'y_set'
      e1.bin = get_bin(e1, e2, retval1.first, retval2.first, y_set);
      update_edge_in_set(e1, retval1, y_set);
      e2.bin = get_bin(e2, e1, retval2.first, retval1.first, y_set);
      update_edge_in_set(e2, retval2, y_set);
//      std::cerr << "e1       : " << e1 << std::endl;
//      std::cerr << "e1 in set: " << *(y_set.find(e1)) << std::endl;
//      std::cerr << "e2       : " << e2 << std::endl;
//      std::cerr << "e2 in set: " << *(y_set.find(e2)) << std::endl;

      find_update_closest(e1, retval1.first, y_set);
      // in case e2 was updated
      retval2.first = y_set.find(e2);
      e2 = *retval2.first;
      find_update_closest(e2, retval2.first, y_set);
    }

//    std::cerr << "Edges in y_set:" << std::endl;
//    for (std::set<E_Edge>::iterator it=y_set.begin(); it!=y_set.end(); ++it) std::cout << *it << std::endl;
  }

  // all valid edges should now be in 'valid_edges', pick one at random and one random closest edge.
//  std::cerr << "valid edges:" << std::endl;
//  for (unsigned int i = 0; i < valid_edges.size(); ++i) std::cerr << valid_edges[i] << std::endl;

  // clear all non-2D edges and non-simple holes
  // TODO: make it pick a random edges to check, as the code below technically
  // just removes the non-valid edges and then picks a random edge.
  unsigned int r_e, r_ce;
  E_Edge ends_e = E_Edge(&points[0], &points[points.size()-1]);
  while (valid_edges.size() > 0) {
    // get a random edge from valid_edges
    UniformRandomI(r_e, 0, valid_edges.size()-1);
    return_edge = valid_edges[r_e];
    // for inner polygonal chains, the (0,last) edge is an invalid option.
    if (return_edge != ends_e) {
      while (valid_edges[r_e].closest.size() > 0) {
        // pick a random closest edge
        UniformRandomI(r_ce, 0, valid_edges[r_e].closest.size()-1);
        // for inner polygonal chains, the (0,last) edge is an invalid option.
        if (valid_edges[r_e].closest[r_ce] != ends_e) {
          return_edge.closest.resize(0);
          return_edge.closest.push_back(valid_edges[r_e].closest[r_ce]);

          std::vector<unsigned int> polygon;
          for (unsigned int i = 0; i < points.size(); ++i) polygon.push_back(i);
          if (is_2D(return_edge, polygon, points, is_hole)) {
//            std::cerr << "hole is 2D" << std::endl;

            std::vector<unsigned int> hol, pol;
            get_hole_and_new_pol(hol, pol, return_edge, polygon, points, is_hole);
            std::vector<Point> p_hol;
            for (unsigned int i = 0; i < hol.size(); ++i) {
              p_hol.push_back(points[hol[i]]);
              p_hol[i].v = i;
            }
//            std::cerr << "p_hol: " << std::endl;
//            pdisplay(p_hol);
            enum error simple = simple_pol_check(p_hol);
            if (simple == SUCCESS) {
//              std::cerr << "hole is simple" << std::endl;
              // with the 'bin_edge' check that 'p_hol' is oriented with correct "in" inside.
              if (check_in_orientation(return_edge, p_hol)) {
//                std::cerr << "hole has correct orientation of 'in'" << std::endl;
                std::vector<Point> p_pol;
                for (unsigned int i = 0; i < pol.size(); ++i) {
                  p_pol.push_back(points[pol[i]]);
                  p_pol[i].v = i;
                }
//                std::cerr << "p_pol: " << std::endl;
//                pdisplay(p_pol);
                enum error simple = simple_pol_check(p_pol);
                if (simple == SUCCESS) {
//                  std::cerr << "pol is simple" << std::endl;
                  return return_edge;
                }
                else {
                  valid_edges[r_e].closest.erase(valid_edges[r_e].closest.begin()+r_ce);
                }
              }
              else {
                valid_edges[r_e].closest.erase(valid_edges[r_e].closest.begin()+r_ce);
              }
            }
            else {
              valid_edges[r_e].closest.erase(valid_edges[r_e].closest.begin()+r_ce);
            }
          }
          else {
            valid_edges[r_e].closest.erase(valid_edges[r_e].closest.begin()+r_ce);
          }
        }
        else {
          valid_edges[r_e].closest.erase(valid_edges[r_e].closest.begin()+r_ce);
        }
      }
    }
    else {
      valid_edges.erase(valid_edges.begin()+r_e);
    }
    if (valid_edges[r_e].closest.size() == 0) {
      valid_edges.erase(valid_edges.begin()+r_e);
    }
  }
  // the loop above should have returned an edge if a valid hole was found.
//  std::cerr << "No valid holes were found!" << std::endl;
  return_edge = E_Edge(&points[0], &points[1]);
  return_edge.closest.push_back(D_Edge(&points[0], &points[1]));

  return return_edge;
}

// function to go over the edges in 'closest' to make sure that the "smallest angle
// theorem" isn't violated, where e.p2 is the origin, and looking the opposite direction of the planesweep.
void validate_closest(E_Edge& e) {
//  std::cerr << "=== validate_closest function ===" << std::endl;
  double angle_left, angle_right = 8;
  // go from the next-last element to the starting element, possibly removing elements from vector.

  int i = e.closest.size()-1;
  while(i > -1) {
//    std::cerr << "val i: " << i << std::endl;
//    std::cerr << "e: " << e << std::endl;

    E_Edge temp = e.closest[i];
//    std::cerr << "closest: " << temp << std::endl;
    angle_left = get_smaller_angle(e, temp, false);
//    std::cerr << "angle_left (p1): " << angle_left << ", angle_right: " << angle_right << std::endl;

    if (angle_right < fabs(angle_left)) {
//      std::cerr << "erasing edge: " << e.closest[i] << std::endl;
      e.closest.erase(e.closest.begin()+i);
    }
    else {
      angle_left = fabs(angle_left);
      if (angle_left < angle_right) angle_right = angle_left;
    }
    --i;
  }


  // remove any edges from closest that do not fall within the plane of the edge itself.
  i = e.closest.size()-1;
  while (i > -1) {
//    std::cerr << "checking: " << e.closest[i] << std::endl;
    if ((*e.p1 == *e.closest[i].p1) || (*e.p2 == *e.closest[i].p2)) {
      e.closest.erase(e.closest.begin()+i);
//      std::cerr << "--erased" << std::endl;
    }
    --i;
  }
//  std::cerr << "finished validating: " << e << std::endl;
//  std::cerr << "closest edges:" << std::endl;
//  for (unsigned int i = 0; i < e.closest.size(); ++i) std::cerr << e.closest[i] << std::endl;
}

// function to find the current closest edge in 'y_set' for 'e' which was inserted at 'iter'
// Updates the 'closest' property of 'e' if necessary.
// Updates the incidental edge in y_set if 'e' is a valid 'closest' candidate of 'inc_e'
void find_update_closest(E_Edge& e, std::set<E_Edge>::iterator& iter, std::set<E_Edge>& y_set) {
//  std::cerr << "=== in find_update_closest function ===" << std::endl;
  if (y_set.size() < 2) return;
  E_Edge inc_e;
  double angle_right;
  std::pair<std::set<E_Edge>::iterator, bool> retval_e, retval_i;
  retval_e.first = iter;
  retval_e.second = true;
//  std::cerr << "current edge: " << e << std::endl;
//  for (unsigned int i = 0; i < e.closest.size(); ++i) std::cerr << "closest[" << i << "]: " << e.closest[i] << std::endl;
//  if (y_set.size() > 2) {
//    std::cerr << "iter: " << *iter << std::endl;
//    if (iter != y_set.begin()) std::cerr << "prev: " << *(std::prev(iter)) << std::endl;
//    if (iter != std::prev(y_set.end())) std::cerr << "next: " << *(std::next(iter)) << std::endl;
//  }

  // find the correct incidental edge
  if (e.bin) {
    if (iter != y_set.begin()) {
      retval_i.first = std::prev(iter);
      retval_i.second = true;
      inc_e = *(retval_i.first);
    }
    else {
      std::cerr << "Error!  No valid incidental edge was found!" << std::endl;
      return;
    }
  }
  else {
    if (iter != std::prev(y_set.end())) {
      retval_i.first = std::next(iter);
      retval_i.second = true;
      inc_e = *(retval_i.first);
    }
    else {
      std::cerr << "Error!  No valid incidental edge was found!" << std::endl;
      return;
    }
  }
//  std::cerr << "inc_e: " << inc_e << std::endl;
//  for (unsigned int i = 0; i < inc_e.closest.size(); ++i) std::cerr << "closest[" << i << "]: " << inc_e.closest[i] << std::endl;
//  std::cerr << "before: edges in 'y_set':" << std::endl;
//  for (std::set<E_Edge>::iterator it=y_set.begin(); it!=y_set.end(); ++it) std::cerr << *it << std::endl;
//  if (e.bin ^ inc_e.bin) std::cerr << "edges facing each other properly" << std::endl;
//  else std::cerr << "edges not facing each other." << std::endl;

  // if 'closest' property has no edges, the current incidental edge in the right direction gets added.
  if ((e.closest.size() == 0) || ((*e.p2 == *inc_e.p2) && (inc_e != e.closest[e.closest.size()-1]))) {
//    std::cerr << "pushing inc_e to e.closest" << std::endl;
    e.closest.push_back(inc_e);
    e.angle = get_smaller_angle(e, inc_e, true);
    update_edge_in_set(e, retval_e, y_set);
  }
  // else the incidental edge only gets added if the smaller angle 'e' makes with the latest 'closest' edge
  // is larger than the larger angle 'e' makes with the incidental edge.
  else if (e.closest[e.closest.size()-1] != inc_e) {
      // 'e.angle' are older calculations of edges in 'closest', where 'angle' should be the smallest angle seen so far.
//      std::cerr << "checking 'e.angle' vs. angles of 'inc_e'" << std::endl;
//      std::cerr << "e:     " << e << std::endl;
//      std::cerr << "inc_e: " << inc_e << std::endl;
      angle_right = get_smaller_angle(e, inc_e, true);
//      std::cerr << "e.angle: " << e.angle << ", angle_right (p1): " << angle_right << std::endl;
      if (fabs(angle_right) <= fabs(e.angle)) {
//        std::cerr << "e: right is smaller" << std::endl;
        e.closest.push_back(inc_e);
        e.angle = angle_right;
        update_edge_in_set(e, retval_e, y_set);
      }
  }
  // checking if 'e' needs to be in 'inc_e.closest'
  if ((inc_e.closest.size() == 0) || ((*e.p2 == *inc_e.p2) && (e != inc_e.closest[inc_e.closest.size()-1]))) {
//    std::cerr << "pushing e to inc_e.closest" << std::endl;
    inc_e.closest.push_back(e);
    inc_e.angle = get_smaller_angle(inc_e, e, true);
    //retval.first = (e.bin) ? std::prev(iter) : std::next(iter);
    update_edge_in_set(inc_e, retval_i, y_set);
  }
  else if (inc_e.closest[inc_e.closest.size()-1] != e) {
//    std::cerr << "checking 'inc_e.angle' vs. inc_e-e" << std::endl;
    // 'e' is being inserted, which means there is a possibility that inc_e closest is on the other side of 'e'
    // if inc_e.closest[last] is on other side of e, remove inc_e.closest, then continue
    E_Edge other_side;
    //bool check_other_side = false;
    // 'iter' is a iterator to 'e', so 'next(iter)' should be the other side, if it exists.
    if (!e.bin && (std::next(iter) != y_set.end()) && (*(std::next(iter)) == inc_e.closest[inc_e.closest.size()-1])) {
//      std::cerr << "other side is next(iter): " << *(std::next(iter)) << std::endl;
//      std::cerr << "inc_e.closest[last] is : " << inc_e.closest[inc_e.closest.size()-1] << std::endl;
      inc_e.closest.erase(inc_e.closest.end()-1);
    }
    else if (e.bin && (iter != y_set.begin()) && (*(std::prev(iter)) == inc_e.closest[inc_e.closest.size()-1])) {
//      std::cerr << "other side is prev(iter): " << *(std::prev(iter)) << std::endl;
//      std::cerr << "inc_e.closest[last] is : " << inc_e.closest[inc_e.closest.size()-1] << std::endl;
      inc_e.closest.erase(inc_e.closest.end()-1);
    }


//    std::cerr << "e:     " << e << std::endl;
//    std::cerr << "inc_e: " << inc_e << std::endl;
    angle_right = get_smaller_angle(inc_e, e, true);
//    std::cerr << "angle: " << inc_e.angle << ", inc_e angle_right (p1): " << angle_right << std::endl;
    if (fabs(angle_right) <= fabs(inc_e.angle)) {
//      std::cerr << "inc_e: right is smaller" << std::endl;
      inc_e.closest.push_back(e);
      inc_e.angle = angle_right;
      //retval.first = (e.bin) ? std::prev(iter) : std::next(iter);
      update_edge_in_set(inc_e, retval_i, y_set);
    }
  }

//  std::cerr << "after: edges in 'y_set':" << std::endl;
//  for (std::set<E_Edge>::iterator it=y_set.begin(); it!=y_set.end(); ++it) std::cerr << *it << std::endl;
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

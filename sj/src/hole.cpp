#include <iostream> // for endl
#include <vector>
#include <set>
#include <assert.h>
#include <utility> // for std::pair
#include <math.h>  // for signbit
#include <iterator>     // std::next
#include <algorithm> // for std::sort
#include "basicDefinitions.h"
#include "point.h"
#include "edge.h"
#include "hole.h"
#include "pol.h"
#include "rand.h"
#include "simple_pol_check.h"


// overarching function:
unsigned int generateHoles(std::vector<std::vector<unsigned int>>& sph, std::vector<Point>& points, unsigned int nr_holes) {
  // 1) polygon in sph[0] is the main polygon that should have holes
  // 2) any already generated holes should be numbered in sph[i], where i != 0
  // 3) call function to grab all the inner chains that can make holes and the number of points in them.

  std::vector<unsigned int> ch; // points on the convex hull.
  unsigned int total_inner_points, total_hole_points;;
  std::vector<Ends> ends;
  std::vector<std::pair<unsigned int, unsigned int>> holes;
  get_convex_hull(ch, sph[0], points);
  // 'valid' means it threw out ends if: is_2D == false, for the chain between the ends
  get_valid_inner_chains_to_ch(ends, ch, sph[0], points);
  //if there are holes in sph, add them to 'holes'
  for (unsigned int i = 1; i < sph.size(); ++i) {
    if (sph[i].size() > 5) {holes.push_back(std::make_pair(i, sph[i].size()));}
  }

  do {
    total_inner_points = 0;
    total_hole_points = 0;

    std::cerr << "inner chains:" << std::endl;
    for(unsigned int i = 0; i < ends.size();++i) {
      std::cerr << ends[i] << std::endl;
      total_inner_points = total_inner_points + ends[i].nr_holes;
    }
    std::cerr << "holes: " << std::endl;
    // grab all the points in the already generated holes (min points to make 2 holes = 6)
    for (unsigned int i = 0; i < holes.size(); ++i) {
      std::cerr << "i: " << holes[i].first << ", size: " << holes[i].second << std::endl;
      total_hole_points = total_hole_points + ((holes[i].second < 6) ? 0 : holes[i].second);
    }
    std::cerr << "Total inner points: " << total_inner_points << ", total hole points: " << total_hole_points << std::endl;
    if (total_inner_points+total_hole_points == 0) break;


    // randomly pick a chain or hole based on the number of points.
    unsigned int selection=0, hole_i=0;
    bool is_end = false;
    // [0, total inner points] is a specific end, [total_inner_points+1, total_hole_points] is a specific hole
    UniformRandomI(selection, 1, total_inner_points+total_hole_points);
    std::cerr << "randomly selected: " << selection << std::endl;

    if (selection <= total_inner_points) {
      is_end = true;
      for(unsigned int i = 0; i < ends.size(); ++i) {
        if (selection <= ends[i].nr_holes) {
          selection = i;
          break;
        }
        else {
          selection = selection - ends[i].nr_holes;
        }
      }
    }
    else {
      is_end = false;
      selection = selection - total_inner_points;
      for (unsigned int i = 0; i < holes.size(); ++i) {
        if (selection <= holes[i].second) {
          selection = holes[i].first;
          hole_i = i;
          break;
        }
        else {
          selection = selection - holes[i].second;
        }
      }
    }
    // selection is now the index into either 'sph' or 'ends', is_end tells you which is which.
    std::cerr << "selection: " << selection << ", is_end: " << is_end << std::endl;

    // 6) call a function 'get_hole()' that uses the linesweep on the chain or hole
    // - returns 'true' if it changed sph and added a hole, false if it didn't.
    // - returns 'false' if no candidate was found that was valid, which invalidates splitting the chain/hole, so it can be discarded.
    // - 2 different functions whether it's a chain or polygon
    enum error result;
    if(is_end) {
      result = get_hole(ends[selection], sph, points);
      if (result == HOLES_INADEQUATE) {
        ends.erase(ends.begin()+selection);
      }
      else {
        --nr_holes;
        update_end(selection, ends, ch, sph[0], points);
        if (sph.back().size() > 5) {
          holes.push_back(std::make_pair(sph.size()-1, sph.back().size()));
          total_hole_points = total_hole_points + ((sph.back().size() < 6) ? 0 : sph.back().size());
        }
      }
    }
    else {
      result = get_hole(selection, sph, points);
      if (result == HOLES_INADEQUATE) {
        holes.erase(holes.begin()+hole_i);
      }
      else {
        --nr_holes;
        if (sph[selection].size() < 6) holes.erase(holes.begin()+hole_i);
        else holes[hole_i].second = sph[selection].size();
        if (sph.back().size() > 5) {
          holes.push_back(std::make_pair(sph.size()-1, sph.back().size()));
          total_hole_points = total_hole_points + ((sph.back().size() < 6) ? 0 : sph.back().size());
        }
      }
    }
    std::cerr << "hole result: "  << result << std::endl;
    std::cerr << "holes left: " << nr_holes << ", ends left: " << ends.size() << ", total hole points:" << total_hole_points << std::endl;
  } while (nr_holes > 0 && (ends.size() > 0 || total_hole_points > 0));
  return nr_holes;
}

// 7) repeat the random pick until number of holes desired has been fulfilled.

// get_hole() function
// - needs to know whether it's a chain or a closed chain so it can properly deal with the fictional c.h. edge of a chain.
// - needs the polygon, or the polygon and the points that define the inner chain, i.e. 2 functions.
enum error get_hole(Ends end, std::vector<std::vector<unsigned int>>& sph, std::vector<Point>& points) {
  std::cerr << "in get_hole, ends version: " << end << ", sph.size: " << sph.size() << ", p.size: " << points.size() << std::endl;

  //'polygonal_chain' are indices into 'points', not into 'inner_points'
  std::vector<unsigned int> polygonal_chain;
  get_inner_chain_polygon(polygonal_chain, end, sph[0]);
//  std::cerr << "poly chain:" << std::endl;
//  pdisplay(polygonal_chain, points);

  // vector of 'Point' points for the chain.
  std::vector<Point> inner_points;
  get_inner_chain_points(inner_points, polygonal_chain, points);
//  std::cerr << "inner_points:" << std::endl;
//  pdisplay(inner_points);

  // 1) set up a linesweep that populates valid_candidates vector.
  std::vector<E_Edge> candidates, valid_candidates;
  //                                                is_chain, l2r
  sweep_polygonal_chain(candidates, inner_points, true, true);
  sweep_polygonal_chain(candidates, inner_points, true, false);
  std::sort(candidates.begin(), candidates.end(), candidate_compare);

  E_Edge last, next_last;
  std::cerr << "sorting candidates:" << std::endl;
  while (candidates.size() > 1) {
    last = candidates.back();
    next_last = candidates[candidates.size()-2];
    std::cerr << "last:      " << last << std::endl;
    std::cerr << "next_last: " << next_last << std::endl;
    if (last != next_last) candidates.pop_back();
    else {
      std::cerr << "last.closest: " << last.closest.back() << std::endl;
      std::cerr << "next_last.cl: " << next_last.closest.back() << std::endl;
      if (last.closest.back() == next_last.closest.back()) {
        std::cerr << "found a pair" << std::endl;
        //                            is_chain
        if (is_2D(last, inner_points, true)) valid_candidates.push_back(last);
        candidates.pop_back();
      }
      candidates.pop_back();
    }
  }

  std::cerr << "valid candidates:" << std::endl;
  for (unsigned int i = 0; i < valid_candidates.size(); ++i) {
    std::cerr << "i: " << i << ", edge: " << valid_candidates[i] << std::endl;
    for (unsigned int j = 0; j < valid_candidates[i].closest.size();++j) {
      std::cerr << "            " << valid_candidates[i].closest[j] << std::endl;
    }
  }
  // at this point I have valid candidates that split the chain into 2 good pieces.

  if (valid_candidates.size() > 0) {
    unsigned int r_cand = 0;
    UniformRandomI(r_cand, 0, valid_candidates.size()-1);
    std::cerr << "random candidate: " << valid_candidates[r_cand] << std::endl;

    E_Edge original = E_Edge(&points[polygonal_chain[(*valid_candidates[r_cand].p1).i]],
                             &points[polygonal_chain[(*valid_candidates[r_cand].p2).i]]);
    original.closest.push_back(D_Edge(&points[polygonal_chain[(*valid_candidates[r_cand].closest[0].p1).i]],
                                      &points[polygonal_chain[(*valid_candidates[r_cand].closest[0].p2).i]]));
    std::cerr << "original: " << original << std::endl;
    std::cerr << " closest: " << original.closest[0] << std::endl;
    //                                 hole_selection, is_chain
    get_hole_and_new_pol(original, sph, points, 0, false);
    return SUCCESS;
  }
  else {
    return HOLES_INADEQUATE;
  }
}

enum error get_hole(unsigned int& selection, std::vector<std::vector<unsigned int>>& sph, std::vector<Point>& points) {
  std::cerr << "in get_hole, hole version: " << ", sph.size: " << sph.size() << ", p.size: " << points.size() << std::endl;

  // vector of 'Point' points for the chain.
  std::vector<Point> inner_points;
  get_inner_chain_points(inner_points, sph[selection], points);
//  std::cerr << "inner_points:" << std::endl;
//  pdisplay(inner_points);

  // 1) set up a linesweep that populates valid_candidates vector.
  std::vector<E_Edge> candidates, valid_candidates;
  //                                           is_chain, l2r
  sweep_polygonal_chain(candidates, inner_points, false, true);
  sweep_polygonal_chain(candidates, inner_points, false, false);
  std::sort(candidates.begin(), candidates.end(), candidate_compare);

  E_Edge last, next_last;
  std::cerr << "sorting candidates:" << std::endl;
  while (candidates.size() > 1) {
    last = candidates.back();
    next_last = candidates[candidates.size()-2];
    std::cerr << "last:      " << last << std::endl;
    std::cerr << "next_last: " << next_last << std::endl;
    if (last != next_last) candidates.pop_back();
    else {
      std::cerr << "last.closest: " << last.closest.back() << std::endl;
      std::cerr << "next_last.cl: " << next_last.closest.back() << std::endl;
      if (last.closest.back() == next_last.closest.back()) {
        std::cerr << "found a pair" << std::endl;
        //                            is_chain
        if (is_2D(last, inner_points, false)) valid_candidates.push_back(last);
        candidates.pop_back();
      }
      candidates.pop_back();
    }
  }

  for (unsigned int i = 0; i < valid_candidates.size(); ++i) {
    std::cerr << "i: " << i << ", edge: " << valid_candidates[i] << std::endl;
    for (unsigned int j = 0; j < valid_candidates[i].closest.size();++j) {
      std::cerr << "            " << valid_candidates[i].closest[j] << std::endl;
    }
  }
  // at this point I have valid candidates that split the chain into 2 good pieces.

  if (valid_candidates.size() > 0) {
    unsigned int r_cand = 0;
    UniformRandomI(r_cand, 0, valid_candidates.size()-1);
    std::cerr << "random candidate: " << valid_candidates[r_cand] << std::endl;

    E_Edge original = E_Edge(&points[sph[selection][(*valid_candidates[r_cand].p1).i]],
                             &points[sph[selection][(*valid_candidates[r_cand].p2).i]]);
    original.closest.push_back(D_Edge(&points[sph[selection][(*valid_candidates[r_cand].closest[0].p1).i]],
                                      &points[sph[selection][(*valid_candidates[r_cand].closest[0].p2).i]]));
    std::cerr << "original: " << original << std::endl;
    std::cerr << " closest: " << original.closest[0] << std::endl;
    //                                 hole_selection, is_hole
    get_hole_and_new_pol(original, sph, points, selection, true);
    return SUCCESS;
  }
  else {
    return HOLES_INADEQUATE;
  }


  return SUCCESS;
}

bool candidate_compare(E_Edge e1, E_Edge e2) {
  if (*e1.p1 == *e2.p1) {
    if (*e1.p2 == *e2.p2) {
      if (*e1.closest.back().p1 == *e2.closest.back().p1)
        return *e1.closest.back().p2 < *e2.closest.back().p2;
      return *e1.closest.back().p1 < *e2.closest.back().p1;
    }
    return (*e1.p2 < *e2.p2);
  }
  return (*e1.p1 < *e2.p1);
}

// Function that sweeps over points and finds candidates to split the polygon in points into 2,
// for valid candidates it checks if the 2 polygons would be valid.
// INPUT:
//        valid_candidates: empty vector of E_Edge that represents a valid candidate.
//        ignore_ch_ends: a boolean that tells us whether the polygon in 'points'
//                        is a chain or a closed chain, a chain ignores candidates with an edge between ends.
//        points: the points of a chain or a closed chain that is to be sweeped.
//        is_chain: if the polygon is an open chain or not.
//        l2r: if we should sweep left2right or right2left.
enum error sweep_polygonal_chain(std::vector<E_Edge>& valid_candidates, std::vector<Point>& points, bool is_chain, bool l2r) {
  std::cerr << "in sweep_polygonal_chain" << std::endl;
  unsigned int end, i;
  int step;
  std::set<E_Edge> ls_edges;
  std::pair<std::set<E_Edge>::iterator, bool> retval1, retval2; // return values from the ls_edges 'insert' or 'erase' functions.
  Point *m, *l, *r; // m is the current event point at lex. position 'i' and 'l' and 'r' are the 2 adjacent points in the polygon.
  bool isll, isrl, dbl_rem; // are 'l' and 'r' points left of 'm' in the euclidian planar sense.
  std::vector<unsigned int> ls_events (points.size()); // lexicographically sorted indexes of 'points'
  fill_lex(ls_events, points);
//  std::cerr << "ls_events:" << std::endl;
//  pdisplay(ls_events, points);

  if (l2r) {
    i = 0;
    end = ls_events.size();
    step = 1;
  }
  else {
    i = ls_events.size()-1;
    end = -1;
    step = -1;
  }

  while (i != end) {
    std::cerr << std::endl << "i: " << i << std::endl;
    std::cerr << "ls_edges:" << std::endl;
    for (std::set<E_Edge>::iterator i = ls_edges.begin(); i != ls_edges.end(); ++i) std::cerr << *i << std::endl;

    m = &points[ls_events[i]];
    l = &points[(points.size() + (*m).v - 1) % points.size()];
    r = &points[(points.size() + (*m).v + 1) % points.size()];
    std::cerr << "m: " << *m << ", l: " << *l << ", r: " << *r << std::endl;

    // create 2 new 'E_Edge's
    E_Edge e1 = E_Edge (m, l);
    E_Edge e2 = E_Edge (m, r);
    std::cerr << "e1: " << e1 << std::endl;
    std::cerr << "e2: " << e2 << std::endl;

    // check for 'o<', '-o-', '>o' condition
    (*m < *l) ? isll = false : isll = true;
    (*m < *r) ? isrl = false : isrl = true;

    if (((isll && isrl) && l2r) || ((!isll && !isrl) && !l2r)) {
      std::cerr << "=== >o ===" << std::endl;
      dbl_rem = true;
      // removing both edges from the set, and going over their 'closest' from the other direction.
      // find 'e1' in 'y_set' and update with values of the edge in 'y_set'
      retval1.first = ls_edges.find(e1);
      if (e1 != *retval1.first) {
        std::cerr << "Error!  could not find edge" << e1 << " in ls_edges!" << std::endl;
      }

      e1 = (*retval1.first); // there are properties of the edge in the set that are needed.
      removal_update(e1, retval1.first, valid_candidates, ls_edges, dbl_rem, l2r);

      retval2.first = ls_edges.find(e2);
      if (e2 != *retval2.first) {
        std::cerr << "Error!  could not find edge" << e2 << " in ls_edges!" << std::endl;
      }

      e2 = (*retval2.first); // there are properties of the edge in the set that are needed.
      removal_update(e2, retval2.first, valid_candidates, ls_edges, dbl_rem, l2r);

      //Adjacent edges and valid_candidates updated, we can remove both edges.
      ls_edges.erase(retval1.first);
      ls_edges.erase(retval2.first);

    }
    else if (isll ^ isrl) {
      std::cerr << "=== -o- ===" << std::endl;
      dbl_rem = false;
      E_Edge old_e, new_e;
      if (((*e1.p1 < *e2.p2) && l2r) || ((*e2.p1 < *e1.p2) && !l2r)) {
        old_e = e1;
        new_e = e2;
      }
      else {
        old_e = e2;
        new_e = e1;
      }
//      std::cerr << "old_e: " << old_e << ", new_e: " << new_e << std::endl;

      // find 'old' in 'y_set'
      retval1.first = ls_edges.find(old_e);
      if (old_e != *retval1.first) {
        std::cerr << "Error!  could not find edge" << old_e << " in ls_edges!" << std::endl;
      }
      // copy values from iterator to 'new_e'
      old_e = *retval1.first;
      removal_update(old_e, retval1.first, valid_candidates, ls_edges, dbl_rem, l2r);

      new_e.bin = old_e.bin;
      retval2.first = std::next(retval1.first);
      retval2.first = ls_edges.insert(retval2.first, new_e);

      //need to remove old_e before I do any checking of adjacent edges of new_e
      ls_edges.erase(retval1.first);
      //                                           dbl_ins
      insert_update(new_e, retval2.first, ls_edges, false, is_chain, l2r);
    }
    else {
      std::cerr << "=== o< ===" << std::endl;
      dbl_rem = false;
      retval1 = ls_edges.insert(e1);
      if (e1 != *retval1.first) std::cerr << "Error!  could not find edge" << e1 << " in ls_edges!" << std::endl;
      retval2 = ls_edges.insert(e2);
      if (e2 != *retval2.first) std::cerr << "Error!  could not find edge" << e2 << " in ls_edges!" << std::endl;

      set_bin(e1, e2, retval1.first, retval2.first, ls_edges);
      remove_adjacent_candidate(e1, e2, retval1.first, retval2.first, ls_edges);
      //                                      dbl_ins
      insert_update(e1, retval1.first, ls_edges, true, is_chain, l2r);
      insert_update(e2, retval2.first, ls_edges, true, is_chain, l2r);
    }

    i = i + step;
  }

  return SUCCESS;
}

// Function to remove the candidate of the 2 adjacent edges before the double event happened, if they are each others candidate.
// If the double event is not facing each others 'in' sides, the 2 adjacent edges were adjacent to each others in sides.
// The lex earlier adjacent edge should be checked if it has the later edge as a candidate, remove it if it has it.
void remove_adjacent_candidate(E_Edge& e1, E_Edge& e2, std::set<E_Edge>::iterator& it1, std::set<E_Edge>::iterator& it2,
                               std::set<E_Edge>& ls_edges) {
  std::cerr << "inside remove_adjacent_candidate" << std::endl;
//  std::cerr << "ls_edges:" << std::endl;
//  for (std::set<E_Edge>::iterator i = ls_edges.begin(); i != ls_edges.end(); ++i) std::cerr << *i << std::endl;

  E_Edge adj1, adj2;
  std::set<E_Edge>::iterator adj_it1, adj_it2;

  if ((e1 < e2) && e2.bin) return;
  if ((e2 < e1) && e1.bin) return;

  if (e1 < e2) {
    adj_it1 = std::prev(it1);
    adj1 = *adj_it1;
    std::cerr << "adj1: " << adj1 << std::endl;
    adj_it2 = std::next(it2);
    adj2 = *adj_it2;
    std::cerr << "adj2: " << adj2 << std::endl;

    if (*adj1.p1 < *adj2.p1) {
      if (adj1.closest.size() == 0) return;
      std::cerr << "closest removed: " << adj1 << ", closest: " << adj1.closest.back() << std::endl;
      E_Edge b = E_Edge(adj1.closest.back());
      if (b == adj2) adj1.closest.pop_back();
      update_edge_in_set(adj1, adj_it1, ls_edges);
    }
    else {
      if (adj2.closest.size() == 0) return;
      std::cerr << "closest removed: " << adj2 << ", closest: " << adj2.closest.back() << std::endl;
      E_Edge b = E_Edge(adj2.closest.back());
      if (b == adj1) adj2.closest.pop_back();
      update_edge_in_set(adj2, adj_it2, ls_edges);
    }
  }
  else {
    adj_it1 = std::prev(it2);
    adj1 = *adj_it1;
    adj_it2 = std::next(it1);
    adj2 = *adj_it2;

    if (*adj1.p1 < *adj2.p1) {
      if (adj1.closest.size() == 0) return;
      std::cerr << "closest removed: " << adj1 << ", closest: " << adj1.closest.back() << std::endl;
      E_Edge b = E_Edge(adj1.closest.back());
      if (b == adj2) adj1.closest.pop_back();
      update_edge_in_set(adj1, adj_it1, ls_edges);
    }
    else {
      if (adj2.closest.size() == 0) return;
      std::cerr << "closest removed: " << adj2 << ", closest: " << adj2.closest.back() << std::endl;
      E_Edge b = E_Edge(adj2.closest.back());
      if (b == adj1) adj2.closest.pop_back();
      update_edge_in_set(adj2, adj_it2, ls_edges);
    }
  }
}

// Function to update 'bin' property of 2 edges in a double insert event.
// INPUT:
//      e1, e2 : 2 edges of a double insert event.
//      ls_edges: the linesweep BST for edges.
//      it1, it2  : iterators to the edges 'e1' and 'e2' in 'ls_edges'
// OUTPUT:  Updates 'bin' property of the 2 edges.
bool set_bin(E_Edge& e1, E_Edge& e2, std::set<E_Edge>::iterator& it1, std::set<E_Edge>::iterator& it2, std::set<E_Edge>& ls_edges) {
//  std::cerr << "inside set bin" << std::endl;
//  std::cerr << "e1: " << e1 << std::endl;
//  std::cerr << "e2: " << e2 << std::endl;
  if (e1 < e2) { // next(it1) should == it2
    if (it1 != ls_edges.begin()) {
      e1.bin = !(*(std::prev(it1))).bin;
      e2.bin = !e1.bin;
    }
    else {
      e1.bin = false; // e1 the first (lowest) edge, "below edge is inside" (bin) must be false
      e2.bin = true;
    }
  }
  else {
    if (it2 != ls_edges.begin()) {
      e2.bin = !(*(std::prev(it2))).bin;
      e1.bin = !e2.bin;
    }
    else {
      e2.bin = false; // e2 the last (highest) edge,  "below edge is inside" (bin) must be true.
      e1.bin = true;
    }
  }
}

// function that does 2 things:
// 1) if event is a double insert event and adjacent edges face each other, i.e. dbl_ins and e.p1 == adj_e.p1 is true,
//    only adds the (e.p1,adj_e.p2) as an angle to both edges.
// 2) if no double insert, finds the adjacent edge of 'e', checks if 'e' is a valid candidate and if true,
//    adds 'e' to 'closest', updates the angle for both edges and reinserts 'adj_e' and inserts 'e'
// INPUT:
//      e: edge that was inserted at the event point
//      it: iterator of edge 'e'
//      ls_edges: the edge set for the linesweep
//      dbl_ins: is true if event is "double insert and both inserted edges face each others 'in' sides"
//      is_chain: true if it's an open chain and any edge between the ends should be ignored.
//      l2r: is true if the linesweep sweeps left2right.
void insert_update(E_Edge& e, std::set<E_Edge>::iterator& it, std::set<E_Edge>& ls_edges, bool dbl_ins, bool is_chain, bool l2r) {
  std::cerr << "inside insert_update" << std::endl;
  bool skip = false;
  double angle;
  E_Edge adj_e;
  std::set<E_Edge>::iterator adj_it;
  // find the correct incidental edge
  if (e.bin) adj_it = std::prev(it);
  else adj_it = std::next(it);
  adj_e = *(adj_it);
  std::cerr << "edge: " << e << ", it: " << *it << std::endl;
  std::cerr << "adjacent edge: " << adj_e << ", adj_it: " << *adj_it << std::endl;
  if (is_chain && (*e.p1).i == 0 && (*e.p2).i != 1) skip = true;
  if (is_chain && (*e.p2).i == 0 && (*e.p1).i != 1) skip = true;
  if (is_chain && (*adj_e.p1).i == 0 && (*adj_e.p2).i != 1) skip = true;
  if (is_chain && (*adj_e.p2).i == 0 && (*adj_e.p1).i != 1) skip = true;

  if (!skip) {
    if (dbl_ins && ((*e.p1 == *adj_e.p1) || (*e.p2 == *adj_e.p2))) {
      std::cerr << "o< event" << std::endl;
      if (*e.p1 == *adj_e.p1) e.angle = get_angle(e, *adj_e.p2, l2r);
      else e.angle = get_angle(e, *adj_e.p1, l2r);
    }
    else {
      //                                dbl_rem
      e.angle = get_angle(e, adj_e, l2r, false);
      angle = get_angle(adj_e, e, l2r, false);
      std::cerr << "angle: " << angle << ", adj.angle: " << adj_e.angle << std::endl;
      if (angle < adj_e.angle) {
        std::cerr << "updating adj_e.angle" << std::endl;
        adj_e.angle = angle;
        adj_e.closest.push_back(e);
        update_edge_in_set(adj_e, adj_it, ls_edges);
      }
    }
  }
  else {
    // this is an inner polygonal chain, and the end is between a fictional edge
    // between the c.h. points, which cannot be used anyway
    std::cerr << "end edge, skipping" << std::endl;
  }
  update_edge_in_set(e, it, ls_edges);
  std::cerr << "after insert:" << std::endl;
  std::cerr << "edge: " << e << std::endl;
  std::cerr << "adjacent edge: " << adj_e << std::endl;
}

// function that does 2 things:
// 1) If 'e1' has any candidates adds 'e1' to 'valid_candidates'
// 2) updates 'angle' property of the adjacent edge to 'e1' on its 'in' side in 'ls_edges',
//    but only if it's a double removal.
// INPUT:
//      e1: edge at event point being removed
//      it: iterator to 'e1' in ls_edges
//      ls_edges: linesweep edge set
//      dbl_rem: whether the event is a double remove event
//      l2r: direction of the linesweep, true if sweeping 'left to right'
void removal_update(E_Edge& e, std::set<E_Edge>::iterator& it, std::vector<E_Edge>& valid_candidates,
                    std::set<E_Edge>& ls_edges, bool dbl_rem, bool l2r) {
  std::cerr << "inside removal_update" << std::endl;
  std::cerr << "e: " << e << std::endl;
  while (e.closest.size() > 0) {
    if (candidate_compare(e.closest.back(),e)) {
      E_Edge cand_e = E_Edge(e.closest.back());
      cand_e.closest.push_back(e);
      valid_candidates.push_back(cand_e);
    }
    else {
      E_Edge cand_e = E_Edge(e);
      cand_e.closest.clear();
      cand_e.closest.push_back(e.closest.back());
      valid_candidates.push_back(cand_e);
    }
    e.closest.pop_back();
  }

  if (dbl_rem) {
    double angle;
    E_Edge adj_e;
    std::set<E_Edge>::iterator adj_it;
    // find the correct incidental edge
    if (e.bin) adj_it = std::prev(it);
    else adj_it = std::next(it);
    adj_e = *(adj_it);
    std::cerr << "adjacent edge: " << adj_e << std::endl;
    std::cerr << "adj.angle: " << adj_e.angle << std::endl;

    angle = get_angle(adj_e, e, l2r, dbl_rem);
    std::cerr << "new angle: " << angle << std::endl;
    if (angle < adj_e.angle) {
      adj_e.angle = angle;
//    std::cerr << "angle: " << adj_e.angle << std::endl;
      update_edge_in_set(adj_e, adj_it, ls_edges);
    }
  }
}

// function to update edge in set, i.e. find it, remove it and insert it again.
void update_edge_in_set(E_Edge& e, std::set<E_Edge>::iterator& it, std::set<E_Edge>& ls_edges) {
//  std::cerr << "=== update_edge_in_set ===" << std::endl;
  if (e == *it) {
    std::set<E_Edge>::iterator new_it = std::next(it);
    //std::pair<std::set<E_Edge>::iterator, bool> retval;
    ls_edges.erase(it);
    new_it = ls_edges.insert(new_it, e);
    if (*new_it != e) {
      std::cerr << "Error!  update_edge_in_set failed to insert edge!" << std::endl;
      std::cerr << "e: " << e << std::endl;
      std::cerr << "iterator: " << *new_it << std::endl;
    }
  }
  else {
    std::cerr << "Error! Update_edge_in_set got an edge and iterator that did not match" << std::endl;
    std::cerr << "e: " << e << std::endl;
    std::cerr << "iterator: " << *it << std::endl;
  }
}

// - each event can add candidates, remove them, and update angles for each edge.
// - should have all candidates that were fine in regards to 'p1' points.
// - Candidates can be pushed to a vector per edge, then when edge is removed, copy edge pairs to 'candidates' vector.
// 3) set up a linesweep loop from highest to lowest.
// 4) deal with each event.
// - should have all candidates that were fine in regards to 'p2' points.
// - removal of edges pushes the valid candidates onto the 'candidates' vector, but...
// - candidates are only older edge to the new edge as seen from left-to-right, so the valid candidates need to be swapped.
// 5) Let's assume the candidates are edge-pairs, 2) and 4) should have given us a vector of edge-pairs
// - If we sort the vector, valid candidates seen from 'p1' and 'p2' should be adjacent, i.e. doubled in the vector,
// - so after sort we can remove singles and leave just one of the doubles.
// 6) pick a random candidate and check if it splits the chain into 2 valid polygons, if not remove it and repeat.
// 7) if we find a good candidate, call a function 'split_chain' that splits polygon sph[0] into new sph[0] and a new hole at the end of sph,
// - or if it was a hole, split the sph[hole_index] into new sph[hole_index] and new hole at end of sph.
// - then return with 'true'.
// ((8) if we don't find a good candidate, we could swap x and y coordinates and do the linesweep again from 1)))
// 9) if we find no good candidate, we must return with a 'false'.

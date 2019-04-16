#include <iostream> // for endl
#include <vector>
#include <list>
#include <set>
#include <utility> // for std::pair
#include <stdlib.h>  // for abs
#include <algorithm>    // std::sort
#include <iterator> // for std:prev and std::next
#include "basicDefinitions.h"
#include "basicFunctions.h"
#include "point.h"
#include "edge.h"
#include "randomGenerator.h"

// comparison function for sorting pairs by secondary value
bool sortbysec(const std::pair<Point*,double> &a, const std::pair<Point*,double> &b) {
    return (a.second > b.second);
}

bool sortbyint(const int i, const int j) { return (i<j); }

// function to remove edges from 'edgeS' up to and including value of 'index'
void decrementEdges(unsigned int index, std::set<Edge, setComp>& edgeS) {
	std::set<Edge, setComp>::iterator it = edgeS.begin();
	while (it != edgeS.end()) {
		if ((*it).l_idx >= index) it = edgeS.erase(it);
		else ++it;
	}
}

// swaps the order of points of the edge in the polygon,
// i.e. v values of the points in the edge 'e' and order of points in 'polygon'.
void polSwap(Edge e, std::vector<unsigned int>& polygon) {
  unsigned int temp = (*e.p1).v;
  (*e.p1).v = (*e.p2).v;
  (*e.p2).v = temp;
  polygon[(*e.p1).v] = (*e.p1).i;
  polygon[(*e.p2).v] = (*e.p2).i;
}
void polSwap(Point* a, Point* b, std::vector<unsigned int>& polygon) {
  unsigned int temp = (*a).v;
  (*a).v = (*b).v;
  (*b).v = temp;
  polygon[(*a).v] = (*a).i;
  polygon[(*b).v] = (*b).i;
}

// function that takes 3 points: a, b, and c that are already assumed collinear
// a is assumed to be the lowest point lexicographically as well as the middle point in polygon of the 3 points.
// swaps the points in the polygon so that the lex. order of the points is also the vertex order of the points.
bool collSwap(Point* a, Point* b, Point* c, std::vector<unsigned int>& polygon) {
  bool swapped = false;
  unsigned int left=-1, mid=-1, right=-1;
  //need to be able to catch boundary conditions.
  if ((*a).v == 0) {
    if ((*b).v == polygon.size()-1) {
      if ((*c).v == polygon.size()-2) {
        left = (*c).v; mid = (*b).v; right = (*a).v;
        
      } else {
        left = (*b).v; mid = (*a).v; right = (*c).v;
      }
    } else if ((*c).v == polygon.size()-1) {
        if ((*b).v == polygon.size()-2) {
          left = (*b).v; mid = (*c).v; right = (*a).v;
        } else {
          left = (*c).v; mid = (*a).v; right = (*b).v;
        }
      }
  }
  if ((*b).v == 0) {
    if ((*a).v == polygon.size()-1) {
      if ((*c).v == polygon.size()-2) {
        left = (*c).v; mid = (*a).v; right = (*b).v;
      } else {
        left = (*a).v; mid = (*b).v; right = (*c).v;
      }
    } else if ((*c).v == polygon.size()-1) {
      if ((*a).v == polygon.size()-2) {
        left = (*a).v; mid = (*c).v; right = (*b).v;
      } else {
        left = (*c).v; mid = (*b).v; right = (*a).v;
      }
    }
  }
  if ((*c).v == 0) {
    if ((*a).v == polygon.size()-1) {
      if ((*b).v == polygon.size()-2) {
        left = (*b).v; mid = (*a).v; right = (*c).v;
      } else {
        left = (*a).v; mid = (*c).v; right = (*b).v;
      }
    } else if ((*b).v == polygon.size()-1) {
      if ((*a).v == polygon.size()-2) {
        left = (*a).v; mid = (*b).v; right = (*c).v;
      } else {
        left = (*b).v; mid = (*c).v; right = (*a).v;
      }
    }
  }

  if ((*b) < (*c)) {
    if ((*b).v < (*a).v) {polSwap(a, b, polygon); swapped = true;}
    else {polSwap(a, c, polygon); polSwap(c, b, polygon); swapped = true;}
  } else {
    if ((*c).v < (*a).v) {polSwap(a, c, polygon); swapped = true;}
    else {polSwap(a, b, polygon); polSwap(c, b, polygon); swapped = true;}
  }
  if (!swapped) std::cerr << "Unexpected error in 3P collSwap!" << std::endl;
  return swapped;
}

// Function that takes 2 edges, e1 and e2 that both intersect and are collinear
bool collSwap (Edge& e1, Edge& e2, std::vector<unsigned int>& polygon) {
  double rd1 = reldist(e1, *e2.p1);
  double rd2 = reldist(e1, *e2.p2);
  bool d1 = ((int)(*e1.p1).v - (int)(*e1.p2).v) < 0; // true  means edge order in poly is : [...p1p2...]
  bool d2 = ((int)(*e2.p1).v - (int)(*e2.p2).v) < 0; // false means edge order in poly is : [...p2p1...]

  if ((rd1 > 1) && (rd2 > 1)) return false;
  if ((rd1 < 0) && (rd2 < 0)) return false;
  if ( d1 &&  d2 && (rd1 > 0) && (rd1 < 1) && (rd2 > 1)) {
    polSwap(e2.p1, e1.p2, polygon);
    return true;
  }
  if ( d1 && !d2 && (rd1 > 0) && (rd1 < 1) && (rd2 > 1)) {
    polSwap(e2.p1, e1.p2, polygon);
    polSwap(e1.p2, e2.p2, polygon);
    return true;
  }
  if (!d1 &&  d2 && (rd1 > 0) && (rd1 < 1) && (rd2 > 1)) {
    polSwap(e2.p1, e1.p2, polygon);
    polSwap(e1.p1, e2.p1, polygon);
    return true;
  }
  if (!d1 && !d2 && (rd1 > 0) && (rd1 < 1) && (rd2 > 1)) {
    polSwap(e2.p1, e1.p2, polygon);
    polSwap(e1.p1, e2.p1, polygon);
    polSwap(e1.p2, e2.p2, polygon);
    return true;
  }

  if ( d1 &&  d2 && (rd1 < 0) && (rd2 > 0) && (rd2 < 1)) {
    polSwap(e1.p1, e2.p2, polygon);
    return true;
  }
  if (!d1 &&  d2 && (rd1 < 0) && (rd2 > 0) && (rd2 < 1)) {
    polSwap(e1.p1, e2.p2, polygon);
    polSwap(e2.p2, e1.p2, polygon);
    return true;
  }
  if ( d1 && !d2 && (rd1 < 0) && (rd2 > 0) && (rd2 < 1)) {
    polSwap(e1.p1, e2.p2, polygon);
    polSwap(e2.p1, e1.p1, polygon);
    return true;
  }
  if (!d1 && !d2 && (rd1 < 0) && (rd2 > 0) && (rd2 < 1)) {
    polSwap(e1.p1, e2.p2, polygon);
    polSwap(e2.p1, e1.p1, polygon);
    polSwap(e2.p2, e1.p2, polygon);
    return true;
  }

  if ( d1 &&  d2 && (rd1 > 0) && (rd1 < 1) && (rd2 > 0) && (rd2 < 1)) {
    polSwap(e2.p1, e1.p2, polygon);
    polSwap(e2.p2, e1.p2, polygon);
    return true;
  }
  if ( d1 && !d2 && (rd1 > 0) && (rd1 < 1) && (rd2 > 0) && (rd2 < 1)) {
    polSwap(e2.p1, e1.p2, polygon);
    return true;
  }
  if (!d1 &&  d2 && (rd1 > 0) && (rd1 < 1) && (rd2 > 0) && (rd2 < 1)) {
    polSwap(e2.p1, e1.p2, polygon);
    polSwap(e1.p1, e2.p1, polygon);
    polSwap(e2.p2, e1.p2, polygon);
    return true;
  }
  if (!d1 && !d2 && (rd1 > 0) && (rd1 < 1) && (rd2 > 0) && (rd2 < 1)) {
    polSwap(e2.p1, e1.p2, polygon);
    polSwap(e1.p1, e2.p1, polygon);
    return true;
  }

  if ( d1 &&  d2 && (rd1 < 0) && (rd2 > 1)) {
    polSwap(e1.p1, e2.p2, polygon);
    polSwap(e1.p2, e2.p2, polygon);
    return true;
  }
  if (!d1 &&  d2 && (rd1 < 0) && (rd2 > 1)) {
    polSwap(e1.p1, e2.p2, polygon);
    return true;
  }
  if ( d1 && !d2 && (rd1 < 0) && (rd2 > 1)) {
    polSwap(e1.p1, e2.p2, polygon);
    polSwap(e2.p1, e1.p1, polygon);
    polSwap(e1.p2, e2.p2, polygon);
    return true;
  }
  if (!d1 && !d2 && (rd1 < 0) && (rd2 > 1)) {
    polSwap(e1.p1, e2.p2, polygon);
    polSwap(e2.p1, e1.p1, polygon);
    return true;
  }
  std::cerr << "ERROR in 4P collSwap!" << std::endl;
  std::cerr << "edge collswap: rd1: " << rd1 << ", dp2: " << rd2 << std::endl;
  std::cerr << "d1: " << ((d1) ? "true" : "false") << ", d2: " << ((d2) ? "true" : "false") << std::endl;
  return true;
}

// checks if an edge 'e' is: already in 'edgeS', if not checks if it intersects its neighbours and either cleans 'edgeS' or add 'e' into it.
enum edge_t processEdge(unsigned int& index, Edge& e, std::set<Edge, setComp>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
	enum edge_t valid = E_VALID;
	enum intersect_t cross = IS_FALSE;
	bool bef = false, af = false;
	// ok to try and insert the edge into 'edgeS', if it's already there, we have an iterator for removal of the value, if it isn't, we have an iterator to check neighbours.
	std::pair<std::set<Edge, setComp>::iterator,bool> retval;
  std::set<Edge, setComp>::key_compare mycomp = edgeS.key_comp();
  mycomp.o.isect = IS_FALSE;
  mycomp.o.l_idx = index;
	retval = edgeS.insert(e);

  // handle if the insert found an intersection.
  std::cerr << "setComp isect: ";
  printEnum(mycomp.o.isect);
  std::cerr << std::endl;

  if ((mycomp.o.isect == IS_TRUE) || (mycomp.o.isect == IS_3P_COLLINEAR)) {
    std::cerr << "intersection between " << mycomp.o.lhs << " and " << mycomp.o.rhs << std::endl;
    flip(mycomp.o.lhs, mycomp.o.rhs, polygon, points);
    index = mycomp.o.l_idx;
    decrementEdges(index, edgeS);
    valid = E_SKIP;
    mycomp.o.isect = IS_FALSE;
    return valid;
  } else if (mycomp.o.isect == IS_4P_COLLINEAR) {
    std::cerr << "collinearity before swap:" << mycomp.o.lhs << " and " << mycomp.o.rhs << std::endl;
    if (collSwap(mycomp.o.lhs, mycomp.o.rhs, polygon)) {
      std::cerr << "collinearity after swap: " << mycomp.o.lhs << " and " << mycomp.o.rhs << std::endl;
      //flip(mycomp.o.lhs, mycomp.o.rhs, polygon, points);
      //std::cerr << "collinearity after flip: " << mycomp.o.lhs << " and " << mycomp.o.rhs << std::endl;
      //index = mycomp.o.l_idx;
      if (mycomp.o.lhs.getLowerLexIdx() < mycomp.o.rhs.getLowerLexIdx()) {
        if (mycomp.o.lhs.getLowerLexIdx() < mycomp.o.l_idx) index = mycomp.o.lhs.getLowerLexIdx();
        else index = mycomp.o.l_idx;
      } else {
        if (mycomp.o.rhs.getLowerLexIdx() < mycomp.o.l_idx) index = mycomp.o.rhs.getLowerLexIdx();
        else index = mycomp.o.l_idx;
      }
      decrementEdges(index, edgeS);
      valid = E_SKIP;
      mycomp.o.isect = IS_FALSE;
    } else {
      std::cerr << "false alarm." << std::endl;
    }
    return valid;
  } else if (mycomp.o.isect == IS_VERTEX22) {
    // need to check if edges are 3 point collinear and swap if necessary.
    double val3 = det(mycomp.o.lhs, *mycomp.o.rhs.p1);
    if (val3 == 0) {
      std::cerr << "3 point collinearity found at: " << mycomp.o.lhs << " and " << mycomp.o.rhs << std::endl;
      if (*mycomp.o.lhs.p1 < *mycomp.o.rhs.p1) collSwap(mycomp.o.lhs.p1, mycomp.o.lhs.p2, mycomp.o.rhs.p1, polygon);
      else  collSwap(mycomp.o.rhs.p1, mycomp.o.rhs.p2, mycomp.o.lhs.p1, polygon);
      std::cerr << "after swap: lhs: " << mycomp.o.lhs << ", rhs: " << mycomp.o.rhs << std::endl;
      index = mycomp.o.l_idx;
      decrementEdges(index, edgeS);
      valid = E_SKIP;
      mycomp.o.isect = IS_FALSE;
      return valid;
    }
  }

	std::cout << "retval.first : " << *retval.first << std::endl;
	std::cout << "retval.second: " << retval.second << std::endl;

  // I need to know neighbours for both a successful insert as well as unsuccessful
  Edge before, after;

  std::cout << ((retval.first != edgeS.begin()) ? "'e' is NOT the first edge" : "'e' is the first edge" ) << std::endl;
  if (retval.first != edgeS.begin()) {
    before = *(std::prev(retval.first));
    std::cout << "before: " << before << std::endl;
    bef = true;
  }
  std::cout << ( (retval.first != --edgeS.end()) ? "'e' is NOT the last edge" : "'e' is the last edge" ) << std::endl;
  if (retval.first != --edgeS.end()) {
    after  = *(std::next(retval.first));
    std::cout << "after : " << after << std::endl;
    af = true;
  }

  if (mycomp.o.isect == IS_SAME_EDGE) {
    // 'e' already existed and needs to be removed from 'edgeS'
		std::cout << "'e' found in 'edgeS'.  Need to remove, then (possibly) check neighbours after removal." << std::endl;
    if (e == *retval.first) {
    	std::cerr << "'e' same as r.1" << std::endl;
      edgeS.erase(retval.first);
      if (bef && af) {
        cross = checkIntersection(before, after);
        if (cross < IS_TRUE) {
          std::cout << "no intersection between 'before' and 'after'" << std::endl;
        }
        else {
          std::cout << "intersection between 'before' and 'after'" << std::endl;
          flip(before, after, polygon, points);
          if (before.l_idx < after.l_idx) {
            if (before.l_idx < mycomp.o.l_idx)  index = before.l_idx;
            else index = mycomp.o.l_idx;
          }
          else {
            if (after.l_idx < mycomp.o.l_idx) index = after.l_idx;
            else index = mycomp.o.l_idx;
          }
					decrementEdges(index, edgeS);
					valid = E_SKIP;
        }
      }
      else std::cerr << "removal ok, continue." << std::endl;
    }
    else {
      std::cerr << "retval.first WAS NOT 'e'!!!" << std::endl;
      std::cerr << "o.lhs: " << mycomp.o.lhs << ", o.rhs: " << mycomp.o.rhs << std::endl;
      valid = E_NOT_VALID;
      return valid;
    }
  }

	if (retval.second) {
		// 'e' was successfully inserted into 'edgeS' as a new element
		// check if it intersects with its neighbours

		if (bef) {
			cross = checkIntersection(e, before);
      std::cerr << "cross: " << cross << std::endl;
			if (cross < IS_TRUE) {
				std::cout << "no intersection between 'e' and 'before'" << std::endl;
				valid = E_VALID;
			}
			else {
				// edge intersects with 'before'.  Need to flip 'e' and 'before' in polygon, then remove edges in 'edgeS'
				std::cout << "intersection with 'before'" << std::endl;
				flip(e, before, polygon, points);
        index = mycomp.o.l_idx;
				decrementEdges(index, edgeS);
				valid = E_SKIP;
			}
		}
		if (valid != E_SKIP) {
			if(af) {
				cross  = checkIntersection(e, after);
				if (af && (cross < IS_TRUE)) {
					// edge inserted, and does not intersect its neighbours.
					std::cout << "no intersection between 'e' and 'after'" << std::endl;
					valid = E_VALID;
				}
				else {
					// edge intersects with 'after'.  Need to flip 'e' and 'after' in polygon, then remove edges in 'edgeS'
					std::cout << "intersection with 'after'" << std::endl;
					flip(e, after, polygon, points);
          index = mycomp.o.l_idx;
					decrementEdges(index, edgeS);
					valid = E_SKIP;
				}
			}
		}
	} else if ((mycomp.o.isect != IS_SAME_EDGE) && !retval.second) {
    std::cerr << "ERROR: Insert failed!!!" << std::endl;
  }
	return valid;
}

enum error opt2(std::vector<unsigned int>& polygon, std::vector<Point>& points) {
	// initialise and create a random permutation for the polygon
	createRandPol(polygon, points);

	// the point set 'points' now has x/y coordinates as well as
	// original input index of points in 'i' and polygon index in 'v'
	// Now it can be sorted lexicographically
	std::vector<unsigned int> lex (points.size());
	for(unsigned int i = 0; i < points.size(); ++i)
		lex[i] = points[i].i;

	// lex contains a vector of 'points' indexes sorted lexicographically
	std::sort(lex.begin(), lex.end(), lexComp(points));
  unsigned int counter = 0;
  for (std::vector<unsigned int>::iterator it = lex.begin(); it != lex.end(); ++it) {
    points[(*it)].l = counter;
    ++counter;
  }

	// Given a lexicographical sort, we can go through the vector, check for intersections and untangle them
	unsigned int index=0, before, after;
	//double d_idx;
  compObject comp;
	enum edge_t val1, val2;
  double val3;
	Point *p1, *p2, *p3;
	Edge e1, e2;
  bool reset;
	//std::list<Edge> edgesL; // a list for edges
	std::set<Edge, setComp> edgeS(comp); // a set of edges.

  reset = false;
	while (index < points.size()) {
		std::cout << std::endl << "index: " << index << std::endl;
		val1 = E_VALID; val2 = E_VALID;
		// get the current point at 'index'
		p1 = &points[lex[index]];
		comp.t = (*p1).x; // the x index used as the comparison.

		// get the 2 points it is connected to in 'polygon', treating the edge case when the point 'p1' is on the ends
		before = ((*p1).v + points.size() -1) % points.size();
		after =  ((*p1).v + points.size() +1) % points.size();

		p2 = &points[polygon[before]];
		p3 = &points[polygon[after]];

    if (reset && (p1 < p2) && (p1 < p3)) {
      reset = false;
		  // construct the edges
      //std::cerr << *p2 << " < " << *p3 << " : " << ((*p2 < *p3) ? "true" : "false") << std::endl;
		  if (*p2 < *p3) {
			  e1 = Edge (p1, p2, index);
			  e2 = Edge (p1, p3, index);
        val3 = det(e1, *p3);
		  }
		  else {
			  e1 = Edge (p1, p3, index);
			  e2 = Edge (p1, p2, index);
        val3 = det(e1, *p2);
		  }

      if (val3 == 0) {
        // the 2 edges are collinear
        std::cerr << "collinear check found a possible match."  << std::endl;
        if ((*p1 < *p2) && (*p1 < *p3)) {
          std::cerr << "before swap: e1: " << e1 << ", e2: " << e2 << std::endl;
          if (collSwap(p1, p2, p3, polygon)) {
            std::cerr << "after  swap: e1: " << e1 << ", e2: " << e2 << std::endl;
            if ((*p1).l < (*p2).l) {
              if ((*p1).l < (*p3).l) index = (*p1).l;
              else index = (*p3).l;
            } else {
              if ((*p2).l < (*p3).l) index = (*p2).l;
              else index = (*p3).l;
            }
            decrementEdges(index, edgeS);
            reset = true;
            continue;
          }
        } else std::cerr << "false alarm." << std::endl;
      }

		  std::cout << "processing e1: " << e1 << std::endl;
		  val1 = processEdge(index, e1, edgeS, polygon, points);
		  if (val1 == E_SKIP) {
        reset = true;
        continue; // swapping invalidates 'e2' so start again from the lower index before processing 'e2'
      }

		  std::cout << "processing e2: " << e2 << std::endl;
		  val2 = processEdge(index, e2, edgeS, polygon, points);
		  //std::cout << "after edgecheck2." << std::endl;
		  if (val2 == E_SKIP) {
        reset = true;
        continue;
      }
    } else if (reset && (p1 < p2)) {
      reset = false;
      e1 = Edge (p1, p2, index);
      std::cout << "processing e1: " << e1 << std::endl;
		  val1 = processEdge(index, e1, edgeS, polygon, points);
		  if (val1 == E_SKIP) {
        reset = true;
        continue;
      }
    } else if (reset && (p1 < p3) ) {
      reset = false;
      e1 = Edge (p1, p3, index);
      std::cout << "processing e1: " << e1 << std::endl;
		  val1 = processEdge(index, e1, edgeS, polygon, points);
		  if (val1 == E_SKIP) {
        reset = true;
        continue;
      }
    }
    else {
      reset = false;
      // construct the edges
      //std::cerr << *p2 << " < " << *p3 << " : " << ((*p2 < *p3) ? "true" : "false") << std::endl;
		  if (*p2 < *p3) {
			  e1 = Edge (p1, p2, index);
			  e2 = Edge (p1, p3, index);
        val3 = det(e1, *p3);
		  }
		  else {
			  e1 = Edge (p1, p3, index);
			  e2 = Edge (p1, p2, index);
        val3 = det(e1, *p2);
		  }

      if (val3 == 0) {
        // the 2 edges are collinear
        std::cerr << "collinear check found a possible match."  << std::endl;
        if ((*p1 < *p2) && (*p1 < *p3)) {
          std::cerr << "before swap: e1: " << e1 << ", e2: " << e2 << std::endl;
          if (collSwap(p1, p2, p3, polygon)) {
            std::cerr << "after  swap: e1: " << e1 << ", e2: " << e2 << std::endl;
            if ((*p1).l < (*p2).l) {
              if ((*p1).l < (*p3).l) index = (*p1).l;
              else index = (*p3).l;
            } else {
              if ((*p2).l < (*p3).l) index = (*p2).l;
              else index = (*p3).l;
            }
            decrementEdges(index, edgeS);
            reset = true;
            continue;
          }
        } else std::cerr << "false alarm." << std::endl;
      }

		  std::cout << "processing e1: " << e1 << std::endl;
		  val1 = processEdge(index, e1, edgeS, polygon, points);
		  if (val1 == E_SKIP) {
        reset = true;
        continue; // swapping invalidates 'e2' so start again from the lower index before processing 'e2'
      }

		  std::cout << "processing e2: " << e2 << std::endl;
		  val2 = processEdge(index, e2, edgeS, polygon, points);
		  //std::cout << "after edgecheck2." << std::endl;
		  if (val2 == E_SKIP) {
        reset = true;
        continue;
      }
    }

    //std::cout << "edges in 'edges':" << std::endl;
    //for (std::list<Edge>::iterator it=edges.begin(); it!=edges.end(); ++it) std::cout << *it << std::endl;
		index++;
    reset = false;
	}

	return SUCCESS;
}

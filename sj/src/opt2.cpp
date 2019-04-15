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
/*
  bool retval = false;
  double dp = reldist(*a, *b, *c);
  if (dp > 1) {
    if ((*a).v > (*b).v) {polSwap(a, b, polygon); retval = true;}
    if ((*a).v > (*c).v) {polSwap(a, c, polygon); retval = true;}
    if ((*b).v > (*c).v) {polSwap(b, c, polygon); retval = true;}
    return retval;
  }
  dp = reldist(*a, *c, *b);
  if (dp > 1) {
    if ((*a).v > (*b).v) {polSwap(a, b, polygon); retval = true;}
    if ((*a).v > (*c).v) {polSwap(a, c, polygon); retval = true;}
    if ((*c).v > (*b).v) {polSwap(b, c, polygon); retval = true;}
    return retval;
  }
  return retval;
}
*/


/*
  if ((*a) < (*b))  {
    int left, right; // how far away b is from a in either direction
    if (((*a).v - (*b).v) < 0) {
      left = ((*a).v + polygon.size() - (*b).v) % polygon.size();
      right = (*b).v - (*a).v)
    } else {
      left = polygon.size() - (*a).v;
      right = (*a).v - (*b).v;
    }

    if (left < right) {
      polSwap(a, b, polygon);
      retval = true;
    }
  } else {
    if ((*a).v < (*b).v) {
      polSwap(a, b, polygon);
      retval = true;
    }
  }
  if ((*a) < (*c)) {
    if ((*a).v > (*c).v) {
      polSwap(a, c, polygon);
      retval = true;
    }
  } else {
    if ((*a).v < (*c).v) {
      polSwap(a, c, polygon);
      retval = true;
    }
  }
  if ((*b) < (*c)) {
    if ((*b).v > (*c).v) {
      polSwap(b, c, polygon);
      retval = true;
    }
  } else {
    if ((*b).v < (*c).v) {
      polSwap(b, c, polygon);
      retval = true;
    }
  }
  return retval;
  //if (*b < *c) polSwap(a, b, polygon);
  //else polSwap(a, c, polygon);
}
*/


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
/*
  if ((dp1 > 1) && (dp2 > 1)) return false;
  if ((dp1 < 0) && (dp2 < 0)) return false;
  if ((dp1 < 0) && (dp2 > 1)) {polSwap(e1.p1, e2.p2, polygon); polSwap(e1.p1, e1.p2, polygon); return true;}
  if ((dp1 > 1) && (dp2 < 0)) {polSwap(e1.p2, e2.p2, polygon); return true;}
  if ((dp1 > 0) && (dp1 < 1) && dp2 > 1) {polSwap(e1.p2, e2.p1, polygon); return true;}
  if ((dp1 > 0) && (dp1 < 1) && dp2 < 0) {polSwap(e1.p1, e2.p1, polygon); return true;}
  if ((dp2 > 0) && (dp2 < 1) && dp1 > 1) {polSwap(e1.p2, e2.p2, polygon); return true;}
  if ((dp2 > 0) && (dp2 < 1) && dp1 < 0) {polSwap(e1.p1, e2.p2, polygon); return true;}
  if ((dp1 > 0) && (dp1 < 1) && (dp2 > 0) && (dp2 < 1)) {polSwap(e1.p2, e2.p1, polygon);return true;}

  std::cerr << "2 EDGE collSwap ERROR, all conditions should have been caught earlier." << std::endl;
  std::cerr << "e1: " << e1 << ", e2: " << e2 << std::endl;
  return false;
}
*/

/*
{
  if (  ((*e1.p1) > (*e1.p2)) ||
        ((*e1.p1) > (*e2.p1)) ||
        ((*e1.p1) > (*e2.p2)) ||
        ((*e1.p2) > (*e2.p1)) ||
        ((*e1.p2) > (*e2.p2)) ||
        ((*e2.p1) > (*e2.p2))   )
  {
    collSwap(e1.p2, e2.p1, e2.p2, polygon);
    collSwap(e1.p1, e1.p2, e2.p1, polygon);
    return true;
  } else return false;
*/
/*
  if (e1.checkPolLoHi()) {
    if (e2.checkPolLoHi()) return false;
    else {
      polSwap(e2, polygon);
      return true;
    }
  } else {
    if (e2.checkPolLoHi()) {
      polSwap(e2, polygon);
      return true;
    } else return false;
  }
}
*/


/*
// function to remove edges from 'edges' up to and including value of 'index'
void decrementEdges1(unsigned int index, std::list<Edge>& edges) {
	// need to remove all edges in the set from the end up to and including new index.
	unsigned int check = (*edges.rbegin()).l_idx;
	//std::cout << "check: " << check << std::endl;
	while ((edges.size() > 0) && (check >= index)) {
		edges.pop_back();
		check = (*edges.rbegin()).l_idx;
		//std::cout << "check: " << check << std::endl;
	}
}
*/
/*
bool lexSwap(Point* p1, Point* p2, std::vector<unsigned int>& polygon) {
  bool swapped = false;
  unsigned int temp;
  if ( abs(int((*p1).v - (*p2).v)) == 1 ) {
    if ((*p1).v > (*p2).v) {
      std::cerr << "p1.v: " << (*p1).v << " > p2.v: " << (*p2).v << std::endl;
      temp = (*p1).v;
      (*p1).v = (*p2).v;
      (*p2).v = temp;
      polygon[(*p1).v] = (*p1).i;
      polygon[(*p2).v] = (*p2).i;
      swapped = true;
    }
  } else if ((*p1).v == 0) {
    if (abs(int(polygon.size()-(*p2).v)) == 1 ) {
      if ((*p1).v > (*p2).v) {
        std::cerr << "p1.v: " << (*p1).v << " > p2.v: " << (*p2).v << std::endl;
        temp = (*p1).v;
        (*p1).v = (*p2).v;
        (*p2).v = temp;
        polygon[(*p1).v] = (*p1).i;
        polygon[(*p2).v] = (*p2).i;
        swapped = true;
      }
    }
  } else if ((*p2).v == 0) {
    if (abs(int(polygon.size()-(*p1).v)) == 1 ) {
      if ((*p1).v > (*p2).v) {
        std::cerr << "p1.v: " << (*p1).v << " > p2.v: " << (*p2).v << std::endl;
        temp = (*p1).v;
        (*p1).v = (*p2).v;
        (*p2).v = temp;
        polygon[(*p1).v] = (*p1).i;
        polygon[(*p2).v] = (*p2).i;
        swapped = true;
      }
    }
  }
  return swapped;
}

// p1, p2 and p3 are collinear, which won't work for the purpose of 2opt without reordering them in the polygon
bool lexReorder(Point* p1, Point* p2, Point* p3, std::vector<unsigned int>& polygon) {
  bool swapped = false;
  unsigned int temp;
  if (*p1 < *p2) {
    if ((*p1).v > (*p2).v) {
      std::cerr << "p1.v: " << (*p1).v << " > p2.v: " << (*p2).v << std::endl;
      temp = (*p1).v;
      (*p1).v = (*p2).v;
      (*p2).v = temp;
      polygon[(*p1).v] = (*p1).i;
      polygon[(*p2).v] = (*p2).i;
      swapped = true;
    }
  } else {
    if ((*p1).v < (*p2).v) {
      std::cerr << "p1.v: " << (*p1).v << " < p2.v: " << (*p2).v << std::endl;
      temp = (*p1).v;
      (*p1).v = (*p2).v;
      (*p2).v = temp;
      polygon[(*p1).v] = (*p1).i;
      polygon[(*p2).v] = (*p2).i;
      swapped = true;
    }
  }
  if (*p1 < *p3) {
    if ((*p1).v > (*p3).v) {
      std::cerr << "p1.v: " << (*p1).v << " > p3.v: " << (*p3).v << std::endl;
      temp = (*p1).v;
      (*p1).v = (*p3).v;
      (*p3).v = temp;
      polygon[(*p1).v] = (*p1).i;
      polygon[(*p3).v] = (*p3).i;
      swapped = true;
    }
  } else {
    if ((*p1).v < (*p3).v) {
      std::cerr << "p1.v: " << (*p1).v << " < p3.v: " << (*p3).v << std::endl;
      temp = (*p1).v;
      (*p1).v = (*p3).v;
      (*p3).v = temp;
      polygon[(*p1).v] = (*p1).i;
      polygon[(*p3).v] = (*p3).i;
      swapped = true;
    }
  }
  if (*p2 > *p3) {
    if ((*p2).v < (*p3).v) {
      std::cerr << "p2.v: " << (*p2).v << " < p3.v: " << (*p3).v << std::endl;
      temp = (*p1).v;
      (*p1).v = (*p3).v;
      (*p3).v = temp;
      polygon[(*p2).v] = (*p2).i;
      polygon[(*p3).v] = (*p3).i;
      swapped = true;
    }
  } else {
    if ((*p2).v > (*p3).v) {
      std::cerr << "p2.v: " << (*p2).v << " > p3.v: " << (*p3).v << std::endl;
      temp = (*p2).v;
      (*p2).v = (*p3).v;
      (*p3).v = temp;
      polygon[(*p2).v] = (*p2).i;
      polygon[(*p3).v] = (*p3).i;
      swapped = true;
    }
  }
  std::cerr << "reorder: p1: " << *p1 << ", p2: " << *p2 << ", p3: " << *p3 << std::endl;
  return swapped;
}

bool lexReorder(Edge& e1, Edge& e2, std::vector<unsigned int>& polygon) {
  bool swapped = false;
  // if one vertex is the same
  if (*e1.p1 == *e2.p1) {
    swapped = lexReorder(e1.p1, e1.p2, e2.p2, polygon);
    return swapped;
  } else if (*e1.p1 == *e2.p2) {
    swapped = lexReorder(e1.p1, e1.p2, e2.p1, polygon);
    return swapped;
  }
  else if (*e1.p2 == *e2.p1) {
    swapped = lexReorder(e1.p2, e1.p1, e2.p2, polygon);
    return swapped;
  }
  else if (*e1.p2 == *e2.p2) {
    swapped = lexReorder(e1.p2, e1.p1, e2.p1, polygon);
    return swapped;
  }

  // check if it's really 4 point collinearity
  double det_a = det(e1, *e2.p1);
  double det_b = det(e1, *e2.p2);
  double det_c = det(e2, *e1.p1);
  double det_d = det(e2, *e1.p2);

  if (abs(det_a)+abs(det_b)+abs(det_c)+abs(det_d) > 0) {
    // only 3 point collinearity
         if (det_a == 0) swapped = lexReorder(e1.p1, e1.p2, e2.p1, polygon);
    else if (det_b == 0) swapped = lexReorder(e1.p1, e1.p2, e2.p2, polygon);
    else if (det_c == 0) swapped = lexReorder(e2.p1, e2.p2, e1.p1, polygon);
    else if (det_d == 0) swapped = lexReorder(e2.p1, e2.p2, e1.p2, polygon);
    return swapped;
  }

  unsigned int temp;
  // sort the e1.p1...
  if (*e1.p1 < *e1.p2) {
    if ((*e1.p1).v > (*e1.p2).v) {
      temp = (*e1.p1).v;
      (*e1.p1).v = (*e1.p2).v;
      (*e1.p2).v = temp;
      polygon[(*e1.p1).v] = (*e1.p1).i;
      polygon[(*e1.p2).v] = (*e1.p2).i;
      swapped = true;
    }
  } else {
    if ((*e1.p1).v < (*e1.p2).v) {
      temp = (*e1.p1).v;
      (*e1.p1).v = (*e1.p2).v;
      (*e1.p2).v = temp;
      polygon[(*e1.p1).v] = (*e1.p1).i;
      polygon[(*e1.p2).v] = (*e1.p2).i;
      swapped = true;
    }
  }
  if (*e1.p1 < *e2.p1) {
    if ((*e1.p1).v > (*e2.p1).v) {
      temp = (*e1.p1).v;
      (*e1.p1).v = (*e2.p1).v;
      (*e2.p1).v = temp;
      polygon[(*e1.p1).v] = (*e1.p1).i;
      polygon[(*e2.p1).v] = (*e2.p1).i;
      swapped = true;
    }
  } else {
    if ((*e1.p1).v < (*e2.p1).v) {
      temp = (*e1.p1).v;
      (*e1.p1).v = (*e2.p1).v;
      (*e2.p1).v = temp;
      polygon[(*e1.p1).v] = (*e1.p1).i;
      polygon[(*e2.p1).v] = (*e2.p1).i;
      swapped = true;
    }
  }
  if (*e1.p1 < *e2.p2) {
    if ((*e1.p1).v > (*e2.p2).v) {
      temp = (*e1.p1).v;
      (*e1.p1).v = (*e2.p2).v;
      (*e2.p2).v = temp;
      polygon[(*e1.p1).v] = (*e1.p1).i;
      polygon[(*e2.p2).v] = (*e2.p2).i;
      swapped = true;
    }
  } else {
    if ((*e1.p1).v < (*e2.p2).v) {
      temp = (*e1.p1).v;
      (*e1.p1).v = (*e2.p2).v;
      (*e2.p2).v = temp;
      polygon[(*e1.p1).v] = (*e1.p1).i;
      polygon[(*e2.p2).v] = (*e2.p2).i;
      swapped = true;
    }
  }
  // ...then the rest can be sorted with the other lexReorder
  swapped = lexReorder(e1.p2, e2.p1, e2.p2, polygon);
  return swapped;
}
*/
/*
// function that erases points in 'moveP' from the polygon and adds them to the end in order of 'moveP'.
void movePoints(std::vector<unsigned int>& moveP, std::vector<unsigned int>& polygon, std::vector<Point>& points) {

	std::vector<unsigned int> newpnts;
	std::vector<unsigned int> newP = moveP;
	std::sort(newP.begin(), newP.end());

	for (unsigned int i = 0; i < moveP.size(); ++i) {
		newpnts.push_back(polygon[moveP[i]]);
		std::cout << "newP[" << i << "]: " << newP[i] << " : " << points[polygon[newP[i]]] << std::endl;
	}

	// erase points inside 'polygon' vector in reverse order.
	for (int i = newP.size()-1; i >= 0;--i) {
		std::vector<unsigned int>::iterator it = polygon.begin()+newP[i];
		std::cout << "erasing: " << *it << ", point:" << points[*it] << std::endl;
		polygon.erase(it);					// erase the point at its original index.
	}
	std::cout << "size of pol: " << polygon.size() << std::endl;

	//pick a random index out of the 4 to insert the points.
	RandomGenerator rg;
	int j;
	do {
		j = rg.getRandomIndex(moveP.size()-1);
		std::cout << "j: " << j << ", index: " << moveP[j] << std::endl;
	} while (moveP[j] > points.size()-4);

	std::vector<unsigned int>::iterator it = polygon.begin()+moveP[j];
	std::cout << "points index at pol[j]: " << *it << ", point: " << points[*it] << std::endl;

	it = polygon.insert(it, newpnts.begin(), newpnts.end());
	std::cout << "first points index: " << *it << ", point: " << points[polygon[*it]] << std::endl;
	std::cout << "size of pol: " << polygon.size() << std::endl;

	for (unsigned int i = newP[0]; i < polygon.size();++i) {
		points[polygon[i]].v = i;
	}

	std::cout << "points in polygon at iterator:" << std::endl;
	for (unsigned int i = 0; i < moveP.size()+1; ++i) {
		std::vector<unsigned int>::iterator it = polygon.begin()+moveP[j]+i;
		std::cout << "it: " << moveP[j]+i << ", point: " << points[*it] << std::endl;
	}
}
*/

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
      std::cerr << "3 point collinearity found at: " << mycomp.o.lhs << " and " << mycomp.o.rhs;
      if (*mycomp.o.lhs.p1 < *mycomp.o.rhs.p1) collSwap(mycomp.o.lhs.p1, mycomp.o.lhs.p2, mycomp.o.rhs.p1, polygon);
      else  collSwap(mycomp.o.rhs.p1, mycomp.o.rhs.p2, mycomp.o.lhs.p1, polygon);
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
	}
	return valid;
}

/*
// old version that checks all edges in 'edges' list to the edge 'e'
enum edge_t edgeCheck1(unsigned int& index, Edge& e, std::list<Edge>& edges, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
	enum edge_t valid = E_VALID;
	enum intersect_t cross = IS_FALSE;

	// check if 'e1' intersects other edges already in 'edges'
	for (std::list<Edge>::iterator it=edges.begin(); it!=edges.end(); ++it) {
		// if either edge is already in the 'edges' list: remove it and do not process the edge.
		//std::cout << "*it: " << *it << std::endl;
		if (e == *it) {
			//std::cout << "e1 found in edges." << std::endl;
			it = edges.erase(it);
			valid = E_NOT_VALID;
			break;
		}

		cross = checkIntersection(e, *it);
		if((valid == E_VALID) && (cross == IS_TRUE)) {
			//std::cout << "found an intersection for e1 and *it." << (*it) << std::endl;
			//std::cout << "polygon before flip: ";
			//poldisplay(polygon);
			flip(e, *it, polygon, points);
			//std::cout << "polygon after flip: ";
			//poldisplay(polygon);
			// after flipping, the index needs to be reset to the lower of 'e1' and '*it'
			//std::cout << "old index: " << index << std::endl;
			index = (e.l_idx < (*it).l_idx) ? e.l_idx : (*it).l_idx;
			//std::cout << "new index: " << index << std::endl;
			decrementEdges(index, edges);
			valid = E_SKIP;
			break;
		}
		else if ((valid == E_VALID) && (cross == IS_COLLINEAR)) {
			//std::cout << "Inside collinear part" << std::endl;
			// need to sort the points into lexicographical order relative to themselves
			// check relative distances between points in e and *it

			// first create a vector of a pair of points and and double
			std::vector< std::pair<Point*, double> > sortP;
			std::vector<unsigned int> sortPol;

			sortP.push_back( std::make_pair(e.p1, 0.0) );
			sortP.push_back( std::make_pair(e.p2, 1.0) );
			sortP.push_back( std::make_pair((*it).p1, reldist(e, *(*it).p1)) );
			sortP.push_back( std::make_pair((*it).p2, reldist(e, *(*it).p2)) );

			// sort it by the secondary double value
			std::sort(sortP.begin(), sortP.end(), sortbysec);
			for (unsigned int i = 0; i < sortP.size(); ++i) {
				sortPol.push_back((*sortP[i].first).v);
				//std::cout << "sortP[" << i << "]: " << points[polygon[sortPol[i]]] << std::endl;
			}

			// move the points in 'moveP' to end of 'polygon' in the right order
			movePoints(sortPol, polygon, points);

			// index needs to be reset to lowest l_idx of 'e' or '*it'
			//std::cout << "old index: " << index << std::endl;
			index = (e.l_idx < (*it).l_idx) ? e.l_idx : (*it).l_idx;
			//std::cout << "new index: " << index << std::endl;
			decrementEdges(index, edges);
			//std::cout << "Decrement successful." << std::endl;
			valid = E_SKIP;
			break;
		}
	}
	return valid;
}
*/

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
	//std::list<Edge> edgesL; // a list for edges
	std::set<Edge, setComp> edgeS(comp); // a set of edges.

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
          continue;
        }
      } else std::cerr << "false alarm." << std::endl;
    }

		std::cout << "processing e1: " << e1 << std::endl;
		val1 = processEdge(index, e1, edgeS, polygon, points);
		if (val1 == E_SKIP) continue; // swapping invalidates 'e2' so start again from the lower index before processing 'e2'

		std::cout << "processing e2: " << e2 << std::endl;
		val2 = processEdge(index, e2, edgeS, polygon, points);
		//std::cout << "after edgecheck2." << std::endl;
		if (val2 == E_SKIP) continue;

    //std::cout << "edges in 'edges':" << std::endl;
    //for (std::list<Edge>::iterator it=edges.begin(); it!=edges.end(); ++it) std::cout << *it << std::endl;

		index++;
	}

	return SUCCESS;
}

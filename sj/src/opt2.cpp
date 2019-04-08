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

	retval = edgeS.insert(e);
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
				index = 0;
				//index = (e.l_idx < before.l_idx) ? e.l_idx : after.l_idx;
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
					index = 0;
					//index = (e.l_idx < after.l_idx) ? e.l_idx : after.l_idx;
					decrementEdges(index, edgeS);
					valid = E_SKIP;
				}
			}
		}
	}
	else {
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
          flip(e, after, polygon, points);
					index = 0;
					//index = (e.l_idx < after.l_idx) ? e.l_idx : after.l_idx;
					decrementEdges(index, edgeS);
					valid = E_SKIP;
        }
      }
      else std::cerr << "removal ok, continue." << std::endl;
    }
    else {
      std::cerr << "retval.first was not 'e'!!!" << std::endl;
      valid = E_NOT_VALID;
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
	//std::cout << "lexicographical order:" << std::endl;
	//poldisplay(lex);

	// Given a lexicographical sort, we can go through the vector, check for intersections and untangle them
	unsigned int index=0, before, after;
	double d_idx;
	enum edge_t val1, val2;
  enum intersect_t val3;
	Point *p1, *p2, *p3;
	Edge e1, e2;
	//std::list<Edge> edgesL; // a list for edges
	std::set<Edge, setComp> edgeS(d_idx); // a set of edges.

	while (index < points.size()) {
		std::cout << "index: " << index << std::endl;
		val1 = E_VALID; val2 = E_VALID;
		// get the current point at 'index'
		p1 = &points[lex[index]];
		d_idx = (*p1).x;
		// get the 2 points it is connected to in 'polygon', treating the edge case when the point 'p1' is on the ends
		before = ((*p1).v + points.size() -1) % points.size();
		after =  ((*p1).v + points.size() +1) % points.size();

		p2 = &points[polygon[before]];
		p3 = &points[polygon[after]];

		// construct the edges
		if (p2 < p3) {
			e1 = Edge (p1, p2, index);
			e2 = Edge (p1, p3, index);
		}
		else {
			e1 = Edge (p1, p3, index);
			e2 = Edge (p1, p2, index);
		}

    val3 = checkIntersection(e1, e2);
    if (!(val3 < IS_TRUE)) {
      // the 2 edges are collinear
      flip(e1, e2, polygon, points);
      index = 0;
      decrementEdges(index, edgeS);
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

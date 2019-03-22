#include <iostream> // for endl
#include <vector>
#include <list>
#include <utility> // for std::pair
#include <stdlib.h>  // for abs
#include <algorithm>    // std::sort
#include "basicDefinitions.h"
#include "point.h"
#include "edge.h"
#include "randomGenerator.h"

// comparison function for sorting pairs by secondary value
bool sortbysec(const std::pair<Point*,double> &a, const std::pair<Point*,double> &b) { 
    return (a.second > b.second); 
}

bool sortbyint(const int i, const int j) { return (i<j); }

// function to remove edges from 'edges' up to and including value of 'index'
void decrementEdges(unsigned int index, std::list<Edge>& edges) {
	// need to remove all edges in the set from the end up to and including new index.
	unsigned int check = (*edges.rbegin()).l_idx;
	//std::cout << "check: " << check << std::endl;
	while ((edges.size() > 0) && (check >= index)) {
		edges.pop_back();
		check = (*edges.rbegin()).l_idx;
		//std::cout << "check: " << check << std::endl;
	}
}

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
	} while (moveP[j] > 396);

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

enum edge_t edgeCheck(unsigned int& index, Edge& e, std::list<Edge>& edges, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
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

enum error opt2(std::vector<unsigned int>& polygon, std::vector<Point>& points) {
	// initialise and create a random permutation for the polygon
	createRandPol(polygon, points);

	// the point set 'points' now has x/y coordinates as well as 
	// original input index of points in 'i' and polygon index in 'v'
	// Now it can be sorted lexicographically
	std::vector<unsigned int> lex (points.size());
	for(unsigned int i = 0; i < points.size(); ++i)
		lex[i] = points[i].i;

	lexSort(lex, points);
	//std::cout << "lexicographical order:" << std::endl;
	//poldisplay(lex);

	// Given a lexicographical sort, we can go through the vector, check for intersections and untangle them
	unsigned int index=0, before, after;
	enum edge_t val1, val2;
	Point *p1, *p2, *p3;
	Edge e1, e2;
	std::list<Edge> edges; // a list for edges that need to be checked against the current edge being validated

	while (index < points.size()) {
		std::cout << "index: " << index << std::endl;
		val1 = E_VALID; val2 = E_VALID;
		// get the current point at 'index'
		p1 = &points[lex[index]];
		// get the 2 points it is connected to in 'polygon', treating the edge case when the point 'p1' is on the ends
		before = ((*p1).v + points.size() -1) % points.size();
		after =  ((*p1).v + points.size() +1) % points.size();

		p2 = &points[polygon[before]];
		p3 = &points[polygon[after]];

		// construct the edges
		e1 = Edge (p1, p2, index);
		e2 = Edge (p1, p3, index);
		//std::cout << "e1:" << e1 << std::endl;
		//std::cout << "e2:" << e2 << std::endl;

		val1 = edgeCheck(index, e1, edges, polygon, points);
		//std::cout << "after edgecheck1." << std::endl;
		if (val1 == E_SKIP) continue; // swapping invalidates 'e2' so start again from the lower index before processing 'e2'
		val2 = edgeCheck(index, e2, edges, polygon, points);
		//std::cout << "after edgecheck2." << std::endl;
		if (val2 == E_SKIP) continue;

		if (val1 == E_VALID) edges.emplace_back(e1);
		if (val2 == E_VALID) edges.emplace_back(e2);
    //std::cout << "edges in 'edges':" << std::endl;
    //for (std::list<Edge>::iterator it=edges.begin(); it!=edges.end(); ++it) std::cout << *it << std::endl;
		
		index++;
	}

	return SUCCESS;
}


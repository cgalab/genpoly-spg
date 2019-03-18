#include <iostream> // for endl
#include <vector>
#include <list>
#include "basicDefinitions.h"
#include "point.h"
#include "edge.h"

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
  bool loopSkip, adde1, adde2;
  Point *p1, *p2, *p3;
  Edge e1, e2;
  std::list<Edge> edges; // a list for edges that need to be checked against the current edge being validated

  while (index < points.size()) {
    
    //std::cout << "index: " << index << std::endl;
    loopSkip = false; adde1 = true; adde2 = true;
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

    // check if 'e1' intersects other edges already in 'edges'
    for (std::list<Edge>::iterator it=edges.begin(); it!=edges.end(); ++it) {
      // if either edge is already in the 'edges' list: remove it and do not process the edge.
      //std::cout << "*it: " << *it << std::endl;
      if (e1 == *it) {
        //std::cout << "e1 found in edges." << std::endl;
        it = edges.erase(it);
        adde1 = false;
        break;
      }

      if(adde1 && checkIntersection(e1, *it)) {
        //std::cout << "found an intersection for e1 and *it." << std::endl;
        flip(e1, *it, polygon, points);
        // after flipping, the index needs to be reset to the lower of 'e1' and '*it'
        //std::cout << "old index: " << index << std::endl;
        index = (e1.l_idx < (*it).l_idx) ? e1.l_idx : (*it).l_idx;
        //std::cout << "new index: " << index << std::endl;
        // need to remove all edges in the set from the end up to and including new index.
        unsigned int check = (*edges.rbegin()).l_idx;
        //std::cout << "check: " << check << std::endl;
        while ((edges.size() > 0) && (check >= index)) {
          edges.pop_back();
          check = (*edges.rbegin()).l_idx;
          //std::cout << "check: " << check << std::endl;
        }
        loopSkip = true;
        break;
      }
    }

    if (loopSkip) continue; // swapping invalidates 'e2' so start again from the lower index

    // check if 'e2' intersects other edges already in 'edges'
    for (std::list<Edge>::iterator it=edges.begin(); it!=edges.end(); ++it) {
      if (e2 == *it) {
        //std::cout << "e2 found in edges." << std::endl;
        it = edges.erase(it);
        adde2 = false;
        break;
      }

      if(adde2 && checkIntersection(e2, *it)) {
        //std::cout << "found an intersection for e2 and *it: " << std::endl;
        // edges intersect, need to flip a part of the polygon which means '.v' changes for points in 'points'
        flip(e2, *it, polygon, points);
        //std::cout << "flip done" << std::endl;
        // after flipping, the index needs to be reset to the lower of 'e1' and '*it'
        //std::cout << "old index: " << index << std::endl;
        index = (e2.l_idx < (*it).l_idx) ? e2.l_idx : (*it).l_idx;
        //std::cout << "new index: " << index << std::endl;
        unsigned int check = (*edges.rbegin()).l_idx;
        //std::cout << "check: " << check << std::endl;
        while ((edges.size() > 0) && (check >= index)) {
          edges.pop_back();
          check = (*edges.rbegin()).l_idx;
          //std::cout << "check: " << check << std::endl;
        }
        loopSkip = true;
        break;
      }
    }


    if (loopSkip) continue;

    if (adde1) edges.emplace_back(e1);
    if (adde2) edges.emplace_back(e2);
    //std::cout << "edges in 'edges':" << std::endl;
    //for (std::list<Edge>::iterator it=edges.begin(); it!=edges.end(); ++it) std::cout << *it << std::endl;

    index++;
  }

  return SUCCESS;
}


#ifndef __HOLE_H_
#define __HOLE_H_

//#include <vector>
#include "pol.h"

unsigned int generateHoles(std::vector<std::vector<unsigned int>>& sph, std::vector<Point>& points, unsigned int nr_holes);
enum error get_hole(Ends end, std::vector<std::vector<unsigned int>>& sph, std::vector<Point>& points);
enum error get_hole(unsigned int& selection, std::vector<std::vector<unsigned int>>& sph, std::vector<Point>& points);
bool candidate_compare(E_Edge e1, E_Edge e2);
enum error sweep_polygonal_chain(std::vector<E_Edge>& valid_candidates, std::vector<Point>& points, bool is_chain, bool l2r);
void remove_adjacent_candidate(E_Edge& e1, E_Edge& e2, std::set<E_Edge>::iterator& it1, std::set<E_Edge>::iterator& it2,
                               std::set<E_Edge>& ls_edges);
bool set_bin(E_Edge& e1, E_Edge& e2, std::set<E_Edge>::iterator& it1, std::set<E_Edge>::iterator& it2, std::set<E_Edge>& ls_edges);
void insert_update(E_Edge& e, std::set<E_Edge>::iterator& it, std::set<E_Edge>& ls_edges, bool dbl_ins, bool is_chain, bool l2r);
void removal_update(E_Edge& e, std::set<E_Edge>::iterator& it, std::vector<E_Edge>& valid_candidates,
                    std::set<E_Edge>& ls_edges, bool dbl_rem, bool l2r);
void update_edge_in_set(E_Edge& e, std::set<E_Edge>::iterator& it, std::set<E_Edge>& ls_edges);

#endif

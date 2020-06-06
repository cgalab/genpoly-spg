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
#include "hole.h"
#include "pol.h"
#include "rand.h"
#include "simple_pol_check.h"


// overarching function:
enum error generateHoles(std::vector<std::vector<unsigned int>>& sph, std::vector<Point>& points, unsigned int nr_holes) {
  // 1) polygon in sph[0] is the main polygon that should have holes
  // 2) any already generated holes should be numbered in sph[i], where i != 0
  // 3) call function to grab all the inner chains that can make holes and the number of points in them.

  std::vector<unsigned int> ch; // points on the convex hull.
  get_convex_hull(ch, sph[0], points);
  unsigned int total_inner_points = sph[0].size() - ch.size();
  std::vector<Ends> ends;
  get_inner_chains_to_ch(ends, ch, sph[0], points);
  for(unsigned int i = 0; i < ends.size();++i) {
    std::cerr << ends[i] << std::endl;
  }

  // 4) grab all the points in the already generated holes
  unsigned int total_hole_points = 0;
  for (unsigned int i = 1; i < sph.size(); ++i) {
    total_hole_points = total_hole_points + sph[i].size();
  }

  std::cerr << "Total inner points: " << total_inner_points << ", total hole points: " << total_hole_points << std::endl;

  return SUCCESS;
}




// 5) randomly pick a chain or hole based on the number of points.
// 6) call a function 'get_hole()' that uses the linesweep on the chain or hole
// - returns 'true' if it changed sph and added a hole, false if it didn't.
// - returns 'false' if no candidate was found that was valid, which invalidates splitting the chain/hole, so it can be discarded.
// - 2 different functions whether it's a chain or polygon
// 7) repeat the random pick until number of holes desired has been fulfilled.

// get_hole() function
// - needs to know whether it's a chain or a closed chain so it can properly deal with the fictional c.h. edge of a chain.
// - needs the polygon, or the polygon and the points that define the inner chain, i.e. 2 functions.
// 1) set up a linesweep loop from lowest to highest.
// 2) deal with each event, double insert, continue, double remove.
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

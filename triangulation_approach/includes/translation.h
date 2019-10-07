/*
	Include standard libraries
*/
#include <list>
#include <stack>
#include <cmath>
#include <limits>
#include <string>

/*
	Include my headers	
*/
#include "vertex.h"
#include "triangle.h"
#include "tedge.h"
#include "triangulation.h"
#include "eventQueue.h"
#include "settings.h"

#ifndef __TRANSLATION_H_
#define __TRANSLATION_H_

/*
	Define the different types a translation can end with:
		FULL: 		The translation was fully executed
		REJECTED: 	The translation has even not been started because the result is
					not simple or the event queue is unstable
		PARTIAL: 	The translation has been started, but was aborted due to instability
					of the event queue
		UNDONE: 	The translation has been started, but was aborted due to instability
					if the event queue and afterwards it was undone
*/
enum class Executed {FULL, REJECTED, PARTIAL, UNDONE};

/*
	Define the different types of translations:
		DEFAULT: 		A default translation generated by some function outside of the
						translation class
		SPLIT_PART_1: 	The first part of a splitted translation
		SPLIT_PART_2: 	The second part of a splitted translation
*/
enum class TranslationType {DEFAULT, SPLIT_PART_1, SPLIT_PART_2};

/*
	Remember the state change of a flip

	oldDV0 	One vertex of the edge which was flipped
	oldDV1 	The other vertex of the edge which was flipped
	newDV0 	One vertex of the inserted edge
	newDV1 	The other vertex of the inserted edge
*/
struct Flip{
	Vertex * const oldDV0;
	Vertex * const oldDV1;
	Vertex * const newDV0;
	Vertex * const newDV1;

	Flip(Vertex * const old0, Vertex * const old1, Vertex * const new0, Vertex * const new1) :
		oldDV0(old0), oldDV1(old1), newDV0(new0), newDV1(new1) {}
};

class Translation{

private:
	
	/*
		The triangulation the vertex to be translated lives in
	*/
	Triangulation * const T;

	/*
		The index of the vertex in the triangulation's vertex vector
	*/
	const int index;

	/*
		The vertex to be translated, a copy at the start position and a copy at the
		target position
	*/
	Vertex * original;
	Vertex * oldV;
	Vertex * newV;

	/*
		The neighboring vertices in the polygon
	*/
	Vertex *prevV;
	Vertex *nextV;

	/*
		The path the vertex should move along
	*/
	TEdge *transPath;

	/*
		The actual edges between the vertex and its neighbors
	*/
	TEdge *prevOldE;
	TEdge *nextOldE;

	/*
		The new polygon edges the translation will produce
	*/
	TEdge *prevNewE;
	TEdge *nextNewE;

	/*
		The components of the translation vector
	*/
	const double dx, dy;

	/*
		Indicates whether the translation must be split to be processed or can be processed
		directly
	*/
	bool split;

	/*
		The type of the translation
	*/
	const TranslationType type;

	/*
		The event queue for this translation
	*/
	EventQueue *Q;

	/*
		A stack containing information of all processed flips
	*/
	std::stack<Flip*> FlipStack;

	/*
		The event time of the last processed event
	*/
	double actualTime;

	/*
		P ~ R ~ I ~ V ~ A ~ T ~ E 	M ~ E ~ M ~ B ~ E ~ R 	F ~ U ~ N ~ C ~ T ~ I ~ O ~ N ~ S
	*/

	/*
		Private Constructor:
		Generates a new translation of any type, whereas the publiuc constructor is just
		capable of generating translations of type DEFAULT. Greps the moving vertex and its
		neighbors from the triangulations vertices list and initalize the translation path.

		@param 	Tr 		The triangulation the moving vertex lives in
		@param 	i 		The index of the moving vertex in the triangulation
		@param 	dX 		The x-component of the translation vector
		@param 	dY 		The y-component of the translation vector
		@param	type 	The type of the translation

		Note:
			A translation of non-DEFAULT type can just be generated by a DEFAULT
			translation which must be split.
	*/
	Translation(Triangulation *Tr, int i, double dX, double dY, TranslationType tp);

	/*
		The function generateInitialQueue() generates the initial version of the event queue.
		Therefore it decides for each triangle containing the moving vertex, whether the
		triangle will collapse during the translation. To achieve this it gets checked whether
		the opposite edge of the triangle lays between the start and the target position of the
		moving vertex. After inserting all collapsing triangles with their estimated collapse
		times into the event queue it lets check the event queue the stability of the event
		ordering and try to repair.

		@return 	True if the event queue is stable, otherwise false

		Note:
			This function also checks whether any of the triangles is zero and tries to repair
			it. If it finds something not repairable it erros with exit code 7.
	*/
	bool generateInitialQueue();

	/*
		The function insideQuadrilateral() checks whether the vertex v lays inside of a
		quadrilateral formed by the edge from oldV to its neighboring  vertices and from
		newV to its neighboring vertices.
		Therefore it generates a dummy vertex with the same y-coordinate as v and a
		x-coordinate which is the maximum x-coordinate of all vertices of the triangle plus
		10. So the dummy vertex lays definitelly outside of the qudrilateral. Then it
		checks how often the the edge between v and the dummy vertex intersects the edges
		of the quadrilateral. If the number of intersections is odd, then v must lay inside
		of the quadrilateral.

		@param 	v 	The vertex of interest
		@return 	True if v is inside of the quadrilateral, otherwise false

		Note:
			Source: https://www.geeksforgeeks.org/how-to-check-if-a-given-point-lies-inside-a-polygon/
	*/
	bool insideQuadrilateral(Vertex * const v) const;

	/*
		The function checkEdge() checks whether the edge newE starting at vertex fromV
		intersects any polygon edge. Therefore it first checks whether newE intersects any
		edge of the surrounding polygon of fromV. If it does not, then it can not intersect
		any polygon edge, otherwise we continue the check in the other triangle assigned to
		the intersected edge until newE either intersects a polygon edge or ends in any
		triangle.

		@param 	fromV 	The vertex where the new edge starts
		@param 	newE 	The new edge
		@return 		True if the new edge does not intersect any polygon edge,
						otherwise false

		Note:
			- If the edge goes right through another vertex we also count it as intersecting
				a polygonedge even if the edge we checked was no polygon edge, because a
				new polygon edge obviously is not allowed to go through a vertex.
			- As we had the case that a new edge numerically intersected all three edges of
				the same triangle or multiple edges of the surrounding polygon we do not
				stopp at the first intersection we find, but we count all intersections.
				If we find more then one, the function returns false.
			- Edges of the bounding box are treated as polygon edges
	*/
	bool checkEdge(Vertex * const fromV, TEdge * const newE) const;

	/*
		The function repairEnd() checks at the end of a translation whether there exist any
		triangle with area 0 connected to the moving vertex. If such a triangle exists it
		tries to flip it if possible, otherwise it tries to move the vertex back by 10% of
		the translation.

		Note:
			As the checkIntersection functions tries to keep vertices far from edges this
			should never be the case besides such splitted translation where the vertex is
			moved exactly to a triangulation edge on purpose.
	*/
	void repairEnd();

	/*
		The function executeSplitRetainSide() decomposes one translation, which can not be
		executed directly and where the moving vertex stays at the same side of the edge
		connecting the neighboring vertices, and executes the resulting translations. The
		decomposition is done in the following way:
		1. The vertex is moved to the position where the edge from the previous vertex to the
			old position intersects the edge from the next vertex to the new position resp.
			the edge from the next vertex to the old position intersects the edge from the
			previous vertex to the new position (exactly one of the two pairs of edges
			intersects!)
		2. The vertex is moved from the intersection position of the first part to the final
			position.

		@return 	Indicates whether the execution was rejected, aborted or fully processed

		Note:
			For detailed information why we can do that take a look at my Master Thesis
	*/
	enum Executed executeSplitRetainSide() const;

	/*
		The function executeSplitChangeSide() decomposes one translation, which can not be
		executed directly and where the moving vertex changes the side of the edge connecting
		the neighboring vertices, and executes the resulting translations. The decomposition
		is done in the following way:
		1. The vertex is moved exactly at the middle between the two neighboring vertices.
		2. The vertex is moved from the position between the two neighboring vertices to its
			final position.

		@return 	Indicates whether the execution was rejected, aborted or fully processed

		Note:
			For detailed information why we can do that take a look at my Master Thesis
	*/
	enum Executed executeSplitChangeSide();

	/*
		The function flip() executes one event by removing the longest edge of the collapsing
		triangle and inserting the other diagonal of the resulting quadrilateral. It errors
		with exit code 3 if the longest edge is a polygon edge. If the flip is no singleFlip
		it also computes whether the the two resulting triangles will also collapse during the
		further translation and in case insert them into the event queue and check its stability.
		This computation is outsourced to the functions insertAfterOppositeFlip() and
		insertAfterNonOppositeFlip().

		@param 	t0 			The collapsing triangle
		@param 	singleFlip 	Indicates whether this flip is part of working off the event queue,
							or it is just a single security flip
		@return 			True if the event queue is still stable, otherwise false

		Note:
			- It is assumed that the area of the collapsing triangle is zero (or at least close
				to 0), i.e. the moving vertex is already shifted to the event time
			- Checking whether a resulting triangle will collapse during the further translation
				is a highly sensible thing! It is not recommend to use the actual position of
				the moving vertex therefore, because the small errors in its position can lead
				to wrong decisions
			- For more information on the method of deciding take a look into my Master Thesis
	*/
	bool flip(Triangle *t0, const bool singleFlip);

	/*
		The function insertAfterOppositeFlip() decides whether newly generated triangles after
		an opposite flip (the edge not containing the moving vertex has been flipped) has to be
		inserted into the event queue. Therefore it does not use the actual position of the 
		moving vertex as this position may be corrupted by numerical inaccuracies. Instead just
		all static vertices of the triangles are used.

		@param 	leftT 	The left one of the new triangles
		@param 	rightT 	The right one of the new triangles
		@param 	leftV 	The vertex which is only contained by the left triangle
		@param 	rightV 	The vertex which is only contained by the right triangle
		@param 	common 	The vertex which is contained by both triangles, but is not the moving
						vertex
		@return 		True if one of the triangles has been inserted into the event queue,
						otherwise false

		Note:
			- For detailed information on the decision criteria take a look into my Master Thesis
			- Left and right may not be really left and right, but this does not matter as long
				as leftV corresponds to leftT and rightV to rightT
	*/
	bool insertAfterOppositeFlip(Triangle * leftT, Triangle * rightT, Vertex *leftV, Vertex *
		rightV, Vertex * common) const;

	/*
		The function insertAfterNonOppositeFlip() decides whether newly generated triangles after
		a non-opposite flip (one of the edges containing the moving vertex has been flipped) has to
		be inserted into the event queue. Therefore it does not use the actual position of the 
		moving vertex as this position may be corrupted by numerical inaccuracies. Instead just
		all static vertices of the triangles are used.

		@param 	t 			The one new triangle which can potentially collapse during the further
							translation
		@param 	shared0 	One of the two vertices which are shared by the two new triangles
		@param 	shared1 	The other of the two shared vertices
		@param 	opposite 	The non-shared vertex of the triangle which does not contain the
							moving vertex
		@return 			True if the triangles has been inserted into the event queue,
							otherwise false

		Note:
			- For detailed information on the decision criteria take a look into my Master Thesis
	*/
	bool insertAfterNonOppositeFlip(Triangle *t, Vertex * shared0, Vertex * shared1, Vertex *
		opposite) const;

	/*
		The function undo() checks whether moving vertex still lays inside of its 
		surrounding polygon. If it does not the function undoes all executed flips in
		reversed order and sets the moving vertex back to its original position.
	*/
	bool undo();


public:

	/*
		LIST OF PUBLIC MEMBER FUNCTIONS

		CONSTRUCTORS:
		
						Translation(Triangulation *Tr, int i, double dX, double dY)

		OTHERS:

		bool 			checkOverroll()
		enum Executed 	execute()
		bool 			checkSimplicityOfTranslation()
		void 			checkSplit()
	*/

	
	/*
		C ~ O ~ N ~ S ~ T ~ R ~ U ~ C ~ T ~ O ~ R ~ S
	*/

	/*
		Constructor:
		Generates a new Translation of type DEFAULT. Greps the moving vertex and its
		neighbors from the triangulations vertices list and initalize the translation path.

		@param 	Tr 		The triangulation the moving vertex lives in
		@param 	i 		The index of the moving vertex in the triangulation
		@param 	dX 		The x-component of the translation vector
		@param 	dY 		The y-component of the translation vector

		Note:
			Translations of other types can just be generated by the translation class itself.
	*/
	Translation(Triangulation *Tr, int i, double dX, double dY);
	

	/*
		O ~ T ~ H ~ E ~ R ~ S
	*/

	/*
		The function checkOverroll() checks whether the polygon would change its orientation
		by this translation. This basically means that the moving vertex is shifted across
		the whole polygon which corresponds to all other vertices and edges being inside of
		the qudrilateral formed by the oldV, the newV and their neighboring edges. Obviously
		if the quadrilateral is not simple, this can not happen at all. If the quadrilateral
		is simple then it corresponds to having one of the other vertices of the polygon
		inside the qudrilateral, because if one vertex is inside it follows all other
		vertices must be inside or at least one is outside so it exist at least one edge
		intersecting the quadrilateral so the translation can not lead to a simple polygon
		at all.

		@return 	True if the polygon would change its orientation, otherwise false
	*/
	bool checkOverroll() const;

	/*
		The function execute() processes a translation. If the flag split is set, it calls the
		functions which split the translation into two translations (depending on the geometric
		case) and executes them, otherwise execute() executes the translation by successively
		working through the events in the event queue.

		@return 	Indicates whether the execution was rejected, aborted or fully processed

		Note:
			For more information on the splits see my Master Thesis
	*/
	enum Executed execute();

	/*
		The function checkSimplicityOfTranslation() determines whether the result of a
		translation is a simple polygon. It does so by checking whether one of the edges
		from the new vertex position to one of its neighbors intersects a polygon edge.

		@return 	True if the resulting polygon is still simple, otherwise false
	*/
	bool checkSimplicityOfTranslation() const;

	/*
		The function checkSplit() determines whether a translation can be executed directly
		or must be split into two translation. This corresponds to the question whether the
		translation path intersects any polygon edge or not. If the translation must be
		split the function sets the internal split flag.
	*/
	void checkSplit();


	/*
		D ~ E ~ S ~ T ~ R ~ U ~ C ~ T ~ O ~ R
	*/

	/*
		Destructor:
		Checks and potentially repairs the surrounding polygon of the moved vertex and
		deletes all the remaining construction vertices and edges. It errors with exit
		code 6 if the surrounding polygon check fails. It also deletes the flip stack.
	*/	
	~Translation();
};

#endif

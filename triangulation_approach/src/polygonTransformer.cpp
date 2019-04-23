#include "polygonTransformer.h"
#include <string>

void transformPolygon(Triangulation* T, int iterations, Timer t){
	int index = 1;
	RandomGenerator* generator = new RandomGenerator(); // could maybe be a object instead of a pointer
	double dx = 0, dy = -50; // radius of the initial polygon was 30
	bool simple, split, overroll;
	Translation* trans;
	int n = (*T).getNumberOfVertices();
	std::string filename;

	for(int i = 0; i < iterations; i++){
		filename = "output/triangulation" + std::to_string(i) + ".graphml";
		//(*T).print(filename.c_str());

		index = (*generator).getRandomIndex(n);

		dx = (*generator).getTranslationNormal(0, 5);
		dy = (*generator).getTranslationNormal(0, 5);
		trans = new Translation(T, index, dx, dy);

		printf("iteration: %d index: %d dx: %f dy: %f \n", i, index, dx, dy);

		overroll = (*trans).checkOverroll();

		if(!overroll){
			simple = checkSimplicityOfTranslation(trans);

			//printf("simplicity check gives result %d \n", simple);

			if(simple){
				// check whether the translation can be done at once or must be split to move around a polygon edge
				split = checkEdge((*trans).getOriginal(), (*trans).getTranslationPath());
				if(!split){
					(*trans).setSplit();
					//printf("translation must be split \n");
				}

				(*trans).execute();
			}

			delete trans;
		}
	}

	/*trans = new Translation(T, 0, 0, -40);

	overroll = (*trans).checkOverroll();

	if(!overroll){

		simple = checkSimplicityOfTranslation(trans);

		printf("simplicity check gives result %d \n", simple);

		if(simple){
			// check whether the translation can be done at once or must be split to move around a polygon edge
			split = checkEdge((*trans).getOriginal(), (*trans).getTranslationPath());
			if(!split){
				(*trans).setSplit();
				printf("translation must be split \n");
			}

			(*trans).execute();
		}
	}

	delete trans;

	trans = new Translation(T, 2, -40, 10);

	overroll = (*trans).checkOverroll();

	if(!overroll){

		simple = checkSimplicityOfTranslation(trans);

		printf("simplicity check gives result %d \n", simple);

		if(simple){
			// check whether the translation can be done at once or must be split to move around a polygon edge
			split = checkEdge((*trans).getOriginal(), (*trans).getTranslationPath());
			if(!split){
				(*trans).setSplit();
				printf("translation must be split \n");
			}

			(*trans).execute();
		}
	}

	delete trans;

	trans = new Translation(T, index, -50, -50);

	overroll = (*trans).checkOverroll();

	if(!overroll){

		simple = checkSimplicityOfTranslation(trans);

		printf("simplicity check gives result %d \n", simple);

		if(simple){
			// check whether the translation can be done at once or must be split to move around a polygon edge
			split = checkEdge((*trans).getOriginal(), (*trans).getTranslationPath());
			if(!split){
				(*trans).setSplit();
				printf("translation must be split \n");
			}

			(*trans).execute();
		}
	}

	delete trans;*/
}

bool checkSimplicityOfTranslation(Translation* trans){
	Vertex *oldV, *newV, *prevV, *nextV;
	TEdge *prevE, *nextE, *prevNewE, *nextNewE;
	bool simple;
	
	oldV = (*trans).getOriginal();
	newV = (*trans).getNewV();

	prevV = (*trans).getPrevV();
	nextV = (*trans).getNextV();

	prevE = (*prevV).getEdgeTo(oldV);
	nextE = (*nextV).getEdgeTo(oldV);

	(*prevE).setEdgeType(EdgeType::TRIANGULATION);
	(*nextE).setEdgeType(EdgeType::TRIANGULATION);


	prevNewE = new TEdge(prevV, newV, EdgeType::POLYGON);
	nextNewE = new TEdge(newV, nextV, EdgeType::POLYGON);

	simple = checkEdge(prevV, prevNewE);
	simple = simple && checkEdge(nextV, nextNewE);

	(*prevE).setEdgeType(EdgeType::POLYGON);
	(*nextE).setEdgeType(EdgeType::POLYGON);

	delete prevNewE;
	delete nextNewE;

	return simple;
}

bool checkEdge(Vertex* fromV, TEdge* newE){
	std::vector<TEdge*> surEdges;
	enum intersect_t iType = intersect_t::IS_FALSE;
	EdgeType eType;
	Triangle* nextT = NULL;

	surEdges = (*fromV).getSurroundingEdges();
	Triangle* nexT;

	// start with the edge to the previous vertex
	for(auto& i : surEdges){
		iType = checkIntersection(newE, i);

		// new edge hits vertex of surrounding polygon
		if(iType > intersect_t::IS_FALSE && iType <= intersect_t::IS_VERTEX22) return false;
		// intersection with edge of surrounding polygon
		else if(iType == intersect_t::IS_TRUE){
			eType = (*i).getEdgeType();

			// intersected edge is polygon edge
			if(eType == EdgeType::POLYGON){
				//printf("edge hit polygon edge from vertex %d to vertex %d \n", (*(*i).getV1()).getID(), (*(*i).getV2()).getID());
				return false;
			// intersected edge is frame edge
			}else if(eType == EdgeType::FRAME){
				//printf("edge hit frame edge from vertex %d to vertex %d \n", (*(*i).getV1()).getID(), (*(*i).getV2()).getID());
				return false;
			// intersected edge is just a triangulation edge
			}else{
				//printf("edge hit triangulation edge from vertex %d to vertex %d \n", (*(*i).getV1()).getID(), (*(*i).getV2()).getID());
				nextT = (*i).getTriangleNotContaining(fromV);
				surEdges = (*nextT).getOtherEdges(i);
				break;
			}
		}
	}

	// iterate over the adjacent triangles it there was an intersection with a triangulation edge
	while(!(iType == intersect_t::IS_FALSE)){

		for(auto& i : surEdges){
			iType = checkIntersection(newE, i);

			// new edge hits vertex of surrounding polygon
			if(iType > intersect_t::IS_FALSE && iType <= intersect_t::IS_VERTEX22) return false;
			// intersection with edge of surrounding polygon
			else if(iType == intersect_t::IS_TRUE){
				eType = (*i).getEdgeType();

				// intersected edge is polygon edge
				if(eType == EdgeType::POLYGON){
					//printf("edge hit polygon edge from vertex %d to vertex %d \n", (*(*i).getV1()).getID(), (*(*i).getV2()).getID());
					return false;
				// intersected edge is frame edge
				}else if(eType == EdgeType::FRAME){
					//printf("edge hit frame edge from vertex %d to vertex %d \n", (*(*i).getV1()).getID(), (*(*i).getV2()).getID());
					return false;
				// intersected edge is just a triangulation edge
				}else{
					//printf("edge hit triangulation edge from vertex %d to vertex %d \n", (*(*i).getV1()).getID(), (*(*i).getV2()).getID());
					nextT = (*i).getOtherTriangle(nextT);
					surEdges = (*nextT).getOtherEdges(i);
					break;
				}
			}
		}
	}

	return true;
}


double signedArea(Vertex* v0, Vertex* v1, Vertex* v2){
	double area;
	double ax, ay, bx, by, cx, cy;

	ax = (*v0).getX();
	ay = (*v0).getY();

	bx = (*v1).getX();
	by = (*v1).getY();

	cx = (*v2).getX();
	cy = (*v2).getY();

	area = 0.5 * (- ay * bx + ax * by + ay * cx - by * cx - ax * cy + bx * cy);

	return area;
}



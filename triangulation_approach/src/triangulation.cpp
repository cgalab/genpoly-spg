#include "triangulation.h"

void Triangulation::addVertex(Vertex* v){
	vertices.push_back(v);
	(*v).setTriangulation(this);
}

void Triangulation::removeVertex(int index){
	vertices[index] = NULL;
}

void Triangulation::addEdge(TEdge* e){
	edges.push_back(e);
	(*e).setTriangulation(this);
}

void Triangulation::removeEdge(TEdge* e){
	edges.remove(e);
}

Vertex* Triangulation::getVertex(int i){ 	
	return vertices[i];
}

Vertex* Triangulation::getPVertex(int i){ 	
	if(i < 0){
		return vertices[N + i];
	}else if(i >= N){
		return vertices[i - N];
	}else{
		return vertices[i];
	}
	 return NULL;
}

// prints triangulation as graph into a graphml file
// https://de.wikipedia.org/wiki/GraphML
// works here: http://graphonline.ru/en/
void Triangulation::print(const char* filename){
	FILE* f;

	f = fopen(filename, "w");

	fprintf(f, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	fprintf(f, "<graphml>\n");
	fprintf(f, "<graph id=\"Graph\" edgeDefault=\"undirected\">\n");

	fprintf(f, "<nodes>\n");
	for(auto const& i : vertices){
		if(i != NULL) (*i).print(f);
	}
	fprintf(f, "</nodes>\n");

	fprintf(f, "<edges>\n");
	for(auto const& i : edges){
		(*i).print(f);
	}
	fprintf(f, "</edges>\n");

	fprintf(f, "</graph>\n");
	fprintf(f, "</graphml>\n");

	fclose(f);
}

void Triangulation::printPolygon(const char* filename){
	FILE* f;

	f = fopen(filename, "w");

	fprintf(f, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	fprintf(f, "<graphml>\n");
	fprintf(f, "<graph id=\"Graph\" edgeDefault=\"undirected\">\n");

	fprintf(f, "<nodes>\n");
	for(auto const& i : vertices){
		if(i != NULL && !(*i).isRectangleVertex()) (*i).print(f);
	}
	fprintf(f, "</nodes>\n");

	fprintf(f, "<edges>\n");
	for(auto const& i : edges){
		if((*i).getEdgeType() == EdgeType::POLYGON)(*i).print(f);
	}
	fprintf(f, "</edges>\n");

	fprintf(f, "</graph>\n");
	fprintf(f, "</graphml>\n");

	fclose(f);
}

void Triangulation::check(){
	EdgeType type;
	int n;

	for(auto const& i : edges){
		type = (*i).getEdgeType();
		n = (*i).nrAssignedTriangles();

		if(type == EdgeType::FRAME){
			if(n != 1){
				printf("Edge of type FRAME with %d triangles:\n \t", n);
				(*i).print();
			}
		}else{
			if(n != 2){
				printf("Edge of type not FRAME with %d triangles:\n \t", n);
				(*i).print();
			}
		}			
	}
}

TEdge* Triangulation::getEdge(int index){
	std::list<TEdge*>::iterator it = edges.begin();
    std::advance(it, index);

    return *it;
}

void Triangulation::deleteVertex(int index){
	Vertex* v;
	
	v = vertices[index];
	vertices[index] = NULL;
	delete v;
}
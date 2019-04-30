#include "triangulation.h"

// Constructors
Triangulation::Triangulation(int n){ 
	vertices.reserve(n + 4);
	N = n;
}

// Getter
int Triangulation::getNumberOfVertices(){
	return N;
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

// Setter
void Triangulation::addVertex(Vertex* v){
	vertices.push_back(v);
	(*v).setTriangulation(this);
}

void Triangulation::addEdge(TEdge* e){
	edges.insert(std::pair<int, TEdge*>((*e).getID(), e));
	(*e).setTriangulation(this);
}

// Remover
void Triangulation::removeVertex(int index){
	vertices[index] = NULL;
}

void Triangulation::removeEdge(TEdge* e){
	edges.erase((*e).getID());
}




// Printer

// prints triangulation as graph into a graphml file
// https://de.wikipedia.org/wiki/GraphML
// works here: http://graphonline.ru/en/
void Triangulation::print(const char* filename){
	FILE* f;
	TEdge* e;

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
		e = i.second;
		(*e).print(f);
	}
	fprintf(f, "</edges>\n");

	fprintf(f, "</graph>\n");
	fprintf(f, "</graphml>\n");

	fclose(f);
}

void Triangulation::printPolygon(const char* filename){
	FILE* f;
	TEdge* e;

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
		e = i.second;
		if((*e).getEdgeType() == EdgeType::POLYGON)(*e).print(f);
	}
	fprintf(f, "</edges>\n");

	fprintf(f, "</graph>\n");
	fprintf(f, "</graphml>\n");

	fclose(f);
}

// Others
void Triangulation::check(){
	EdgeType type;
	int n;
	TEdge* e;

	for(auto const& i : edges){
		e = i.second;
		type = (*e).getEdgeType();
		n = (*e).nrAssignedTriangles();

		if(type == EdgeType::FRAME){
			if(n != 1){
				printf("Edge of type FRAME with %d triangles:\n \t", n);
				(*e).print();
			}
		}else{
			if(n != 2){
				printf("Edge of type not FRAME with %d triangles:\n \t", n);
				(*e).print();
			}
		}			
	}
}
#include "triangulation.h"

// Constructors
Triangulation::Triangulation(int n){ 
	vertices.reserve(n);
	N = n;

	Rectangle0 = NULL;
	Rectangle1 = NULL;
	Rectangle2 = NULL;
	Rectangle3 = NULL;
}

// Getter
int Triangulation::getTargetNumberOfVertices(){
	return N;
}

int Triangulation::getActualNumberOfVertices(){
	return vertices.size();
}

Vertex* Triangulation::getVertex(int i){ 	
	int n;

	n = vertices.size();

	if(i < 0){
		return vertices[n + i];
	}else if(i >= n){
		return vertices[i - n];
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

void Triangulation::setRectangle(Vertex* v0, Vertex* v1, Vertex* v2, Vertex* v3){
	Rectangle0 = v0;
	Rectangle1 = v1;
	Rectangle2 = v2;
	Rectangle3 = v3;

	(*v0).setTriangulation(this);
	(*v1).setTriangulation(this);
	(*v2).setTriangulation(this);
	(*v3).setTriangulation(this);
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

	(*Rectangle0).print(f, 0);
	(*Rectangle1).print(f, 0);
	(*Rectangle2).print(f, 0);
	(*Rectangle3).print(f, 0);

	for(auto const& i : vertices){
		if(i != NULL) (*i).print(f, 0);
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
		if(i != NULL && !(*i).isRectangleVertex()) (*i).print(f, 0);
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
bool Triangulation::check(){
	EdgeType type;
	int n;
	TEdge* e;
	bool ok = true;

	for(auto const& i : edges){
		e = i.second;
		type = (*e).getEdgeType();
		n = (*e).nrAssignedTriangles();

		if(type == EdgeType::FRAME){
			if(n != 1){
				printf("Edge of type FRAME with %d triangles:\n \t", n);
				(*e).print();
				ok = false;
			}
		}else{
			if(n != 2){
				printf("Edge of type not FRAME with %d triangles:\n \t", n);
				(*e).print();
				ok = false;
			}
		}

		if((*e).getV1() == (*e).getV2()){
			printf("Edge %llu has two identical vertices with id %llu \n", (*e).getID(), (*(*e).getV1()).getID());
			ok = false;
		}		
	}

	for(auto const& i : vertices){
		ok = ok && (*i).check();
	}

	return ok;
}

void Triangulation::stretch(double factor){
	(*Rectangle0).stretch(factor);
	(*Rectangle1).stretch(factor);
	(*Rectangle2).stretch(factor);
	(*Rectangle3).stretch(factor);

	for(auto const& i : vertices){
		(*i).stretch(factor);
	}
}
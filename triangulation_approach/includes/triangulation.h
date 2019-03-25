#include "vertex.h"
#include "tedge.h"
#include "triangle.h"
#include <vector>
#include <stdio.h>

#ifndef __TRIANGULATION_H_
#define __TRIANGULATION_H_

class Triangulation {

private:
	std::vector<Vertex*> vertices;
	std::list<TEdge*> edges;

public:
	Triangulation(int n){ vertices.reserve(n); }
	void addVertex(Vertex* v){
		vertices.push_back(v);
	}

	void addEdge(TEdge* e){
		edges.push_back(e);
	}

	Vertex* getVertex(int index){
		return vertices[index];
	}

	// prints triangulation as graph into a graphml file
	// https://de.wikipedia.org/wiki/GraphML
	// works here: http://graphonline.ru/en/
	void print(char* filename){
		FILE* f;

		f = fopen(filename, "w");

		fprintf(f, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
		fprintf(f, "<graphml>\n");
		fprintf(f, "<graph id=\"Graph\" edgeDefault=\"undirected\">\n");

		fprintf(f, "<nodes>\n");
		for(auto const& i : vertices){
			(*i).print(f);
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
};

#endif
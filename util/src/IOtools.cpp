#include "IOtools.h"

void printPolygonToDat(Polygon p, char* filename){
	int i;
	FILE *f = fopen(filename, "w");
	int n = p.getNumberOfVertices();
	Point* v;

	fprintf(f, "# X  Y \n");

	for(i = 0; i < n; i++){
		v = p.getVertex(i);
		fprintf(f, "%f %f \n", (*v).x, (*v).y);
	}
	// close the polygon
	v = p.getVertex(0);
	fprintf(f, "%f %f \n", (*v).x, (*v).y);

	fclose(f);
}
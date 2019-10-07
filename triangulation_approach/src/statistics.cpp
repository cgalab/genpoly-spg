#include "statistics.h"

void calculateDistanceDistribution(Triangulation const * const T, const double width){
	int n_seg; // The number of segments
	int n_vert; // The number of vertices of the polygon
	int i, cat;
	std::vector<int> segments;
	Vertex *v;
	double distance, area;

	n_seg = ceil(Settings::boxSize / (sqrt(2) * width)) + 1;
	segments.reserve(n_seg);

	printf("%d \n", n_seg);

	for(i = 0; i < n_seg; i++)
		segments.push_back(0);

	n_vert = (*T).getActualNumberOfVertices();

	for(i = 0; i < n_vert; i++){
		v = (*T).getVertex(i);
		distance =(*v).getDistanceToOrigin();

		cat = floor(distance / width);
		segments[cat]++;
	}

	for(i = 0; i < n_seg; i++)
		printf("%.4f \t", width * i + width / 2);

	printf("\n");

	for(i = 0; i < n_seg; i++){
		area = pow((i + 1) * width, 2) * M_PI - pow(i * width, 2) * M_PI;
		printf("%6.f \t", segments[i] / area);
	}

	printf("\n");
}
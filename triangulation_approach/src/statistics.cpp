#include "statistics.h"

/*
	The function calculateDistanceDistribution() calculates the distance distribution
	for all vertices (including the vertices of the holes) and prints it to stdout.
	Therefore it splits the bounding box into ring layers of diameter width, counts
	the vertices in each layer and compute the vertex densities in the layers.
	The number of layers is computed by the distance of the origin to the bounding 
	box divided by width.

	Note:
		As the box is a rectangle the outer layer is in fact no ring anymore, but is
		defined as the rectangle minus the circle defined by the outer border of the
		second outer layer.
*/
void calculateRadialDistanceDistribution(Triangulation const * const T, const double width){
	int n_seg; // The number of segments
	int n_vert; // The number of vertices of the polygon
	int i, cat;
	std::vector<int> segments;
	Vertex *v;
	double distance, area = 0;

	n_seg = ceil(Settings::boxSize / (2 * width)) + 1;
	segments.reserve(n_seg);

	for(i = 0; i < n_seg; i++)
		segments.push_back(0);

	n_vert = (*T).getActualNumberOfVertices();

	for(i = 0; i < n_vert; i++){
		v = (*T).getVertex(i);
		distance =(*v).getDistanceToOrigin();

		cat = floor(distance / width);
		if(cat < n_seg)
			segments[cat]++;
		else
			segments[n_seg - 1]++;
	}

	for(i = 0; i < n_seg; i++)
		printf("%.4f \t", width * i + width / 2);

	printf("\n");

	for(i = 0; i < n_seg - 1; i++){
		area = pow((i + 1) * width, 2) * M_PI - pow(i * width, 2) * M_PI;
		printf("%6.f \t", segments[i] / area);
	}
	area = pow(Settings::boxSize, 2) - area;
	printf("%6.f \t", segments[n_seg - 1] / area);

	printf("\n");
}

void calculateRadialDistanceDeviation(Triangulation const * const T){
	double sum = 0;
	double radius = Settings::radiusPolygon;
	double dist;
	Vertex *start, *v;

	start = (*T).getVertex(0, 0);
	v = start;

	do{
		dist = (*v).getDistanceToOrigin();

		sum = sum + pow(dist - radius, 2);

		v = (*v).getNext();

	}while((*v).getID() != (*start).getID());

	sum = sum / ((*T).getActualNumberOfVertices(0) - 1);

	sum = sqrt(sum);

	printf("Initial radius: %.3f\n", radius);
	printf("Box size: %.3f\n", Settings::boxSize);
	printf("Radial distance deviation: %.3f\n", sum);
}

void calculateMaxTwist(Triangulation const * const T){
	int n = (*T).getActualNumberOfVertices(0);
	double min, max, sum, angle, average;
	Vertex *start, *v;

	// Calculate the average inside angle of the polygon
	average = 180 * (1 - 2.0 / n);
	printf("Average inside angle: %.3f\n", average);

	start = (*T).getVertex(0, 0);

	angle = (*start).getInsideAngle() / M_PI * 180;
	angle = angle - average;
	sum = angle;
	min = angle;
	max = angle;

	v = (*start).getNext();
	while((*v).getID() != (*start).getID()){
		angle = (*v).getInsideAngle() / M_PI * 180;
		angle = angle - average;

		sum = sum + angle;

		if(sum < min)
			min = sum;

		if(sum > max)
			max = sum;

		v = (*v).getNext();
	}

	printf("sum at end: %.3f\n", sum);
	printf("min: %.3f max: %.3f amplitude: %.3f\n", min, max, fabs(min) + fabs(max));

}

void countOrientationChanges(Triangulation const * const T){
	int n = 0;
	double angle;
	Vertex *start, *v;
	bool toRight = false;

	start = (*T).getVertex(0, 0);

	// Initialise direction
	angle = (*start).getInsideAngle();
	if(angle > M_PI)
		toRight = true;

	v = start;
	do{
		v = (*v).getNext();

		angle = (*v).getInsideAngle();

		// Angles greater then pi a considered as curves to the right,
		// others as curve to the left
		if(angle > M_PI){
			if(!toRight)
				n++;
			toRight = true;
		}else{
			if(toRight)
				n++;
			toRight = false;
		}
	}while((*v).getID() != (*start).getID());

	printf("Number of orientation changes: %d\n", n);
}
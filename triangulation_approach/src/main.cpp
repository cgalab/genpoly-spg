#include "initialPolygonGenerator.h"
#include <stdlib.h>
#include <stdio.h>
#include "triangulation.h"
#include "polygonTransformer.h"
#include "settings.h"
#include "statistics.h"

/*
list of exit codes:
code 	name						meaning
1		Circle edge					An edge starts at the same vertex as it ends
2		Vertex at PE end			A vertex lies exactly at a polygon edge after a translation
3		PE flip 					The edge to be fliped at an event is a polygon edge
4		Triangle overflow			A new triangle is assigned to an edge which already has two triangles
5		Triangle redundance			A new triangle is assigned to an edge which already has the same 
									triangle registered
6		Surrounding polygon fail	The moving vertex is not anymore inside of its surrouding polygon
									at the end of a translation
7		Vertex at PE start 			The moving vertex lies at a polygon edge at the begining of a translation
8		temporarly empty
9		Triangulation error			Some vertex has left its surrouding polygon but it hasn't been recognized
									during the translation
10		Vertex at PE end 			The surrounding polygon check recognizes a vertex exactly laying on a PE
11 		Not simple					The check for simplicity found an intersection
12 		Vertex insertion error 		The pID passed to Triangulation::addVertex() exceeds the number of inner
									polygons
13 		Parameter error				A parameter given in the configuration file has a wrong type
14 		Setting error				Some mandatory settings are not given or settings are conflicting
*/

int main(int argc, char *argv[]){
	Triangulation* T;

	if(argc != 2){
		printf("Usage: fpg <CONFIG FILE>\n");
		Settings::printDummyFile();
		printf("Printed a dummy config file named dummy.fpg\n");
		exit(14);
	}else{
		Settings::readConfigFile(argv[1]);
		Settings::checkAndApplySettings();
		
		if(!Settings::mute)
			Settings::printSettings();
	}

	T = generateRegularPolygon();

	(*T).check();

	if(Settings::executionInfo)
		printf("Initial polygon with %d vertices in regular shape computed after %f seconds\n",
			Settings::initialSize, (*Settings::timer).elapsedTime());

	if(Settings::nrInnerPolygons == 0)
		strategyNoHoles0(T);
	else if(Settings::nrInnerPolygons == 1)
		strategyWithHoles0(T);
	else
		strategyWithHoles0(T);

	(*T).writePolygonToDat(Settings::polygonFile);
	(*T).writePolygon("polygon.graphml");

	if(Settings::triangulationOutputRequired)
		(*T).writeTriangulation(Settings::triangulationFile);

	calculateRadialDistanceDistribution(T, 0.25);
	calculateMaxTwist(T);
	countOrientationChanges(T);
	calculateRadialDistanceDeviation(T);
	
	exit(0);
}
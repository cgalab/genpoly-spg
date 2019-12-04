#include "settings.h"

/*
	Polygon parameters
*/
unsigned int Settings::nrInnerPolygons = 0;
int Settings::outerSize = 0;
std::vector<int> Settings::innerSizes;


/*
	Initial polygon
*/
int Settings::initialSize = 10;
double Settings::radiusPolygon = 0.1;
double Settings::radiusHole = 0.05;
double Settings::boxSize = 3.0;
int Settings::initialTranslationFactor = 1000;
int Settings::initialTranslationNumber = -1;


/*
	Translation parameters
*/
Arithmetics Settings::arithmetics = Arithmetics::DOUBLE;
bool Settings::localChecking = true;
bool Settings::globalChecking = false;


/*
	Insertion parameters
*/
int Settings::insertionTries = 100;
double Settings::minLength = 0.0000001;


/*
	Utilities
*/
Timer* Settings::timer = NULL;


/*
	Random generator
*/
bool Settings::fixedSeed = false;
unsigned int Settings::seed = 0;
RandomGenerator* Settings::generator = NULL;


/*
	Feedback settings
*/
FeedbackMode Settings::feedback = FeedbackMode::EXECUTION;
bool Settings::executionInfo = true;
bool Settings::correctionInfo = false;
bool Settings::mute = false;
bool Settings::simplicityCheck = false;
char* Settings::polygonFile = (char*)"polygon.dat";
bool Settings::triangulationOutputRequired = false;
char* Settings::triangulationFile = NULL;
char* Settings::statisticsFile = NULL;


void Settings::readConfigFile(char *filename){
	std::fstream file;
	std::string line;
	char *token;
	char delimiters[] = "= ,{}\n:\"";
	unsigned int i;
	bool found;

	printf("Read configuration file %s...", filename);

	file.open(filename, std::ios::in);
	if(!file.is_open()){
		printf("\nCannot open configuration file: \"%s\"\n", filename);
		exit(100);
	}

	while(getline(file, line)){
		if(!(token = strtok(stringToChar(line), delimiters)))
            continue;
        
        if(token[0] == '#')
            continue;

        for (i = 0; i < strlen(token); i++)
            token[i] = (char) toupper(token[i]);
        if(!strcmp(token, "NROFHOLES")){
        	if(!(token = strtok(0, delimiters)) || !sscanf(token, "%d", &nrInnerPolygons)){
        		printf("NrOfHoles: expected integer, got \"%s\"!\n", token);
        		exit(13);
        	}
        }else if(!strcmp(token, "POLYGONSIZE")){
        	if(!(token = strtok(0, delimiters)) || !sscanf(token, "%d", &outerSize)){
        		printf("PolygonSize: expected integer, got \"%s\"!\n", token);
        		exit(13);
        	}
        }else if(!strcmp(token, "STARTSIZE")){
        	if(!(token = strtok(0, delimiters)) || !sscanf(token, "%d", &initialSize)){
        		printf("StartSize: expected integer, got \"%s\"!\n", token);
        		exit(13);
        	}
        }else if(!strcmp(token, "SEED")){
        	if(!(token = strtok(0, delimiters)) || !sscanf(token, "%d", &seed)){
        		printf("Seed: expected integer, got \"%s\"!\n", token);
        		exit(13);
        	}
        }else if(!strcmp(token, "FIXEDSEED")){
        	fixedSeed = Settings::readBoolean(found);
        	if(!found){
        		printf("FixedSeed: boolean expected!\n");
        		exit(13);
        	}
        }else if(!strcmp(token, "ARITHMETIC")){
        	arithmetics = Settings::readArithmeticType(found);
        	if(!found){
        		printf("Arithmetic: \"EXACT\" or \"DOUBLE\" expected!\n");
        		exit(13);
        	}
        }else if(!strcmp(token, "HOLESIZES")){
        	Settings::readHoleSizes(found);
        	if(!found){
        		printf("HoleSizes: single integer or list of integers expected!\n");
        		exit(13);
        	}
        }else if(!strcmp(token, "POLYGONFILE")){
        	if(!(token = strtok(0, delimiters))){
        		printf("PolygonFile: missing argument!\n");
        		exit(13);
        	}
        	Settings::polygonFile = token;
        }else if(!strcmp(token, "TRIANGULATIONFILE")){
        	if(!(token = strtok(0, delimiters))){
        		printf("TriangulationFile: missing argument!\n");
        		exit(13);
        	}
        	Settings::triangulationFile = token;
        }else if(!strcmp(token, "STATISTICSFILE")){
        	if(!(token = strtok(0, delimiters))){
        		printf("StatisticsFile: missing argument!\n");
        		exit(13);
        	}
        	Settings::statisticsFile = token;
        }else if(!strcmp(token, "PRINTEXECUTIONINFO")){
        	executionInfo = Settings::readBoolean(found);
        	if(!found){
        		printf("PrintExecutionInfo: boolean expected!\n");
        		exit(13);
        	}
        }else if(!strcmp(token, "NUMERICALCORRECTIONINFO")){
        	correctionInfo = Settings::readBoolean(found);
        	if(!found){
        		printf("NumericalCorrectionInfo: boolean expected!\n");
        		exit(13);
        	}
        }else if(!strcmp(token, "ENABLELOCALCHECKS")){
        	localChecking = Settings::readBoolean(found);
        	if(!found){
        		printf("EnableLocalChecks: boolean expected!\n");
        		exit(13);
        	}
        }else if(!strcmp(token, "ENABLEGLOBALCHECKS")){
        	globalChecking = Settings::readBoolean(found);
        	if(!found){
        		printf("EnableGlobalChecks: boolean expected!\n");
        		exit(13);
        	}
        }else if(!strcmp(token, "COMPLETEMUTE")){
        	mute = Settings::readBoolean(found);
        	if(!found){
        		printf("CompleteMute: boolean expected!\n");
        		exit(13);
        	}
        }else{
        	printf("Unknown token %s\n", token);
        	exit(13);
        }
	}

	file.close();

	printf("successful\n\n");
}

void Settings::printSettings(){
	unsigned int i;

	printf("Polygon settings:\n");
	printf("Number of holes: %d\n", nrInnerPolygons);
	printf("Number of vertices (start polygon): %d\n", initialSize);
	printf("Target number of vertices (polygon): %d\n", outerSize);

	if(nrInnerPolygons > 0){
		printf("Target number of vertices (holes):\n");
		for(i = 0; i < nrInnerPolygons; ++i){
			printf("%d\n", innerSizes[i]);
		}
	}

	printf("\n");

	printf("Machine settings:\n");
	if(arithmetics == Arithmetics::DOUBLE)
		printf("Arithmetic: DOUBLE\n");
	else
		printf("Arithmetic: EXACT\n");
	if(fixedSeed)
		printf("Seed configuration: FIXED\n");
	else
		printf("Seed configuration: RANDOM\n");
	printf("Seed: %llu\n", seed);
	if(localChecking)
		printf("Local checking: enabled\n");
	else
		printf("Local checking: disbaled\n");
	if(globalChecking)
		printf("Global checking: enabled\n");
	else
		printf("Global checking: disbaled\n");

	printf("\n");

	printf("Output settings:\n");
	printf("Polygon file: %s\n", polygonFile);
	if(triangulationOutputRequired)
		printf("Triangulation file: %s\n", triangulationFile);
	if(statisticsFile != NULL)
		printf("Statistics file: %s\n", statisticsFile);
	if(executionInfo)
		printf("Print execution information: true\n");
	else
		printf("Print execution information: false\n");
	if(correctionInfo)
		printf("Print numerical correction information: true\n");
	else
		printf("Print numerical correction information: false\n");

	printf("\n");
}

// cast string to char*
char* Settings::stringToChar(std::string str){
	char * writable = new char[str.size() + 1];
	strcpy(writable, str.c_str());

	return writable;
}

// Reads booleans
bool Settings::readBoolean(bool &found){
    bool out = false;
    unsigned int i;
    char *token = strtok(0, "= ,{}\n");

    found = true;
    if(token){
        for(i = 0; i < strlen(token); i++)
            token[i] = (char) toupper(token[i]);

        if(!strcmp(token, "TRUE"))
            out = true;
        else if(!strcmp(token, "FALSE"))
            out = false;
        else
            found = false;
    }

    return out;
}

// Read arithmetic type
Arithmetics Settings::readArithmeticType(bool &found){
    Arithmetics out = Arithmetics::DOUBLE;
    unsigned int i;
    char *token = strtok(0, "= ,{}\n");

    found = true;
    if(token){
        for(i = 0; i < strlen(token); i++)
            token[i] = (char) toupper(token[i]);

        if(!strcmp(token, "EXACT"))
            out = Arithmetics::EXACT;
        else if(!strcmp(token, "DOUBLE"))
            out = Arithmetics::DOUBLE;
        else
            found = false;
    }

    return out;
}

// Checks whether all necessary settings are given
void Settings::checkAndApplySettings(){
	unsigned int i;
	bool conflict = false;
	printf("Checking for configuration conflicts...");

	if(outerSize < 3){
		printf("The polygon must have at least 3 vertices, given number %d\n", outerSize);
		exit(14);
	}

	if(outerSize < initialSize){
		printf("The size of the start polygon is not allowed to exceed the target size:\n");
		printf("Given start size: %d Given target size: %d\n", initialSize, outerSize);
		exit(14);
	}

	if(nrInnerPolygons != innerSizes.size()){
		printf("Conflicting number of holes:\n");
		printf("Given number: %d Given number of sizes: %d\n", nrInnerPolygons,
			innerSizes.size());
		exit(14);
	}

	for(i = 0; i < nrInnerPolygons; i++){
		if(innerSizes[i] < 3){
			printf("Holes must have a size of at least 3, given size for polygon with id %d: %d\n",
				i + 1, innerSizes[i]);
			exit(14);
		}
	}

	if(fixedSeed){
		if(seed == 0){
			printf("\nNote: FixedSeed is marked to use, but no seed is given!\n");
			conflict = true;
		}
	}else{
		if(seed != 0){
			printf("\nNote: A seed is given but FixedSeed is not marked to use!\n");
			conflict = true;
		}
	}

	if(!conflict)
		printf("no conflicts\n\n");
	else
		printf("\n");


	// Apply settings

	// Mute overrides all other command line output settings
	if(mute){
		executionInfo = false;
		correctionInfo = false;

		printf("Command line muted!\n");
	}

	// Generate and start Timer
	timer = new Timer();
	(*timer).start();

	// Compute the number of translations for the initial polygon
	if(initialSize < outerSize){
		initialTranslationNumber = initialTranslationFactor * initialSize;
	}else{
		initialTranslationNumber = initialTranslationFactor * outerSize;
		initialSize = outerSize;
	}

	// Initialize the RandomGenerator
	generator = new RandomGenerator(fixedSeed, seed);

	// Enable triangulation output
	if(triangulationFile != NULL)
		triangulationOutputRequired = true;

	// Initialize the exact arithmetic
	if(arithmetics == Arithmetics::EXACT)
		exactinit();
}

// Get hole sizes
void Settings::readHoleSizes(bool &found){
	int n, k;
	unsigned int i;
	char delimiters[] = "= ,{}\n:";
	char *token;

	// Get the first given size
	if(!(token = strtok(0, delimiters)) || !sscanf(token, "%d", &n)){
		found = false;
		return;
	}
	innerSizes.push_back(n);
	k = n;

	// No we have to check whether there is an additional size given
	if(!(token = strtok(0, delimiters)) || !sscanf(token, "%d", &n)){
		// If no additional size is given, all holes may have the same size
		if(nrInnerPolygons == 0){
			printf("HoleSizes: Can not be specified as single integer before specifying\
				the number of holes\n");
			found = false;
			return;
		}

		for(i = 1; i < nrInnerPolygons; i++)
			innerSizes.push_back(k);
	}

	// Hole sizes are given as a list
	while(token != NULL){
		innerSizes.push_back(n);

		// Get next
		token = strtok(0, delimiters);

		if(token != NULL){
			if(!sscanf(token, "%d", &n)){
				printf("HoleSizes: integer expected, got %s\n", token);
			}
		}
	}
	found = true;
}

//Print a dummy configuration file
void Settings::printDummyFile(){
	FILE *f;

	f = fopen("dummy.fpg", "w");

	fprintf(f, "##################################################################################\n");
	fprintf(f, "# This is an example configuration file for FPG                                  #\n");
	fprintf(f, "# All lines starting with '#' are ignored!                                       #\n");
	fprintf(f, "##################################################################################\n");

	fprintf(f, "\n\n");


	fprintf(f, "##################################################################################\n");
	fprintf(f, "# Specify the number of holes (optional; default: 0)                             #\n");
	fprintf(f, "##################################################################################\n");
	fprintf(f, "NrOfHoles = 2\n");

	fprintf(f, "\n");

	fprintf(f, "##################################################################################\n");
	fprintf(f, "# Specify the target number of vertices for the polygon (mandatory)              #\n");
	fprintf(f, "##################################################################################\n");
	fprintf(f, "PolygonSize = 1000\n");

	fprintf(f, "\n");

	fprintf(f, "##################################################################################\n");
	fprintf(f, "# Specifies the the size of the start polygon (optional; default: 10)            #\n");
	fprintf(f, "# Note that the StartSize must be less or equal to PolygonSize!                  #\n");
	fprintf(f, "##################################################################################\n");
	fprintf(f, "StartSize = 5\n");

	fprintf(f, "\n");

	fprintf(f, "##################################################################################\n");
	fprintf(f, "# Specify the number of vertices of the holes either as one integer, so all      #\n");
	fprintf(f, "# holes have the same size, or as comma-separated list surrounded by {}.         #\n");
	fprintf(f, "# If it is specified as one integer, the number of holes must have been          #\n");
	fprintf(f, "# specified before!                                                              #\n");
	fprintf(f, "# (mandatory if the number of holes is not 0)                                    #\n");
	fprintf(f, "##################################################################################\n");
	fprintf(f, "HoleSizes = {25, 20}\n");

	fprintf(f, "\n\n");


	fprintf(f, "##################################################################################\n");
	fprintf(f, "# Specifies the type of arithmetics for determinants, either standard            #\n");
	fprintf(f, "# double (DOUBLE) or Shewchuk's predicates (EXACT) (optional; default: DOUBLE)   #\n");
	fprintf(f, "##################################################################################\n");
	fprintf(f, "Arithmetic = EXACT\n");

	fprintf(f, "\n");

	fprintf(f, "##################################################################################\n");
	fprintf(f, "# Enables local correctness checking after aborted translations.                 #\n");
	fprintf(f, "# (optional; default: true)                                                      #\n");
	fprintf(f, "##################################################################################\n");
	fprintf(f, "EnableLocalChecks = false\n");

	fprintf(f, "\n");

	fprintf(f, "##################################################################################\n");
	fprintf(f, "# Enables global correctness checking between stages and after the last stage.   #\n");
	fprintf(f, "# (optional; default: false)                                                     #\n");
	fprintf(f, "##################################################################################\n");
	fprintf(f, "EnableGlobalChecks = true\n");

	fprintf(f, "\n");

	fprintf(f, "##################################################################################\n");
	fprintf(f, "# Specifies whether a given seed should be used (TRUE) or a seed generated by    #\n");
	fprintf(f, "# the machine (FALSE) (optional; default: FALSE)                                 #\n");
	fprintf(f, "##################################################################################\n");
	fprintf(f, "FixedSeed = false\n");

	fprintf(f, "\n");

	fprintf(f, "##################################################################################\n");
	fprintf(f, "# The seed to use (mandatory if FixedSeed = TRUE; 0 is permitted)                #\n");
	fprintf(f, "##################################################################################\n");
	fprintf(f, "Seed = 123456\n");

	fprintf(f, "\n\n");


	fprintf(f, "##################################################################################\n");
	fprintf(f, "# Specifies the output file for the polygon (optional; default: polygon.dat)     #\n");
	fprintf(f, "##################################################################################\n");
	fprintf(f, "PolygonFile = \"poly.dat\"\n");

	fprintf(f, "\n");

	fprintf(f, "##################################################################################\n");
	fprintf(f, "# Specifies a graphml-file to print the whole triangulation in.                  #\n");
	fprintf(f, "# (optional; default: NULL)                                                      #\n");
	fprintf(f, "##################################################################################\n");
	fprintf(f, "TriangulationFile = \"triangulation.graphml\"\n");

	fprintf(f, "\n");

	fprintf(f, "##################################################################################\n");
	fprintf(f, "# Specifies a file to print the statistics of the polygon and generation in.     #\n");
	fprintf(f, "# (optional; default: NULL)                                                      #\n");
	fprintf(f, "##################################################################################\n");
	fprintf(f, "StatisticsFile = \"stats.dat\"\n");

	fprintf(f, "\n\n");


	fprintf(f, "##################################################################################\n");
	fprintf(f, "# Mute all output to stdout. (optional; default: false)                          #\n");
	fprintf(f, "##################################################################################\n");
	fprintf(f, "CompleteMute = false\n");

	fprintf(f, "\n");

	fprintf(f, "##################################################################################\n");
	fprintf(f, "# Specify whether FPG should print information on the state of execution during  #\n");
	fprintf(f, "# the execution. (optional; default:true)                                        #\n");
	fprintf(f, "##################################################################################\n");
	fprintf(f, "PrintExecutionInfo = false\n");

	fprintf(f, "\n");

	fprintf(f, "##################################################################################\n");
	fprintf(f, "# Specify whether FPG should print note numerical corrections at stdout.         #\n");
	fprintf(f, "# (optional; default:false)                                                      #\n");
	fprintf(f, "##################################################################################\n");
	fprintf(f, "NumericalCorrectionInfo = true\n");

	fclose(f);
}
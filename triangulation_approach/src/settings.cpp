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
bool Settings::simplicityCheck = false;

void Settings::initSettings(){
	// Generate and start Timer
	timer = new Timer();
	(*timer).start();

	// Generate the vector of sizes of the inner polygons
	//innerSizes.push_back(100000);

	// Compute the number of translations for the initial polygon
	if(initialSize < outerSize){
		initialTranslationNumber = initialTranslationFactor * initialSize;
	}else{
		initialTranslationNumber = initialTranslationFactor * outerSize;
		initialSize = outerSize;
	}

	// Initialize the RandomGenerator
	generator = new RandomGenerator(fixedSeed, seed);

	// Initialize the exact arithmetic
	if(arithmetics == Arithmetics::EXACT)
		exactinit();
}

void Settings::readConfigFile(char *filename){
	std::fstream file;
	std::string line;
	char *token;
	char delimiters[] = "= ,{}\n";
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
        	if (!(token = strtok(0, delimiters)) || !sscanf(token, "%d", &nrInnerPolygons)){
        		printf("NrOfHoles: expected integer, got \"%s\"!\n", token);
        		exit(13);
        	}
        }else if(!strcmp(token, "POLYGONSIZE")){
        	if (!(token = strtok(0, delimiters)) || !sscanf(token, "%d", &outerSize)){
        		printf("PolygonSize: expected integer, got \"%s\"!\n", token);
        		exit(13);
        	}
        }else if(!strcmp(token, "STARTSIZE")){
        	if (!(token = strtok(0, delimiters)) || !sscanf(token, "%d", &initialSize)){
        		printf("StartSize: expected integer, got \"%s\"!\n", token);
        		exit(13);
        	}
        }else if(!strcmp(token, "SEED")){
        	if (!(token = strtok(0, delimiters)) || !sscanf(token, "%d", &seed)){
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
        }
	}

	file.close();

	printf("successful\n");
}

void Settings::printSettings(){

}

// cast string to char*
char* Settings::stringToChar(std::string str){
	char * writable = new char[str.size() + 1];
	strcpy(writable, str.c_str());

	return writable;
}

// Reads booleans
bool Settings::readBoolean(bool &found){
    bool out;
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
void Settings::checkSettings(){

	if(outerSize < 3){
		printf("The polygon must have at least 3 vertices, given number %d\n", outerSize);
		exit(14);
	}

	if(outerSize < initialSize){
		printf("The size of the start polygon is not allowed to exceed the target size.\n");
		printf("Given start size: %d Given target size: %d\n", initialSize, outerSize);
		exit(14);
	}
}
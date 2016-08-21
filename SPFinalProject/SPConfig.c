/*
 * SPConfig.c
 *
 *  Created on: 21 ���� 2016
 *      Author: DorAlt
 */
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include "SPConfig.h"
typedef struct sp_config_t {
	char* spImagesDirectory;
	char* spImagesPrefix;
	char* spImagesSuffix;
	int spNumOfImages;
	int spPCADimension = 20;
	char* spPCAFileName = "pca.yml";
	int spNumOfFeatures = 100;
	bool spExtractionMode = true;
	int spNumOfSimilarImages = 1;
	enum spKDTreeSplitMethod = MAX_SPREAD;
	int spKNN = 1;
	bool spMinimalGUI = false;
	int spLoggerLevel = 3;
	char* spLoggerFilename = "stdout";
};
/**
 * Creates a new system configuration struct. The configuration struct
 * is initialized based on the configuration file given by 'filename'.
 *
 * @param filename - the name of the configuration file
 * @assert msg != NULL
 * @param msg - pointer in which the msg returned by the function is stored
 * @return NULL in case an error occurs. Otherwise, a pointer to a struct which
 * 		   contains all system configuration.
 *
 * The resulting value stored in msg is as follow:
 * - SP_CONFIG_INVALID_ARGUMENT - if filename == NULL
 * - SP_CONFIG_CANNOT_OPEN_FILE - if the configuration file given by filename cannot be open
 * - SP_CONFIG_ALLOC_FAIL - if an allocation failure occurred
 * - SP_CONFIG_INVALID_INTEGER - if a line in the config file contains invalid integer
 * - SP_CONFIG_INVALID_STRING - if a line in the config file contains invalid string
 * - SP_CONFIG_MISSING_DIR - if spImagesDirectory is missing
 * - SP_CONFIG_MISSING_PREFIX - if spImagesPrefix is missing
 * - SP_CONFIG_MISSING_SUFFIX - if spImagesSuffix is missing
 * - SP_CONFIG_MISSING_NUM_IMAGES - if spNumOfImages is missing
 * - SP_CONFIG_SUCCESS - in case of success
 *
 *
 */
#define BUFSIZE
SPConfig spConfigCreate(const char* filename, SP_CONFIG_MSG* msg){
	assert(msg!=NULL);
	char* string=NULL;
    FILE* fp
	int n=0,i=0;
	if(filename == NULL)
		return SP_CONFIG_INVALID_ARGUMENT;
    fp = fopen("argv1","r");
    if(fp==NULL)
    	return SP_CONFIG_CANNOT_OPEN_FILE;
    string = (char*)calloc(BUFSIZE,1);
    if(string == NULL)
    	return SP_CONFIG_ALLOC_FAIL;
    while((n = fread(string,1, BUFSIZE,fp))>0){
    	trim(*string);
    	if (string[0]!="#"){
    		if(strchr(string,"#") != NULL){
    			return SP_CONFIG_INVALID_ARGUMENT;
    		}
    		if(strstr(string,"spImagesDirectory")){

    		}
    		else if(strstr(string,"spImagesPrefix")){

    		}
    		else if(strstr(string,"spImagesSuffix")){

    		}
    		else if(strstr(string,"spNumOfImages")){

    		}
    		else if(strstr(string,"spPCADimension")){

    		}
    		else if(strstr(string,"spPCAFileName")){

    		}
    		else if(strstr(string,"spNumOfFeatures")){

    		}
    		else if(strstr(string,"spKDTreeSplitMethod")){

    		}
    		else if(strstr(string,"spKNN")){

    		}
    		else if(strstr(string,"spMinimalGUI")){

    		}
    		else if(strstr(string,"spLoggerLevel")){

    		}
    		else if(strstr(string,"spLoggerFilename")){

    		}
    		else if(strstr(string,"spNumOfSimilarImages")){

    		}
    		else if(strstr(string,"spExtractionMode")){

    		}
    	}
    }
}

void trim(char* String)
{
    int dest;
    int src=0;
    int len = strlen(String);

    // Advance src to the first non-whitespace character.
    while(isspace(String[src])) src++;

    // Copy the string to the "front" of the buffer
    for(dest=0; src<len; dest++, src++)
    {
        String[dest] = String[src];
    }

    // Working backwards, set all trailing spaces to NULL.
    for(dest=len-1; isspace(String[dest]); --dest)
    {
        String[dest] = '\0';
    }
}

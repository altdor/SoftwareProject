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
	int spPCADimension;
	char* spPCAFileName;
	int spNumOfFeatures;
	bool spExtractionMode;
	int spNumOfSimilarImages;
	int spKNN;
	bool spMinimalGUI;
	int spLoggerLevel;
	char* spLoggerFilename;
}SP_CONFIG;
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
	SP_CONFIG cnfg = (SP_CONFIG)malloc(sizeof(*cnfg));
	char input[4];
	char* string=NULL;
    FILE* fp
	int n=0,i=0;
    dfult(cnfg);
	if(filename == NULL)
		return SP_CONFIG_INVALID_ARGUMENT;
    fp = fopen("argv1","r");
    if(fp==NULL)
    	return SP_CONFIG_CANNOT_OPEN_FILE;
    string = (char*)calloc(BUFSIZE,1);
    if(string == NULL)
    	return SP_CONFIG_ALLOC_FAIL;
    while((n = fread(string,1, BUFSIZE,fp))>0){
    	int start;
    	trim(*string);
    	if (string[0]!="#"){
    		if(strchr(string,"#") != NULL){
    			return SP_CONFIG_INVALID_ARGUMENT;
    		}
    		start = strchr(string,"=") + 1
    		if(strstr(string,"spImagesDirectory")){
    			cnfg->spImagesDirectory = (char*)malloc(n-start*sizeof(char));
    			strncpy(cnfg->spImagesDirectory, string+start, n-start);
    			trim(*cnfg->spImagesDirectory);
    			input[0]=1;
    			if(n-start ==0 || strchr(cnfg->spImagesDirectory," ")!=NULL){
    				return SP_CONFIG_INVALID_STRING
    			}
    		}
    		else if(strstr(string,"spImagesPrefix")){
    			cnfg->spImagesPrefix = (char*)malloc(n-start*sizeof(char));
    			strncpy(cnfg->spImagesPrefix, string+start, n-start);
    			trim(*cnfg->spImagesPrefix);
    			input[1]=1;
    			if(n-start ==0 || strchr(cnfg->spImagesPrefix," ")!=NULL){
    			    return SP_CONFIG_INVALID_STRING
    			}
    		}
    		else if(strstr(string,"spImagesSuffix")){
    			cnfg->spImagesSuffix = (char*)malloc(n-start*sizeof(char));
    			strncpy(cnfg->spImagesSuffix, string+start, n-start);
    			trim(*cnfg->spImagesSuffix);
    			input[2]=1;
    			if(n-start ==0 || strchr(cnfg->spImagesSuffix," ")!=NULL){
    				return SP_CONFIG_INVALID_STRING
    			}
    		}
    		else if(strstr(string,"spNumOfImages")){
    			char* temp = (char*)malloc(n-start*sizeof(char))

    			strncpy(temp, string+start, n-start);
    			trim(*temp);
    			cnfg->spImagesSuffix = atoi(temp);
    			if(n-start ==0 || cnfg->spImagesSuffix<=0){
    			    	return SP_CONFIG_INVALID_INTEGER
    			 }

    			input[3]=1;
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
    		else{
    			return SP_CONFIG_INVALID_STRING;
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

void dfult(SP_CONFIG confg){
		confg->spPCADimension = 20
		confg->spPCAFileName = (char*)malloc(BUFSIZE)
		confg->spPCAFileName = "pca.yml";
		confg->spNumOfFeatures = 100;
		confg->spExtractionMode = true;
		confg->spNumOfSimilarImages = 1;
		enum SP_KDTREE_SPLIT_METHOD = MAX_SPREAD;
		confg->spKNN = 1;
		confg->spMinimalGUI = false;
		confg->spLoggerLevel = 3;
		confg->spLoggerFilename = (char*)malloc(BUFSIZE)
		confg->spLoggerFilename = "stdout";
}

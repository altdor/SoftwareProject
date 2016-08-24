/*
 * SPConfig.c
 *
 *  Created on: 21 ���� 2016
 *      Author: DorAlt
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

#include "SPConfig.h"
struct sp_config_t{
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
	SP_KDTREE_SPLIT_METHOD splitMethod;
};

#define BUFSIZE 1024
SPConfig spConfigCreate(const char* filename, SP_CONFIG_MSG* msg){
	assert(msg!=NULL);
	SPConfig cnfg = (SPConfig)malloc(sizeof(*cnfg));
	int* input[4];
	int len;
	char* string;
    FILE* fp;
	int n=0;
    dfult(cnfg);
	if(filename == NULL){
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return NULL;
	}
    fp = fopen(filename,"r");
    if(fp==NULL){
    	*msg= SP_CONFIG_CANNOT_OPEN_FILE;
    	return NULL;
    }
    string = (char*)calloc(BUFSIZE,1);
    if(string == NULL){
    	*msg = SP_CONFIG_ALLOC_FAIL;
    	return NULL;
    }
    while((n = fread(string, 1, BUFSIZE, fp))>0){
    	char* start;
    	trim(string);
    	if (!strcmp(string[0],'#')){
    		if(strchr(string,'#') != NULL){
    			*msg = SP_CONFIG_INVALID_ARGUMENT;
    			return NULL;
    		}
    		start = strchr(string,'=');
    		start++;
    		len = n-strlen(start);
    		if(strstr(string,"spImagesDirectory")){
    			cnfg->spImagesDirectory = (char*)malloc(len*sizeof(char));
    			strncpy(cnfg->spImagesDirectory, start, len);
    			trim(*cnfg->spImagesDirectory);
    			input[0]=1;
    			if(len ==0 || strchr(cnfg->spImagesDirectory,' ')!=NULL){
    				spConfigDestroy(cnfg);
    				*msg= SP_CONFIG_INVALID_STRING;
    				return NULL;
    			}
    		}
    		else if(strstr(string,"spImagesPrefix")){
    			cnfg->spImagesPrefix = (char*)malloc(len*sizeof(char));
    			strncpy(cnfg->spImagesPrefix, start, len);
    			trim(cnfg->spImagesPrefix);
    			input[1]=1;
    			if(len ==0 || strchr(cnfg->spImagesPrefix,' ')!=NULL){
    				spConfigDestroy(cnfg);
    			    *msg = SP_CONFIG_INVALID_STRING;
    			    return NULL;
    			}
    		}
    		else if(strstr(string,"spImagesSuffix")){
    			cnfg->spImagesSuffix = (char*)malloc(len*sizeof(char));
    			strncpy(cnfg->spImagesSuffix, start, len);
    			trim(cnfg->spImagesSuffix);
    			input[2]=1;
    			if(len ==0 || strchr(cnfg->spImagesSuffix,' ')!=NULL){
    				spConfigDestroy(cnfg);
    				*msg = SP_CONFIG_INVALID_STRING;
    				return NULL;
    			}
    		}
    		else if(strstr(string,"spNumOfImages")){
    			char* temp = (char*)malloc(len*sizeof(char));

    			strncpy(temp, start, len);
    			trim(temp);
    			cnfg->spImagesSuffix = atoi(temp);
    			if(len ==0 || cnfg->spImagesSuffix<=0){
    				spConfigDestroy(cnfg);
    				free(temp);
    			    *msg = SP_CONFIG_INVALID_INTEGER;
    			    return NULL;
    			 }
    			free(temp);
    			input[3]=1;
    		}
    		else if(strstr(string,"spPCADimension")){
    			char* temp = (char*)malloc(len*sizeof(char));
    			strncpy(temp, start, len);
    			trim(temp);
    			cnfg->spPCADimension = atoi(temp);
    			if(len ==0 || cnfg->spPCADimension<10 || cnfg->spPCADimension>28){
    			    free(temp);
    			    spConfigDestroy(cnfg);
    				*msg = SP_CONFIG_INVALID_INTEGER;
    				return NULL;
    			}
    			free(temp);
    		}
    		else if(strstr(string,"spPCAFileName")){
    			cnfg->spPCAFileName = (char*)malloc(len*sizeof(char));
    			strncpy(cnfg->spPCAFileName, start, len);
    			trim(cnfg->spPCAFileName);
    			if(len ==0 || strchr(cnfg->spPCAFileName,' ')!=NULL){
    				spConfigDestroy(cnfg);
    			    *msg =SP_CONFIG_INVALID_STRING;
    			    return NULL;
    			}
    		}
    		else if(strstr(string,"spNumOfFeatures")){
    			char* temp = (char*)malloc(len*sizeof(char));
    			strncpy(temp, start, len);
    			trim(temp);
    			cnfg->spNumOfFeatures = atoi(temp);
    			if(len ==0 || cnfg->spNumOfFeatures<=0){
    				spConfigDestroy(cnfg);
    				free(temp);
    			   	*msg = SP_CONFIG_INVALID_INTEGER;
    			   	return NULL;
    			}
    			free(temp);
    		}
    		else if(strstr(string,"spKDTreeSplitMethod")){
    			char* temp = (char*)malloc(len*sizeof(char));
    			strncpy(temp, start, len);
    			trim(temp);
    			if(strcmp(temp,"MAX_SPREAD")){
    				cnfg->splitMethod = MAX_SPREAD;
    			}
    			else if(strcmp(temp,"RANDOM")){
    				cnfg->splitMethod = RANDOM;
    			}
    			else if(strcmp(temp,"INCREMENTAL")){
    				cnfg->splitMethod = INCREMENTAL;
    			}
    			else{
    				spConfigDestroy(cnfg);
    				free(temp);
    				*msg = SP_CONFIG_INVALID_STRING;
    				return NULL;
    			}
    			free(temp);
    		}
    		else if(strstr(string,"spKNN")){
    			char* temp = (char*)malloc(len*sizeof(char));
    			strncpy(temp, start, len);
    			trim(temp);
    			cnfg->spKNN = atoi(temp);
    			if(len ==0 || cnfg->spKNN<=0){
    				spConfigDestroy(cnfg);
    				free(temp);
    			 	*msg = SP_CONFIG_INVALID_INTEGER;
    			 	return NULL;
    			}
    		}
    		else if(strstr(string,"spMinimalGUI")){
    			char* temp = (char*)malloc(len*sizeof(char));
    			strncpy(temp, start, len);
    			trim(temp);
    			if(strcmp(temp,"false")){
    			 	cnfg->spMinimalGUI = false;
    			}
    			else if(strcmp(temp,"true")){
    				cnfg->spMinimalGUI = true;
    			}
    			else{
    				spConfigDestroy(cnfg);
    			 	free(temp);
    			 	*msg = SP_CONFIG_INVALID_STRING;
    			 	return NULL;
    			 }
       			free(temp);

    		}
    		else if(strstr(string,"spLoggerLevel")){
    			char* temp = (char*)malloc(len*sizeof(char));
       			strncpy(temp, start, len);
       			trim(temp);
    			cnfg->spLoggerLevel = atoi(temp);
    			if(len ==0 || cnfg->spLoggerLevel<1 || cnfg->spLoggerLevel>4){
    				spConfigDestroy(cnfg);
    				free(temp);
   			    	*msg = SP_CONFIG_INVALID_INTEGER;
   			    	return NULL;
    			}
    			free(temp);
    		}
    		else if(strstr(string,"spLoggerFilename")){
    			cnfg->spLoggerFilename = (char*)malloc(len*sizeof(char));
       			strncpy(cnfg->spLoggerFilename, start, len);
       			trim(cnfg->spLoggerFilename);
       			if(len ==0 || (strchr(cnfg->spLoggerFilename,' ')!=NULL)){
       				spConfigDestroy(cnfg);
       			    *msg = SP_CONFIG_INVALID_STRING;
       			    return NULL;
       			}
    		}
    		else if(strstr(string,"spNumOfSimilarImages")){
    			char* temp = (char*)malloc(len*sizeof(char));
       			strncpy(temp, start, len);
       			trim(temp);
       			cnfg->spNumOfSimilarImages = atoi(temp);
       			if(len ==0 || cnfg->spNumOfSimilarImages<=0){
       				spConfigDestroy(cnfg);
       				free(temp);
       			 	*msg= SP_CONFIG_INVALID_INTEGER;
       			 	return NULL;
       			}
       			free(temp);
    		}
    		else if(strstr(string,"spExtractionMode")){
    			char* temp = (char*)malloc(len*sizeof(char));
    			strncpy(temp, start, len);
    			trim(temp);
    			if(strcmp(temp,"false")){
    				cnfg->spExtractionMode = false;
    			}
    			else if(strcmp(temp,"true")){
  	  				cnfg->spExtractionMode = true;
       			}
    			else{
    				spConfigDestroy(cnfg);
    				free(temp);
    				*msg= SP_CONFIG_INVALID_STRING;
    				return NULL;
       			}
       			free(temp);
    		}
    		else{
    			spConfigDestroy(cnfg);
    			*msg = SP_CONFIG_INVALID_STRING;
    			return NULL;
    		}
    	}
    }
    *msg = checkinput(input);
    if(*msg == SP_CONFIG_SUCCESS){
    	return cnfg;
    }
    spConfigDestroy(cnfg);
    return NULL;
}
SP_CONFIG_MSG checkinput(int* input[4]){
	if(input[0] == 0){
		return SP_CONFIG_MISSING_DIR;
	}
	if(input[1]==0){
		return SP_CONFIG_MISSING_PREFIX;
	}
	if(input[2]==0){
		return SP_CONFIG_MISSING_SUFFIX;
	}
	if(input[3]==0){
		return SP_CONFIG_MISSING_NUM_IMAGES;
	}
	return SP_CONFIG_SUCCESS;
}
void trim(char* string){
    int dest;
    int src=0;
    int len = strlen(string);

    // Advance src to the first non-whitespace character.
    while(isspace(string[src])) src++;

    // Copy the string to the "front" of the buffer
    for(dest=0; src<len; dest++, src++)
    {
        string[dest] = string[src];
    }

    // Working backwards, set all trailing spaces to NULL.
    for(dest=len-1; isspace(string[dest]); --dest)
    {
    	string[dest] = '\0';
    }
}

void dfult(SPConfig confg){
		confg->spPCADimension = 20;
		confg->spPCAFileName = (char*)malloc((BUFSIZE)*sizeof(char));
		confg->spPCAFileName = "pca.yml";
		confg->spNumOfFeatures = 100;
		confg->spExtractionMode = true;
		confg->spNumOfSimilarImages = 1;
		confg->splitMethod = MAX_SPREAD;
		confg->spKNN = 1;
		confg->spMinimalGUI = false;
		confg->spLoggerLevel = 3;
		confg->spLoggerFilename = (char*)malloc((BUFSIZE)*sizeof(char));
		confg->spLoggerFilename = "stdout";
}
void spConfigDestroy(SPConfig cnfg){
	free(cnfg->spImagesDirectory);
	free(cnfg->spImagesPrefix);
	free(cnfg->spImagesSuffix);
	free(cnfg->spPCAFileName);
	free(cnfg->spLoggerFilename);
	free(cnfg->spImagesPrefix);
	free(cnfg);
}

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
};

#define BUFSIZE
SPConfig spConfigCreate(const char* filename, SP_CONFIG_MSG* msg){
	assert(msg!=NULL);
	SPConfig cnfg = (SPConfig)malloc(sizeof(*cnfg));
	char input[4];
	char* string=NULL;
    FILE* fp;
	int n=0;
	int i=0;
    dfult(cnfg);
	if(filename == NULL){
		msg= SP_CONFIG_INVALID_ARGUMENT;
		return NULL;
	}
    fp = fopen("argv1","r");
    if(fp==NULL){
    	msg= SP_CONFIG_CANNOT_OPEN_FILE;
    	return NULL;
    }
    string = (char*)calloc(BUFSIZE,1);
    if(string == NULL){
    	msg = SP_CONFIG_ALLOC_FAIL;
    	return NULL;
    }
    while((n = fread(string,1, BUFSIZE,fp))>0){
    	int start;
    	trim(*string);
    	if (string[0]!="#"){
    		if(strchr(string,"#") != NULL){
    			msg = SP_CONFIG_INVALID_ARGUMENT;
    			return NULL;
    		}
    		start = strchr(string,"=") + 1
    		if(strstr(string,"spImagesDirectory")){
    			cnfg->spImagesDirectory = (char*)malloc(n-start*sizeof(char));
    			strncpy(cnfg->spImagesDirectory, string+start, n-start);
    			trim(*cnfg->spImagesDirectory);
    			input[0]=1;
    			if(n-start ==0 || strchr(cnfg->spImagesDirectory," ")!=NULL){
    				spConfigDestroy(cnfg);
    				msg= SP_CONFIG_INVALID_STRING;
    				return NULL;
    			}
    		}
    		else if(strstr(string,"spImagesPrefix")){
    			cnfg->spImagesPrefix = (char*)malloc(n-start*sizeof(char));
    			strncpy(cnfg->spImagesPrefix, string+start, n-start);
    			trim(*cnfg->spImagesPrefix);
    			input[1]=1;
    			if(n-start ==0 || strchr(cnfg->spImagesPrefix," ")!=NULL){
    				spConfigDestroy(cnfg);
    			    msg = SP_CONFIG_INVALID_STRING;
    			    return NULL;
    			}
    		}
    		else if(strstr(string,"spImagesSuffix")){
    			cnfg->spImagesSuffix = (char*)malloc(n-start*sizeof(char));
    			strncpy(cnfg->spImagesSuffix, string+start, n-start);
    			trim(*cnfg->spImagesSuffix);
    			input[2]=1;
    			if(n-start ==0 || strchr(cnfg->spImagesSuffix," ")!=NULL){
    				spConfigDestroy(cnfg);
    				msg = SP_CONFIG_INVALID_STRING;
    				return NULL;
    			}
    		}
    		else if(strstr(string,"spNumOfImages")){
    			char* temp = (char*)malloc(n-start*sizeof(char))

    			strncpy(temp, string+start, n-start);
    			trim(*temp);
    			cnfg->spImagesSuffix = atoi(temp);
    			if(n-start ==0 || cnfg->spImagesSuffix<=0){
    				spConfigDestroy(cnfg);
    				free(temp);
    			    msg = SP_CONFIG_INVALID_INTEGER;
    			    return NULL;
    			 }
    			free(temp);
    			input[3]=1;
    		}
    		else if(strstr(string,"spPCADimension")){
    			char* temp = (char*)malloc(n-start*sizeof(char))
    			strncpy(temp, string+start, n-start);
    			trim(*temp);
    			cnfg->spPCADimension = atoi(temp);
    			if(n-start ==0 || cnfg->spPCADimension<10 || cnfg->spPCADimension>28){
    			    free(temp);
    			    spConfigDestroy(cnfg);
    				msg = SP_CONFIG_INVALID_INTEGER;
    				return NULL;
    			}
    			free(temp);
    		}
    		else if(strstr(string,"spPCAFileName")){
    			cnfg->spPCAFileName = (char*)malloc(n-start*sizeof(char));
    			strncpy(cnfg->spPCAFileName, string+start, n-start);
    			trim(*cnfg->spPCAFileName);
    			if(n-start ==0 || strchr(cnfg->spPCAFileName," ")!=NULL){
    				spConfigDestroy(cnfg);
    			    msg =SP_CONFIG_INVALID_STRING;
    			    return NULL;
    			}
    		}
    		else if(strstr(string,"spNumOfFeatures")){
    			char* temp = (char*)malloc(n-start*sizeof(char))
    			strncpy(temp, string+start, n-start);
    			trim(*temp);
    			cnfg->spNumOfFeatures = atoi(temp);
    			if(n-start ==0 || cnfg->spNumOfFeatures<=0){
    				spConfigDestroy(cnfg);
    				free(temp);
    			   	msg = SP_CONFIG_INVALID_INTEGER;
    			   	return NULL;
    			}
    			free(temp);
    		}
    		else if(strstr(string,"spKDTreeSplitMethod")){
    			char* temp = (char*)malloc(n-start*sizeof(char));
    			strncpy(temp, string+start, n-start);
    			trim(temp);
    			if(temp=="MAX_SPREAD"){
    				SP_KDTREE_SPLIT_METHOD = MAX_SPREAD;
    			}
    			else if(temp=="RANDOM"){
    				SP_KDTREE_SPLIT_METHOD = RANDOM;
    			}
    			else if(temp=="INCREMENTAL"){
    				SP_KDTREE_SPLIT_METHOD = INCREMENTAL;
    			}
    			else{
    				spConfigDestroy(cnfg);
    				free(temp);
    				msg = SP_CONFIG_INVALID_STRING;
    				return NULL;
    			}
    			free(temp);
    		}
    		else if(strstr(string,"spKNN")){
    			char* temp = (char*)malloc(n-start*sizeof(char))
    			strncpy(temp, string+start, n-start);
    			trim(*temp);
    			cnfg->spKNN = atoi(temp);
    			if(n-start ==0 || cnfg->spKNN<=0){
    				spConfigDestroy(cnfg);
    				free(temp);
    			 	msg = SP_CONFIG_INVALID_INTEGER;
    			 	return NULL;
    			}
    		}
    		else if(strstr(string,"spMinimalGUI")){
    			char* temp = (char*)malloc(n-start*sizeof(char));
    			strncpy(temp, string+start, n-start);
    			trim(temp);
    			if(temp=="false"){
    			 	cnfg->spMinimalGUI = false;
    			}
    			else if(temp=="true"){
    				cnfg->spMinimalGUI = true;
    			}
    			else{
    				spConfigDestroy(cnfg);
    			 	free(temp);
    			 	msg = SP_CONFIG_INVALID_STRING;
    			 	return NULL;
    			 }
       			free(temp);

    		}
    		else if(strstr(string,"spLoggerLevel")){
    			char* temp = (char*)malloc(n-start*sizeof(char))
       			strncpy(temp, string+start, n-start);
       			trim(*temp);
    			cnfg->spLoggerLevel = atoi(temp);
    			if(n-start ==0 || cnfg->spLoggerLevel<1 || cnfg->spLoggerLevel>4){
    				destroy(cnfg);
    				free(temp);
   			    	msg = SP_CONFIG_INVALID_INTEGER;
   			    	return NULL;
    			}
    			free(temp);
    		}
    		else if(strstr(string,"spLoggerFilename")){
    			cnfg->spLoggerFilename = (char*)malloc(n-start*sizeof(char));
       			strncpy(cnfg->spLoggerFilename, string+start, n-start);
       			trim(*cnfg->spLoggerFilename);
       			if(n-start ==0 || strchr(cnfg->spLoggerFilename," ")!=NULL){
       				spConfigDestroy(cnfg);
       			    msg = SP_CONFIG_INVALID_STRING;
       			    return NULL;
       			}
    		}
    		else if(strstr(string,"spNumOfSimilarImages")){
    			char* temp = (char*)malloc(n-start*sizeof(char))
       			strncpy(temp, string+start, n-start);
       			trim(*temp);
       			cnfg->spNumOfSimilarImages = atoi(temp);
       			if(n-start ==0 || cnfg->spNumOfSimilarImages<=0){
       				spConfigDestroy(cnfg);
       				free(temp);
       			 	msg= SP_CONFIG_INVALID_INTEGER;
       			 	return NULL;
       			}
       			free(temp);
    		}
    		else if(strstr(string,"spExtractionMode")){
    			char* temp = (char*)malloc(n-start*sizeof(char));
    			strncpy(temp, string+start, n-start);
    			trim(temp);
    			if(temp=="false"){
    				cnfg->spExtractionMode = false;
    			}
    			else if(temp=="true"){
  	  				cnfg->spExtractionMode = true;
       			}
    			else{
    				spConfigDestroy(cnfg);
    				free(temp);
    				msg= SP_CONFIG_INVALID_STRING;
    				return NULL;
       			}
       			free(temp);
    		}
    		else{
    			spConfigDestroy(cnfg);
    			msg = SP_CONFIG_INVALID_STRING;
    			return NULL;
    		}
    	}
    }
    msg = checkinput(input);
    if(msg == SP_CONFIG_SUCCESS)
    	return cnfg;
    spConfigDestroy(cnfg);
    return NULL;
}
SP_CONFIG_MSG checkinput(char input[4]){
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

void dfult(SPConfig confg){
		confg->spPCADimension = 20
		confg->spPCAFileName = (char*)malloc(BUFSIZE)
		confg->spPCAFileName = "pca.yml";
		confg->spNumOfFeatures = 100;
		confg->spExtractionMode = true;
		confg->spNumOfSimilarImages = 1;
		SP_KDTREE_SPLIT_METHOD = MAX_SPREAD;
		confg->spKNN = 1;
		confg->spMinimalGUI = false;
		confg->spLoggerLevel = 3;
		confg->spLoggerFilename = (char*)malloc(BUFSIZE)
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

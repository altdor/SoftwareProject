/*
 * MainAuc.c
 *
 *  Created on: 28 ���� 2016
 *      Author: DorAlt
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "SPConfig.h"
#include "SPPoint.h"
#include "SPLogger.h"
#define BUFSIZE 1024
#define ERROR1 "Invalid command line: use -c <config_filename>"
#define ERROR2 "The configuration file"
#define COULDNTOPEN "couldn't be open"
#define ERROR3 "The default configuration file spcbir.config couldn't be open"
#define DEFCON "spcbir.config"
bool checkFileName(const char* filename){
	FILE* fp;
	if(strstr(filename, ".")==NULL){
		printf("%s" ERROR1);
		return false;
	}
	fp = fopen(filename,"r");
	if(fp==NULL){
		if(strcmp(filename,DEFCON) == 0){
			printf("%s",ERROR3);
			return false;
		}
		else{
			printf("%s %s %s",ERROR2,filename,COULDNTOPEN);
			return false;
		}
	}
	return true;
}
bool extractToFile(char* imagePathnosuf,SPPoint* features, int numOfFeatures, int level){
	FILE* featfp;
	featfp = fopen(imagePathnosuf,"w");
	if(featfp == NULL){
		return false;
	}
	putc(numOfFeatures,featfp);
	for(int i = 0; i<numOfFeatures;i++){
		putc('\n',featfp);
		putc(spPointGetIndex(features[i]),featfp);
		putc('\n',featfp);
		putc(spPointGetDimension(features[i]),featfp);
		putc('\n',featfp);
		fwrite(spPointGetData(features[i]), spPointGetDimension(features[i]),sizeof(double),featfp);
	}
	return true;
}
SPPoint* extractFromFiles(char* imagePathnosuf, int level){
	FILE* featfp;
	char* str;
	int numOfFeatures;
	SPPoint* features;
	featfp = fopen(imagePathnosuf,"r");
	if(featfp == NULL){
		ErrorLogger(level, "cant open file", "MainAux.cpp",__func__, __LINE__);
		spLoggerDestroy();
		return NULL;
	}
	str = (char*)malloc(BUFSIZE);
	if(str == NULL){
		ErrorLogger(level, "out of memory", "MainAux.cpp",__func__, __LINE__);
		spLoggerDestroy();
		return NULL;
	}
	fgets(str,BUFSIZE ,featfp);
	numOfFeatures = atoi(str);
	free(str);
	features = (SPPoint*)malloc(sizeof(*features)*numOfFeatures);
	if(features == NULL){
		ErrorLogger(level, "out of memory", "MainAux.cpp",__func__, __LINE__);
		spLoggerDestroy();
		return NULL;
	}
	for(int i = 0; i<numOfFeatures;i++){
		int dim, index;
		double* data;
		char* str = (char*)malloc(BUFSIZE);
		if(str == NULL){
			ErrorLogger(level, "out of memory", "MainAux.cpp",__func__, __LINE__);
			spLoggerDestroy();
			return NULL;
		}
		fgets(str ,BUFSIZE, featfp);
		index = atoi(str);
		free(str);
		str = (char*)malloc(BUFSIZE);
		if(str == NULL){
			ErrorLogger(level, "out of memory", "MainAux.cpp",__func__, __LINE__);
			spLoggerDestroy();
			return NULL;
		}
		fgets(str ,BUFSIZE, featfp);
		dim = atoi(str);
		free(str);
		data = (double*)malloc(sizeof(double)*dim);
		fread(data, dim,sizeof(double),featfp);
		features[i] = (SPPoint)malloc(sizeof(SPPoint));
		if(features == NULL){
			ErrorLogger(level, "out of memory", "MainAux.cpp",__func__, __LINE__);
			spLoggerDestroy();
			return NULL;
		}
		features[i] = spPointCreate(data, dim, index);
	}
	return features;
}

void ErrorLogger(int level, const char* msg, const char* file,
		const char* function, const int line){
	if(level>0){
		assert(spLoggerPrintError(msg,file,function,line)==SP_LOGGER_SUCCESS);
		if(level>1){
			assert(spLoggerPrintWarning(msg,file,function,line)==SP_LOGGER_SUCCESS);
			if(level>2){
				assert(spLoggerPrintInfo(msg)==SP_LOGGER_SUCCESS);
				if(level==4){
					assert(spLoggerPrintDebug(msg,file,function,line)==SP_LOGGER_SUCCESS);
				}
			}
		}
	}
}

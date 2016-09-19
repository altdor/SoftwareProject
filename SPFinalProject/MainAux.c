/*
 * MainAuc.c
 *
 *  Created on: 28 баев 2016
 *      Author: DorAlt
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "SPConfig.h"
#include "KDTree.h"
#include "SPKNN.h"
#include "SPPoint.h"
#include "SPLogger.h"
#include "SPBPriorityQueue.h"
#define BUFSIZE 1024
#define ERROR1 "Invalid command line: use -c <config_filename>\n"
#define ERROR2 "The configuration file"
#define COULDNTOPEN "Couldn't be open\n"
#define ERROR3 "The default configuration file spcbir.config couldn't be open\n"
#define ERROR4 "Can't make new file"
#define ERROR5 "Can't open file"
#define ERROR6 "Out of memory"
#define ERROR7 "Missing argument"
#define ERROR8 "Allocating failed"
#define DEFCON "spcbir.config"
#define MAIN "MainAux.c"
#define BESTCAND "Best candidates for -"
#define ARE "are:"
#define READ "r"
#define WRITE "w"
#define STRING "%s"
#define STRING_DOWN "%s\n"
#define TRIPLE_STRING "%s %s %s"
#define TRIPLE_STRING_DOWN "%s %s %s\n"
#define NEXT_LINE '\n'
#define DOT "."
#define ABORT -1
#define START -1
#define INIT 0
#define FIRST 0
#define EQUAL 0

bool checkFileName(const char* filename){
	FILE* fp;
	if(strstr(filename, DOT)==NULL){
		printf(STRING,ERROR1);
		return false;
	}
	fp = fopen(filename,READ);
	if(fp==NULL){
		if(strcmp(filename,DEFCON) == EQUAL){
			printf(STRING,ERROR3);
			return false;
		}
		else{
			printf(TRIPLE_STRING,ERROR2,filename,COULDNTOPEN);
			return false;
		}
	}
	fclose(fp);
	return true;
}
void ErrorLogger(int level, char* msg, const char* file,const char* function, const int line){
	spLoggerPrintError(msg,file,function,line);
		if(level>1){
			assert(spLoggerPrintWarning(msg,file,function,line)==SP_LOGGER_SUCCESS);
			if(level>2){
				assert(spLoggerPrintInfo(msg)==SP_LOGGER_SUCCESS);
				if(level==4){
					assert(spLoggerPrintDebug(msg,file,function,line)==SP_LOGGER_SUCCESS);
				}
			}
		}else return;
}

bool extractToFile(char* imagePathnosuf,SPPoint* features, int numOfFeatures, int level){
	FILE* featfp;
	featfp = fopen(imagePathnosuf,WRITE);
	if(featfp == NULL){
		ErrorLogger(level, ERROR4, MAIN,__func__, __LINE__);
		spLoggerDestroy();
		return false;
	}
	putc(numOfFeatures,featfp);
	for(int i = 0; i<numOfFeatures;i++){
		putc(NEXT_LINE,featfp);
		putc(spPointGetIndex(features[i]),featfp);
		putc(NEXT_LINE,featfp);
		putc(spPointGetDimension(features[i]),featfp);
		putc(NEXT_LINE,featfp);
		fwrite(spPointGetData(features[i]), spPointGetDimension(features[i]),sizeof(double),featfp);
	}
	fclose(featfp);
	return true;
}
int getNumOfFeaturesForImage(char* imagePathnosuf, int level){
	FILE* featfp;
	char* str;
	int numOfFeatures;
	featfp = fopen(imagePathnosuf,READ);
	if(featfp == NULL){
		ErrorLogger(level, ERROR5, MAIN,__func__, __LINE__);
		spLoggerDestroy();
		return ABORT;
	}
	str = (char*)malloc(BUFSIZE);
	if(str == NULL){
		ErrorLogger(level, ERROR6, MAIN,__func__, __LINE__);
		spLoggerDestroy();
		fclose(featfp);
		return ABORT;
	}
	fgets(str,BUFSIZE ,featfp);
	numOfFeatures = atoi(str);
	free(str);
	fclose(featfp);
	return numOfFeatures;
}
SPPoint* extractFromFiles(char* imagePathnosuf, int level, int* Features){
	FILE* featfp;
	char* str;
	int numOfFeatures;
	SPPoint* features;
	featfp = fopen(imagePathnosuf,READ);
	if(featfp == NULL){
		ErrorLogger(level, ERROR5, MAIN,__func__, __LINE__);
		spLoggerDestroy();
		return NULL;
	}
	str = (char*)malloc(BUFSIZE);
	if(str == NULL){
		ErrorLogger(level, ERROR6, MAIN,__func__, __LINE__);
		spLoggerDestroy();
		fclose(featfp);
		return NULL;
	}
	fgets(str,BUFSIZE ,featfp);
	numOfFeatures = atoi(str);
	*Features = numOfFeatures;
	free(str);
	features = (SPPoint*)malloc(sizeof(*features)*numOfFeatures);
	if(features == NULL){
		ErrorLogger(level, ERROR6, MAIN,__func__, __LINE__);
		spLoggerDestroy();
		return NULL;
	}
	for(int i = 0; i<numOfFeatures;i++){
		int dim, index;
		double* data;
		char* str = (char*)malloc(BUFSIZE);
		if(str == NULL){
			ErrorLogger(level, ERROR6, MAIN,__func__, __LINE__);
			spLoggerDestroy();
			fclose(featfp);
			return NULL;
		}
		fgets(str ,BUFSIZE, featfp);
		index = atoi(str);
		free(str);
		str = (char*)malloc(BUFSIZE);
		if(str == NULL){
			ErrorLogger(level, ERROR6, MAIN,__func__, __LINE__);
			spLoggerDestroy();
			fclose(featfp);
			return NULL;
		}
		fgets(str ,BUFSIZE, featfp);
		dim = atoi(str);
		free(str);
		data = (double*)malloc(sizeof(double)*dim);
		fread(data, dim,sizeof(double),featfp);
		features[i] = spPointCreate(data, dim, index);
		if(features == NULL){
			ErrorLogger(level, ERROR6, MAIN,__func__, __LINE__);
			spLoggerDestroy();
			fclose(featfp);
			return NULL;
		}
		free(data);
	}
	fclose(featfp);
	return features;
}
int maxIndex(int* count, int size){
	int max=count[FIRST],index=INIT,i;
	for (i=0; i<size; i++){
		if (count[i]>max){
			max = count[i];
			index = i;
		}
	}
	return index;
}
void notMinimalGui(int* arr, char* imgpath, SPConfig config,int size){
	int index;
	int numOfSimilarImages = GetspNumOfSimilarImages(config);
	if(numOfSimilarImages == ABORT||arr==NULL ||imgpath==NULL || size<=0){
		ErrorLogger(GetSpLoggerLevel(config), ERROR7, MAIN,__func__, __LINE__);
		spLoggerDestroy();
		spConfigDestroy(config);
		return;
	}
	printf(TRIPLE_STRING_DOWN,BESTCAND, imgpath, ARE);
	fflush(NULL);
	for(int i=0;i<numOfSimilarImages;i++){
		index = maxIndex(arr,size);
		arr[index]=START;
		char* path = (char*)malloc(BUFSIZE);
		if(path == NULL){
			ErrorLogger(GetSpLoggerLevel(config), ERROR8, MAIN,__func__, __LINE__);
			spLoggerDestroy();
			free(path);
			spConfigDestroy(config);
			return;
		}
		spConfigGetImagePath(path,config,index);
		printf(STRING_DOWN,path);
		fflush(NULL);
		free(path);
	}
}
int* kNearest(KDTreeNode tree, SPPoint* features, SPConfig config, int size){
	int i,index;
	int numOfSimilarImages = GetspNumOfSimilarImages(config);
	SP_CONFIG_MSG* msg=(SP_CONFIG_MSG*)malloc(sizeof(SP_CONFIG_MSG));
	int* count;
	if(msg == NULL){
		ErrorLogger(GetSpLoggerLevel(config), ERROR8, MAIN,__func__, __LINE__);
		spLoggerDestroy();
		spConfigDestroy(config);
		return NULL;
	}
	if (tree == NULL || features == NULL || config == NULL ||size<=0)
		return NULL;
	count = (int*)malloc(spConfigGetNumOfImages(config,msg)*sizeof(int));
	if(count == NULL){
		ErrorLogger(GetSpLoggerLevel(config), ERROR8, MAIN,__func__, __LINE__);
		spLoggerDestroy();
		spConfigDestroy(config);
		free(msg);
		return NULL;
	}
	for(i=0; i<spConfigGetNumOfImages(config,msg); i++){
		count[i]=INIT;
	}
	for (i=0; i<size; i++){
		SPKNN knn = spKinit(GetSpKNN(config));
		SPBPQueue bpq = GetKnnBpq(knn);
		kNearestNeighbors(tree, bpq, features[i]);
		for(int j=0;j<numOfSimilarImages;j++){
			SPListElement elm = spBPQueuePeek(bpq);
			index = spListElementGetIndex(elm);
			spListElementDestroy(elm);
			count[index]++;
			spBPQueueDequeue(bpq);
		}
		SPKNNDestroy(knn);
	}
	free(msg);
	return count;
}


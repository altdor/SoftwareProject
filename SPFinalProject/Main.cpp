/*
 * main.cpp

 *
 *  Created on: 21 ���� 2016
 *      Author: DorAlt
 */


#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include "spImageProc.h"
extern "C"{
#include "SPConfig.h"
#include "SPPoint.h"
#include "MainAux.h"
#include "SPKDArray.h"
#include "SPKNN.h"
#include "KDTree.h"
#include "SPLogger.h"
#include "SPBPriorityQueue.h"
}

#define BUFSIZE 1024
#define DEFCON "spcbir.config"
#define FEATSUF ".feats"
using namespace sp;
int main(int argc, const char* argv[]){
	int i,j;
	int* numOfFeatures=0;
	SPPoint* featuresArr;
	SPPoint** allImgFeaters;
	SPConfig config;
	SPKDArray kdarr;
	KDTreeNode kdtree;
	int level;
	int numOfImages;
	SP_CONFIG_MSG* msg;
	SPPoint* features;
	char* img;
	char* filename = (char*)malloc(BUFSIZE);
	if(filename == NULL){
		free(filename);
		return -1;
	}
	img = (char*)malloc(BUFSIZE);
	if(img ==NULL){
		free(filename);
		free(img);
		return -1;
	}
	strcpy(filename,DEFCON);
	for(int i=0; i<argc;i++){
		if(strcmp(argv[i],"-c")==0){
			if(i+1<argc)
				strcpy(filename,argv[i+1]);
		}
	}
	if(!checkFileName(filename)){
		return -1;
	}
	config = spConfigCreate(filename,msg);
	if(*msg != SP_CONFIG_SUCCESS){
		free(img);
		free(filename);
		return -1;
	}
	level = GetSpLoggerLevelNum(config);
	assert(spLoggerCreate(GetSpLoggerFilename(config),GetSpLoggerLevel(config)) == SP_LOGGER_SUCCESS);
	if(spConfigIsExtractionMode(config,msg)){
		if(*msg != SP_CONFIG_SUCCESS){
			ErrorLogger(level, "INVAlID ARGUMENT", "Main.cpp",__func__, __LINE__);
			spLoggerDestroy();
			free(img);
			free(filename);
			return -1;
		}
		numOfImages= spConfigGetNumOfImages(config,msg);
		if(*msg != SP_CONFIG_SUCCESS){
			ErrorLogger(level, "INVAlID ARGUMENT", "Main.cpp",__func__, __LINE__);
			spLoggerDestroy();
			free(img);
			free(filename);
			return -1;
		}
		for(int i =0;i<numOfImages;i++){
			char* imagePath = (char*)malloc(BUFSIZE*3+sizeof(int));
			char* imagePathnosuf;
			if(imagePath == NULL){
				ErrorLogger(level, "OUT OF MEMORY", "Main.cpp",__func__, __LINE__);
				spLoggerDestroy();
				free(img);
				free(imagePath);
				spConfigDestroy(config);
				return -1;
			}
			imagePathnosuf = (char*)malloc(BUFSIZE*3+sizeof(int));
			if(imagePathnosuf == NULL){
				ErrorLogger(level, "OUT OF MEMORY", "Main.cpp",__func__, __LINE__);
				spLoggerDestroy();
				free(img);
				free(imagePath);
				free(imagePathnosuf);
				spConfigDestroy(config);
				return -1;
			}
			ImageProc* imagep = new ImageProc(config);
			*msg =spConfigGetImagePath(imagePath, config, i);
			if(*msg != SP_CONFIG_SUCCESS){
				ErrorLogger(level, "Invalid argument", "Main.cpp",__func__, __LINE__);
				spLoggerDestroy();
				free(img);
				free(imagePath);
				free(imagePathnosuf);
				free(filename);
				return -1;
			}
			features = imagep->getImageFeatures(imagePath, i,numOfFeatures);
			allImgFeaters = (SPPoint**)malloc((*numOfFeatures)*numOfImages*sizeof(SPPoint*));
			allImgFeaters[i] = features;
			*msg =spConfigGetImagePathWithoutSuffix(imagePathnosuf, config, i);
			if(*msg != SP_CONFIG_SUCCESS){
				ErrorLogger(level, "Invalid argument", "Main.cpp",__func__, __LINE__);
				spLoggerDestroy();
				free(imagePath);
				free(imagePathnosuf);
				free(filename);
				return -1;
			}
			strcat(imagePathnosuf,FEATSUF);
			if(!extractToFile(imagePathnosuf, features, *numOfFeatures, level)){
				spConfigDestroy(config);
				spLoggerDestroy();
				free(imagePath);
				free(imagePathnosuf);
				free(filename);
				return -1;
			}
			free(imagePath);
			free(imagePathnosuf);
		}

	}
	else{
		char* imagePathnosuf;
		numOfImages= spConfigGetNumOfImages(config,msg);
		if(*msg != SP_CONFIG_SUCCESS){
			ErrorLogger(level, "Invalid argument", "Main.cpp",__func__, __LINE__);
			spLoggerDestroy();
			free(filename);
			return -1;
		}
		imagePathnosuf = (char*)malloc(BUFSIZE*3+sizeof(int));
		if(imagePathnosuf == NULL){
			ErrorLogger(level, "out of memory", "Main.cpp",__func__, __LINE__);
			spLoggerDestroy();
			free(imagePathnosuf);
			free(filename);
			spConfigDestroy(config);
			return -1;
		}
		numOfImages= spConfigGetNumOfImages(config,msg);
		if(*msg != SP_CONFIG_SUCCESS){
			ErrorLogger(level, "Invalid argument", "Main.cpp",__func__, __LINE__);
			spLoggerDestroy();
			free(imagePathnosuf);
			free(filename);
			return -1;
		}
		allImgFeaters = (SPPoint**)malloc(numOfImages*sizeof(allImgFeaters));
		if(allImgFeaters == NULL){
			ErrorLogger(level, "out of memory", "Main.cpp",__func__, __LINE__);
			spLoggerDestroy();
			spConfigDestroy(config);
			free(imagePathnosuf);
			free(filename);
			free(allImgFeaters);
			return -1;
		}
		for(int i=0;i<numOfImages;i++){
			*msg =spConfigGetImagePathWithoutSuffix(imagePathnosuf, config, i);
			if(*msg != SP_CONFIG_SUCCESS){
				ErrorLogger(level, "Invalid argument", "Main.cpp",__func__, __LINE__);
				spLoggerDestroy();
				free(imagePathnosuf);
				free(filename);
				free(allImgFeaters);
				return -1;
			}
			strcat(imagePathnosuf,FEATSUF);

			allImgFeaters[i] = extractFromFiles(imagePathnosuf, level);
			if(allImgFeaters[i] == NULL){
				spConfigDestroy(config);
				for(int j=0;j<=i;j++){
					free(allImgFeaters[j]);
				}
				spLoggerDestroy();
				free(imagePathnosuf);
				free(filename);
				free(allImgFeaters);
				return -1;
			}
			free(imagePathnosuf);
		}

	}
	featuresArr = (SPPoint*)malloc((*numOfFeatures)*numOfImages*sizeof(SPPoint));
	for (i=0; i<numOfImages; i++){
		for(j=0; j<(*numOfFeatures); j++){
			featuresArr[i*j+j] = allImgFeaters[i][j];
		}
		free(allImgFeaters[i]);
	}
	free(allImgFeaters);
	kdarr = (SPKDArray)malloc(sizeof(SPKDArray));
	kdtree = (KDTreeNode)malloc(sizeof(KDTreeNode));
	kdarr = spKdarrayInit(featuresArr, (*numOfFeatures)*numOfImages);
	kdtree = buildKDTree(kdarr, GetSplitMethod(config),0);
	printf("Please enter an image path:/n");
	fflush(NULL);
	scanf("%s",img);
	fflush(NULL);
	while(strcmp(img,"<>")!=0){
		if(checkFileName(img)){
			ImageProc* imagep = new ImageProc(config);
			features = imagep->getImageFeatures(img, -1,numOfFeatures);
			int* counter = kNearest(kdtree, features,config, *numOfFeatures);
			if(spConfigMinimalGui(config,msg)){
				int numOfSimilarImages = GetspNumOfSimilarImages(config);
				for(int i=0;i<numOfSimilarImages;i++){
					int index = maxIndex(counter,numOfImages);
					counter[index]=-1;
					char* path = (char*)malloc(BUFSIZE);
					if(path == NULL){
						ErrorLogger(level, "OUT OF MEMORY", "Main.cpp",__func__, __LINE__);
						spConfigDestroy(config);
						spLoggerDestroy();
						free(path);
						return -1;
					}
					spConfigGetImagePath(path,config,index);
					imagep->showImage(path);
					getchar();
				}
			}
			else{
				notMinimalGui(counter,img,config,numOfImages);
			}
		}
		else{
			printf("InvalidPath:/n");
			fflush(NULL);
		}
		printf("Please enter an image path:/n");
		fflush(NULL);
		scanf("%s",img);
		fflush(NULL);
	}
	free(filename);
	spLoggerDestroy();
	spConfigDestroy(config);
}



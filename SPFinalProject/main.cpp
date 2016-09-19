/*
 * main.cpp

 *
 *  Created on: 21 баев 2016
 *      Author: DorAlt
 */


#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include "SPImageProc.h"
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
#define INVALID_ARG "INVAlID ARGUMENT"
#define MEMORY "OUT OF MEMORY"
#define MAIN "Main.cpp"
#define IMAGE_PATH "Please enter an image path:\n"
#define INVALID_PATH "InvalidPath:\n"
#define C "-c"
#define S "-c"
#define EXIT "<>"
#define ABORT -1
#define START -1
#define INIT 0
#define EQUAL 0
#define FINISH 0
#define THREE 3

using namespace sp;
void dealwithImg(SPKDArray kdarr, ImageProc* imagep, KDTreeNode kdtree, char* img,SPConfig config, int numOfImages);
int main(int argc, const char* argv[]){
	int i,j,totalNumOfFeatures,index,numOfImages,level,feat,numfeats;
	int* numOfFeatures;
	SPPoint* allImgFeaters;
	SPPoint* features;
	SPConfig config;
	SPKDArray kdarr;
	KDTreeNode kdtree;
	SP_CONFIG_MSG* msg;
	char* img;
	char* filename = (char*)malloc(BUFSIZE);
	feat = INIT;
	index = INIT;
	totalNumOfFeatures = INIT;
	numOfFeatures = &feat;
	if(filename == NULL){
		free(filename);
		return ABORT;
	}
	img = (char*)malloc(BUFSIZE);
	if(img ==NULL){
		free(filename);
		return ABORT;
	}
	strcpy(filename,DEFCON);
	for(i=0; i<argc; i++){
		if(strcmp(argv[i],C)==EQUAL){
			if(i+1<argc)
				strcpy(filename,argv[i+1]);
		}
	}
	if(!checkFileName(filename)){
		free(filename);
		free(img);
		return ABORT;
	}
	msg=(SP_CONFIG_MSG*)malloc(sizeof(SP_CONFIG_MSG));
	if(msg==NULL){
		free(filename);
		free(img);
		return ABORT;
	}
	config = spConfigCreate(filename,msg);
	ImageProc* imagep = new ImageProc(config);
	if(*msg != SP_CONFIG_SUCCESS){
		delete imagep;
		free(img);
		free(msg);
		free(filename);
		return ABORT;
	}
	level = GetSpLoggerLevelNum(config);
	assert(spLoggerCreate(GetSpLoggerFilename(config),GetSpLoggerLevel(config)) == SP_LOGGER_SUCCESS);
	if(spConfigIsExtractionMode(config,msg)){
		if(*msg != SP_CONFIG_SUCCESS){
			ErrorLogger(level, INVALID_ARG, MAIN,__func__, __LINE__);
			spLoggerDestroy();
			free(img);
			free(msg);
			free(filename);
			delete imagep;
			return ABORT;
		}
		numOfImages= spConfigGetNumOfImages(config,msg);
		if(*msg != SP_CONFIG_SUCCESS){
			ErrorLogger(level, INVALID_ARG, MAIN,__func__, __LINE__);
			spLoggerDestroy();
			free(img);
			free(msg);
			free(filename);
			delete imagep;
			return ABORT;
		}
		for(i=0; i<numOfImages; i++){
			char* imagePath = (char*)malloc(BUFSIZE*THREE+sizeof(int));
			if(imagePath == NULL){
				ErrorLogger(level, MEMORY, MAIN,__func__, __LINE__);
				spLoggerDestroy();
				free(img);
				free(msg);
				free(imagePath);
				spConfigDestroy(config);
				delete imagep;
				return ABORT;
			}
			*msg =spConfigGetImagePath(imagePath,config,i);

			if(*msg != SP_CONFIG_SUCCESS){
				ErrorLogger(level, INVALID_ARG, MAIN,__func__, __LINE__);
				spLoggerDestroy();
				free(img);
				free(msg);
				free(imagePath);
				free(filename);
				delete imagep;
				return ABORT;
			}
			features = imagep->getImageFeatures(imagePath,i,numOfFeatures);

			totalNumOfFeatures += feat;
			free(imagePath);
			for(j=0; j<feat; j++){
				spPointDestroy(features[j]);
			}
			free(features);
		}
		allImgFeaters = (SPPoint*)malloc(totalNumOfFeatures*sizeof(*allImgFeaters));
		if(allImgFeaters==NULL){
			ErrorLogger(level, INVALID_ARG, MAIN,__func__, __LINE__);
			spLoggerDestroy();
			free(img);
			free(msg);
			free(filename);
			delete imagep;
			return ABORT;
		}

		for(i=0; i<numOfImages; i++){
			char* imagePath = (char*)malloc(BUFSIZE*THREE+sizeof(int));
			char* imagePathnosuf;
			if(imagePath == NULL){
				ErrorLogger(level, MEMORY, MAIN,__func__, __LINE__);
				spLoggerDestroy();
				free(img);
				free(msg);
				free(imagePath);
				spConfigDestroy(config);
				delete imagep;
				return ABORT;
			}
			imagePathnosuf = (char*)malloc(BUFSIZE*THREE+sizeof(int));

			if(imagePathnosuf == NULL){
				ErrorLogger(level, MEMORY, MAIN,__func__, __LINE__);
				spLoggerDestroy();
				free(img);
				free(msg);
				free(imagePath);
				free(imagePathnosuf);
				spConfigDestroy(config);
				delete imagep;
				return ABORT;
			}


			*msg =spConfigGetImagePath(imagePath, config, i);
			if(*msg != SP_CONFIG_SUCCESS){
				ErrorLogger(level, INVALID_ARG, MAIN,__func__, __LINE__);
				spLoggerDestroy();
				free(img);
				free(msg);
				free(imagePath);
				free(imagePathnosuf);
				free(filename);
				delete imagep;
				return ABORT;
			}
			features = imagep->getImageFeatures(imagePath,i,numOfFeatures);
			for(j=0; j<feat; j++){
				allImgFeaters[index]=features[j];
				index++;
			}
			*msg =spConfigGetImagePathWithoutSuffix(imagePathnosuf, config, i);
			if(*msg != SP_CONFIG_SUCCESS){
				ErrorLogger(level, INVALID_ARG, MAIN,__func__, __LINE__);
				spLoggerDestroy();
				free(imagePath);
				free(msg);
				free(imagePathnosuf);
				free(filename);
				delete imagep;
				return ABORT;
			}
			strcat(imagePathnosuf,FEATSUF);
			if(!extractToFile(imagePathnosuf, features, feat, level)){
				spConfigDestroy(config);
				spLoggerDestroy();
				free(imagePath);
				free(imagePathnosuf);
				free(filename);
				delete imagep;
				return ABORT;
			}

			free(imagePath);
			free(imagePathnosuf);
			free(features);
		}
	}
	else{
		index = INIT;
		char* imagePathnosuf;
		numOfImages= spConfigGetNumOfImages(config,msg);
		if(*msg != SP_CONFIG_SUCCESS){
			ErrorLogger(level, INVALID_ARG, MAIN,__func__, __LINE__);
			spLoggerDestroy();
			free(filename);
			free(msg);
			delete imagep;
			return ABORT;
		}
		imagePathnosuf = (char*)malloc(BUFSIZE*THREE+sizeof(int));
		if(imagePathnosuf == NULL){
			ErrorLogger(level, MEMORY, MAIN,__func__, __LINE__);
			spLoggerDestroy();
			free(imagePathnosuf);
			free(filename);
			free(msg);
			spConfigDestroy(config);
			delete imagep;
			return ABORT;
		}
		numOfImages= spConfigGetNumOfImages(config,msg);
		if(*msg != SP_CONFIG_SUCCESS){
			ErrorLogger(level, INVALID_ARG, MAIN,__func__, __LINE__);
			spLoggerDestroy();
			free(imagePathnosuf);
			free(filename);
			free(msg);
			delete imagep;
			return ABORT;
		}
		for(i=0; i<numOfImages; i++){
			numfeats=INIT;
			*msg =spConfigGetImagePathWithoutSuffix(imagePathnosuf, config, i);
			if(*msg != SP_CONFIG_SUCCESS){
				ErrorLogger(level, INVALID_ARG, MAIN,__func__, __LINE__);
				spLoggerDestroy();
				free(imagePathnosuf);
				free(filename);
				free(msg);
				delete imagep;
			return ABORT;
			}
			strcat(imagePathnosuf,FEATSUF);
			numfeats = getNumOfFeaturesForImage(imagePathnosuf, level);
			if(numfeats == ABORT){
				spConfigDestroy(config);
				spLoggerDestroy();
				free(imagePathnosuf);
				free(filename);
				free(msg);
				delete imagep;
				return ABORT;
			}
			totalNumOfFeatures += numfeats;
		}
		allImgFeaters = (SPPoint*)malloc(totalNumOfFeatures*sizeof(SPPoint));
		if(allImgFeaters == NULL){
			ErrorLogger(level, MEMORY, MAIN,__func__, __LINE__);
			spLoggerDestroy();
			spConfigDestroy(config);
			free(imagePathnosuf);
			free(filename);
			free(msg);
			free(allImgFeaters);
			delete imagep;
			return ABORT;
		}
		for(i=0; i<numOfImages; i++){
			*msg =spConfigGetImagePathWithoutSuffix(imagePathnosuf, config, i);
			if(*msg != SP_CONFIG_SUCCESS){
				ErrorLogger(level, INVALID_ARG, MAIN,__func__, __LINE__);
				spLoggerDestroy();
				free(imagePathnosuf);
				free(filename);
				free(msg);
				free(allImgFeaters);
				delete imagep;
				return ABORT;
			}
			strcat(imagePathnosuf,FEATSUF);

			features = extractFromFiles(imagePathnosuf, level,numOfFeatures);
			if(features == NULL){
				spConfigDestroy(config);
				for(j=0; j<=i; j++){
					spPointDestroy(allImgFeaters[j]);
				}
				spLoggerDestroy();
				free(imagePathnosuf);
				free(filename);
				free(msg);
				free(allImgFeaters);
				delete imagep;
				return ABORT;
			}
			for(j=0; j<feat; j++){
				allImgFeaters[index] = features[j];
				index++;
			}
			free(features);
			free(imagePathnosuf);
		}
	}
	free(msg);
	kdarr = spKdarrayInit(allImgFeaters,totalNumOfFeatures);
	if(kdarr==NULL){
		free(filename);
		delete imagep;
		return ABORT;
	}
	kdtree = buildKDTree(kdarr, GetSplitMethod(config),0);
	if(kdtree==NULL){
		free(filename);
		spKDArrayDestroy(kdarr);
		delete imagep;
		return ABORT;
	}
	printf(IMAGE_PATH);
	fflush(NULL);
	scanf(S,img);
	fflush(NULL);


	while(strcmp(img,EXIT)!=EQUAL){
		if(checkFileName(img)){
			dealwithImg(kdarr, imagep, kdtree, img, config, numOfImages);
			if(config==NULL){
				free(filename);
				delete imagep;
				return ABORT;
			}

		}
		else{
			printf(INVALID_PATH);
			fflush(NULL);
		}
		printf(IMAGE_PATH);
		fflush(NULL);
		scanf(S,img);
		fflush(NULL);
	}
	free(filename);
	free(img);
	spLoggerDestroy();
	spConfigDestroy(config);
	spKDArrayDestroy(kdarr);
	for(int i=0;i<totalNumOfFeatures;i++){
		spPointDestroy(allImgFeaters[i]);
	}
	free(allImgFeaters);
	KDTreeDestroy(kdtree);
	delete imagep;
	return FINISH;
}
void dealwithImg(SPKDArray kdarr, ImageProc* imagep, KDTreeNode kdtree, char* img,SPConfig config, int numOfImages){
	int feats=INIT;
	int* numOfFeatures=&feats;
	SP_CONFIG_MSG* msg=(SP_CONFIG_MSG*)malloc(sizeof(SP_CONFIG_MSG));
	SPPoint* features = imagep->getImageFeatures(img, 0,numOfFeatures);
	int i,numOfSimilarImages,index;
	int* counter = kNearest(kdtree, features,config, *numOfFeatures);
	if(counter==NULL || msg == NULL){
		ErrorLogger(GetSpLoggerLevelNum(config), MEMORY, MAIN,__func__, __LINE__);
		spKDArrayDestroy(kdarr);
		spLoggerDestroy();
		KDTreeDestroy(kdtree);
		free(msg);
		free(counter);
		return;
	}
	if(spConfigMinimalGui(config,msg)){
		numOfSimilarImages = GetspNumOfSimilarImages(config);
		for(i=0; i<numOfSimilarImages; i++){
			index = maxIndex(counter,numOfImages);
			counter[index]=START;
			char* path = (char*)malloc(BUFSIZE);
			if(path == NULL){
				ErrorLogger(GetSpLoggerLevelNum(config), MEMORY, MAIN,__func__, __LINE__);
				spConfigDestroy(config);
				spLoggerDestroy();
				free(path);

				spKDArrayDestroy(kdarr);
				KDTreeDestroy(kdtree);
				return;
			}
			spConfigGetImagePath(path,config,index);
			imagep->showImage(path);
			free(path);
		}
	}
	else{
		notMinimalGui(counter,img,config,numOfImages);
	}
	for(int i=0;i<feats;i++){
		spPointDestroy(features[i]);
	}
	free(features);
	free(counter);
	free(msg);
}



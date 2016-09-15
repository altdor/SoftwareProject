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
	feat = 0;
	index = 0;
	totalNumOfFeatures = 0;
	numOfFeatures = &feat;
	if(filename == NULL){
		free(filename);
		return -1;
	}
	img = (char*)malloc(BUFSIZE);
	if(img ==NULL){
		free(filename);
		return -1;
	}
	strcpy(filename,DEFCON);
	for(i=0; i<argc; i++){
		if(strcmp(argv[i],"-c")==0){
			if(i+1<argc)
				strcpy(filename,argv[i+1]);
		}
	}
	if(!checkFileName(filename)){
		free(filename);
		free(img);
		return -1;
	}
	printf("5\n");
	fflush(NULL);
	msg=(SP_CONFIG_MSG*)malloc(sizeof(SP_CONFIG_MSG));
	if(msg==NULL){
		free(filename);
		free(img);
		return -1;
	}
	config = spConfigCreate(filename,msg);
	ImageProc* imagep = new ImageProc(config);
	printf("6\n");
	fflush(NULL);
	if(*msg != SP_CONFIG_SUCCESS){
		delete imagep;
		free(img);
		free(msg);
		free(filename);
		return -1;
	}
	level = GetSpLoggerLevelNum(config);
	printf("7\n");
	fflush(NULL);
	assert(spLoggerCreate(GetSpLoggerFilename(config),GetSpLoggerLevel(config)) == SP_LOGGER_SUCCESS);
	if(spConfigIsExtractionMode(config,msg)){
		printf("8\n");
			fflush(NULL);
		if(*msg != SP_CONFIG_SUCCESS){
			ErrorLogger(level, "INVAlID ARGUMENT", "Main.cpp",__func__, __LINE__);
			spLoggerDestroy();
			free(img);
			free(msg);
			free(filename);
			delete imagep;
			return -1;
		}
		numOfImages= spConfigGetNumOfImages(config,msg);
		if(*msg != SP_CONFIG_SUCCESS){
			ErrorLogger(level, "INVAlID ARGUMENT", "Main.cpp",__func__, __LINE__);
			spLoggerDestroy();
			free(img);
			free(msg);
			free(filename);
			delete imagep;
			return -1;
		}
		for(i=0; i<numOfImages; i++){
			char* imagePath = (char*)malloc(BUFSIZE*3+sizeof(int));
			if(imagePath == NULL){
				ErrorLogger(level, "OUT OF MEMORY", "Main.cpp",__func__, __LINE__);
				spLoggerDestroy();
				free(img);
				free(msg);
				free(imagePath);
				spConfigDestroy(config);
				delete imagep;
				return -1;
			}
			*msg =spConfigGetImagePath(imagePath,config,i);

			if(*msg != SP_CONFIG_SUCCESS){
				ErrorLogger(level, "Invalid argument", "Main.cpp",__func__, __LINE__);
				spLoggerDestroy();
				free(img);
				free(msg);
				free(imagePath);
				free(filename);
				delete imagep;
				return -1;
			}
			features = imagep->getImageFeatures(imagePath,i,numOfFeatures);

			totalNumOfFeatures += feat;
			free(imagePath);
			for(j=0; j<feat; j++){
				spPointDestroy(features[j]);
			}
			free(features);
		}
		printf("end of feats calculation:%d \n",totalNumOfFeatures);
					fflush(NULL);
		allImgFeaters = (SPPoint*)malloc(totalNumOfFeatures*sizeof(*allImgFeaters));
		if(allImgFeaters==NULL){
			ErrorLogger(level, "INVAlID ARGUMENT", "Main.cpp",__func__, __LINE__);
			spLoggerDestroy();
			free(img);
			free(msg);
			free(filename);
			delete imagep;
			return -1;
		}

		for(i=0; i<numOfImages; i++){
			char* imagePath = (char*)malloc(BUFSIZE*3+sizeof(int));
			char* imagePathnosuf;
			if(imagePath == NULL){
				ErrorLogger(level, "OUT OF MEMORY", "Main.cpp",__func__, __LINE__);
				spLoggerDestroy();
				free(img);
				free(msg);
				free(imagePath);
				spConfigDestroy(config);
				delete imagep;
				return -1;
			}
			imagePathnosuf = (char*)malloc(BUFSIZE*3+sizeof(int));

			if(imagePathnosuf == NULL){
				ErrorLogger(level, "OUT OF MEMORY", "Main.cpp",__func__, __LINE__);
				spLoggerDestroy();
				free(img);
				free(msg);
				free(imagePath);
				free(imagePathnosuf);
				spConfigDestroy(config);
				delete imagep;
				return -1;
			}


			*msg =spConfigGetImagePath(imagePath, config, i);
			if(*msg != SP_CONFIG_SUCCESS){
				ErrorLogger(level, "Invalid argument", "Main.cpp",__func__, __LINE__);
				spLoggerDestroy();
				free(img);
				free(msg);
				free(imagePath);
				free(imagePathnosuf);
				free(filename);
				delete imagep;
				return -1;
			}
			features = imagep->getImageFeatures(imagePath,i,numOfFeatures);
			for(j=0; j<feat; j++){
				allImgFeaters[index]=features[j];
				index++;
			}
			*msg =spConfigGetImagePathWithoutSuffix(imagePathnosuf, config, i);
			if(*msg != SP_CONFIG_SUCCESS){
				ErrorLogger(level, "Invalid argument", "Main.cpp",__func__, __LINE__);
				spLoggerDestroy();
				free(imagePath);
				free(msg);
				free(imagePathnosuf);
				free(filename);
				delete imagep;
				return -1;
			}
			strcat(imagePathnosuf,FEATSUF);
			if(!extractToFile(imagePathnosuf, features, feat, level)){
				spConfigDestroy(config);
				spLoggerDestroy();
				free(imagePath);
				free(imagePathnosuf);
				free(filename);
				delete imagep;
				return -1;
			}

			free(imagePath);
			free(imagePathnosuf);
			free(features);
		}
	}
	else{
		index = 0;
		char* imagePathnosuf;
		numOfImages= spConfigGetNumOfImages(config,msg);
		if(*msg != SP_CONFIG_SUCCESS){
			ErrorLogger(level, "Invalid argument", "Main.cpp",__func__, __LINE__);
			spLoggerDestroy();
			free(filename);
			free(msg);
			delete imagep;
			return -1;
		}
		imagePathnosuf = (char*)malloc(BUFSIZE*3+sizeof(int));
		if(imagePathnosuf == NULL){
			ErrorLogger(level, "out of memory", "Main.cpp",__func__, __LINE__);
			spLoggerDestroy();
			free(imagePathnosuf);
			free(filename);
			free(msg);
			spConfigDestroy(config);
			delete imagep;
			return -1;
		}
		numOfImages= spConfigGetNumOfImages(config,msg);
		if(*msg != SP_CONFIG_SUCCESS){
			ErrorLogger(level, "Invalid argument", "Main.cpp",__func__, __LINE__);
			spLoggerDestroy();
			free(imagePathnosuf);
			free(filename);
			free(msg);
			delete imagep;
			return -1;
		}
		for(i=0; i<numOfImages; i++){
			numfeats=0;
			*msg =spConfigGetImagePathWithoutSuffix(imagePathnosuf, config, i);
			if(*msg != SP_CONFIG_SUCCESS){
				ErrorLogger(level, "Invalid argument", "Main.cpp",__func__, __LINE__);
				spLoggerDestroy();
				free(imagePathnosuf);
				free(filename);
				free(msg);
				delete imagep;
			return -1;
			}
			strcat(imagePathnosuf,FEATSUF);
			numfeats = getNumOfFeaturesForImage(imagePathnosuf, level);
			if(numfeats == -1){
				spConfigDestroy(config);
				spLoggerDestroy();
				free(imagePathnosuf);
				free(filename);
				free(msg);
				delete imagep;
				return -1;
			}
			totalNumOfFeatures += numfeats;
		}
		allImgFeaters = (SPPoint*)malloc(totalNumOfFeatures*sizeof(SPPoint));
		if(allImgFeaters == NULL){
			ErrorLogger(level, "out of memory", "Main.cpp",__func__, __LINE__);
			spLoggerDestroy();
			spConfigDestroy(config);
			free(imagePathnosuf);
			free(filename);
			free(msg);
			free(allImgFeaters);
			delete imagep;
			return -1;
		}
		for(i=0; i<numOfImages; i++){
			*msg =spConfigGetImagePathWithoutSuffix(imagePathnosuf, config, i);
			if(*msg != SP_CONFIG_SUCCESS){
				ErrorLogger(level, "Invalid argument", "Main.cpp",__func__, __LINE__);
				spLoggerDestroy();
				free(imagePathnosuf);
				free(filename);
				free(msg);
				free(allImgFeaters);
				delete imagep;
				return -1;
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
				return -1;
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
	printf("11\n");
	fflush(NULL);
	if(kdarr==NULL){
		free(filename);
		delete imagep;
		return -1;
	}
	printf("12\n");
	fflush(NULL);
	kdtree = buildKDTree(kdarr, GetSplitMethod(config),0);
	printf("13\n");
	fflush(NULL);
	if(kdtree==NULL){
		free(filename);
		spKDArrayDestroy(kdarr);
		delete imagep;
		return -1;
	}
	printf("Please enter an image path:\n");
	fflush(NULL);
	scanf("%s",img);
	fflush(NULL);


	while(strcmp(img,"<>")!=0){
		if(checkFileName(img)){
			dealwithImg(kdarr, imagep, kdtree, img, config, numOfImages);
			if(config==NULL){
				free(filename);
				delete imagep;
				return -1;
			}

		}
		else{
			printf("InvalidPath:\n");
			fflush(NULL);
		}
		printf("Please enter an image path:\n");
		fflush(NULL);
		scanf("%s",img);
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
	return 0;
}
void dealwithImg(SPKDArray kdarr, ImageProc* imagep, KDTreeNode kdtree, char* img,SPConfig config, int numOfImages){
	int feats=0;
	int* numOfFeatures=&feats;
	SP_CONFIG_MSG* msg=(SP_CONFIG_MSG*)malloc(sizeof(SP_CONFIG_MSG));
	SPPoint* features = imagep->getImageFeatures(img, 0,numOfFeatures);
	int i,numOfSimilarImages,index;
	int* counter = kNearest(kdtree, features,config, *numOfFeatures);
	if(counter==NULL || msg == NULL){
		ErrorLogger(GetSpLoggerLevelNum(config), "OUT OF MEMORY", "Main.cpp",__func__, __LINE__);
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
			counter[index]=-1;
			char* path = (char*)malloc(BUFSIZE);
			if(path == NULL){
				ErrorLogger(GetSpLoggerLevelNum(config), "OUT OF MEMORY", "Main.cpp",__func__, __LINE__);
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



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
void dealwithImg(SPKDArray kdarr, ImageProc* imagep, SPPoint* features, KDTreeNode kdtree, char* img, int* numOfFeatures,SPConfig config, int numOfImages);
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
	printf("1\n");
	fflush(NULL);
	if(filename == NULL){
		free(filename);
		return -1;
	}
	img = (char*)malloc(BUFSIZE);
	if(img ==NULL){
		free(filename);
		return -1;
	}
	printf("2\n");
	fflush(NULL);
	strcpy(filename,DEFCON);
	printf("3\n");
	fflush(NULL);
	for(i=0; i<argc; i++){
		if(strcmp(argv[i],"-c")==0){
			if(i+1<argc)
				strcpy(filename,argv[i+1]);
		}
	}
	printf("4\n");
	fflush(NULL);
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
	printf("9 \n");
	fflush(NULL);/*
	featuresArr = make2DTo1D(allImgFeaters,numOfImages,feat,config);
	printf("after \n");
	fflush(NULL);
	if(featuresArr ==NULL){
		free(filename);
		spConfigDestroy(config);
		return -1;
	}
	kdarr = (SPKDArray)malloc(sizeof(SPKDArray));
	if(kdarr==NULL){
		ErrorLogger(GetSpLoggerLevel(config), "Allocating Failed", "Main.cpp",__func__, __LINE__);
		spLoggerDestroy();
		spConfigDestroy(config);
		free(filename);
		for (i=0; i<numOfImages; i++){
			for(j=0; j<(feat); j++){
				spPointDestroy(allImgFeaters[i*j+j]);
			}
		}
		free(allImgFeaters);
		return -1;
	}
	kdtree = (KDTreeNode)malloc(sizeof(KDTreeNode));
	if(kdtree==NULL){
		ErrorLogger(GetSpLoggerLevel(config), "Allocating Failed", "Main.cpp",__func__, __LINE__);
		spLoggerDestroy();
		spConfigDestroy(config);
		free(filename);
		free(kdarr);
		for (i=0; i<numOfImages; i++){
			for(j=0; j<(feat); j++){
				spPointDestroy(allImgFeaters[i*j+j]);
			}
		}
		free(allImgFeaters);
		free(msg);
		return -1;
	}*/
	printf("10\n");
	fflush(NULL);
	kdarr = spKdarrayInit(allImgFeaters,totalNumOfFeatures);
	printf("11\n");
		fflush(NULL);
	if(kdarr==NULL){
		printf("no kdarray\n");
		fflush(NULL);
		free(filename);
		free(msg);
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
		free(msg);
		delete imagep;
		return -1;
	}
	printf("Please enter an image path:\n");
	fflush(NULL);
	scanf("%s",img);
	fflush(NULL);


	while(strcmp(img,"<>")!=0){
		if(checkFileName(img)){
			dealwithImg(kdarr, imagep, features, kdtree, img, numOfFeatures, config, numOfImages);
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
	printf("somewhere ");
			fflush(NULL);
	free(filename);
	spLoggerDestroy();
	printf("only ");
				fflush(NULL);
	spConfigDestroy(config);
	printf("we ");
				fflush(NULL);
	free(filename);
	spKDArrayDestroy(kdarr);
	printf("know \n");
				fflush(NULL);
	KDTreeDestroy(kdtree);
	printf("know \n");
					fflush(NULL);
	delete imagep;
	return 0;
}
void dealwithImg(SPKDArray kdarr, ImageProc* imagep, SPPoint* features, KDTreeNode kdtree, char* img, int* numOfFeatures,SPConfig config, int numOfImages){
	SP_CONFIG_MSG* msg=(SP_CONFIG_MSG*)malloc(sizeof(SP_CONFIG_MSG));
	features = imagep->getImageFeatures(img, -1,numOfFeatures);
	int i,numOfSimilarImages,index;
	int* counter = kNearest(kdtree, features,config, *numOfFeatures);
	for(int i = 0; i<spConfigGetNumOfImages(config,msg);i++){
		printf("%d ",counter[i]);
		fflush(NULL);
	}
	printf("\n");
	fflush(NULL);
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
		getchar();
	}
	}
	else{
		notMinimalGui(counter,img,config,numOfImages);
	}
	free(msg);
}



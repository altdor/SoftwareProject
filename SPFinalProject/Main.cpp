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
#include "SPLogger.h"
}

#define BUFSIZE 1024
#define DEFCON "spcbir.config"
#define FEATSUF ".feats"
using namespace sp;
int main(int argc, const char* argv[]){
	SPPoint** allImgFeaters;
	SPConfig config;
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
			int *numOfFeatures=0;
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

				free(imagePathnosuf);
				free(filename);
				free(allImgFeaters);
				return -1;
			}
			free(imagePathnosuf);
		}

	}
	printf("Please enter an image path:/n");
	scanf("%s",img);
	while(strcmp(img,"<>")!=0){
		/*
		 *
		 *
		 *
		 *
		 */
		printf("Please enter an image path:/n");
		scanf("%s",img);
	}
	for(int j=0;j<=numOfImages;j++){
		free(allImgFeaters[j]);
	}
	free(filename);
	free(allImgFeaters);
	spLoggerDestroy();
	spConfigDestroy(config);
}



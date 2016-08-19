#include "sp_image_proc_util.h"
#include "main_aux.h"
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <opencv2/xfeatures2d.hpp>//SiftDescriptorExtractor
#include <opencv2/features2d.hpp>
#include <stdlib.h>
#include <cstdio>
#include <cstring>

#include <unistd.h>
#define EXIT_MSG "Exiting...\n"
#define DIRECTORY_MSG "Enter images directory path:\n"
#define PREFIX_MSG "Enter images prefix:\n"
#define IMG_NUM_MSG "Enter number of images:\n"
#define ALLOCATION_ERROR "An error occurred - allocation failure\n"
//#define WORD_SIZE 1024
int main() {
	char dir[1024],pref[1024],suf[1024],query[1024];
	char numOfimages[1024];
	int num,bins,fet,queryFeatures;
	int *nFeatures, *bestHistsdis, *bestSiftDis;
	int*** hist;
	double*** sift;
	int** queryHist;
	double** querySift;
	printf(DIRECTORY_MSG);
	fflush(NULL);
	scanf("%s",dir);
	printf(PREFIX_MSG);
	fflush(NULL);
	scanf("%s",pref);
	printf(IMG_NUM_MSG);
	fflush(NULL);

	scanf("%s",numOfimages);
	num = atoi(numOfimages);
	if (num<1){
		printf("An error occurred - invalid number of images\n");
		fflush(NULL);
		return 0;
	}
	nFeatures = (int*)malloc(num*sizeof(int));
	if(nFeatures == NULL)
		return 0;
	printf("Enter images suffix:\n");
	fflush(NULL);
	scanf("%s",suf);
	printf("Enter number of bins:\n");
	fflush(NULL);
	scanf("%d",&bins);
	if (bins<1){
		printf("An error occurred - invalid number of bins\n");
		fflush(NULL);
		free(nFeatures);
		return 0;
	}
	printf("Enter number of features:\n");
	fflush(NULL);
	scanf("%d",&fet);
	fflush(NULL);
	if (fet<1){
		printf("An error occurred - invalid number of features\n");
		fflush(NULL);
		free(nFeatures);

		return 0;
	}
	hist = (int***)malloc(num*sizeof(int**));
	if(hist == NULL){
			printf(ALLOCATION_ERROR);
			free(nFeatures);
			free(hist);
			return -1;
		}
		for(int i = 0;i<num;i++){
			hist[i] = (int**)malloc(3*sizeof(int*));
			if(hist[i] == NULL){
					printf(ALLOCATION_ERROR);
					free(nFeatures);
					free(hist);
					return -1;
				}
			for(int j = 0;j<3;j++){
				hist[i][j] = (int*)malloc(bins*sizeof(int));
				if(hist[i][j] == NULL){
						printf(ALLOCATION_ERROR);
						free(nFeatures);
						free(hist);
						return -1;
				}
			}
		}
	hist = makeHist(dir, pref, num, suf, bins, hist);
	if(hist == NULL){
		printf(ALLOCATION_ERROR);
		free(nFeatures);
		free(hist);
		return -1;
	}
	sift = makeSift(dir,  pref, num, suf, fet, nFeatures);
	if(sift == NULL){
		printf(ALLOCATION_ERROR);
		free(nFeatures);
		free(hist);
		free(sift);
		return -1;
	}
	while(true){
		printf("Enter a query image or # to terminate:\n");
		fflush(NULL);
		scanf("%s",query);

		fflush(NULL);
		if(strcmp("#", query) == 0){
			printf(EXIT_MSG);
			fflush(NULL);
			free(nFeatures);
			free(hist);
			free(sift);

			return 0;
		}
		queryHist =  spGetRGBHist(query, bins);
		if(queryHist == NULL){
			printf(ALLOCATION_ERROR);
			free(nFeatures);
			free(hist);
			free(sift);
			free(queryHist);
			free(bestHistsdis);
			free(queryHist);
			free(bestHistsdis);
			free(bestSiftDis);
			return -1;
		}
		querySift =  spGetSiftDescriptors(query, fet, &queryFeatures);
		if(querySift == NULL){
			printf(ALLOCATION_ERROR);
			free(nFeatures);
			free(hist);
			free(sift);
			free(queryHist);
			free(bestHistsdis);
			free(queryHist);
			free(bestHistsdis);
			free(bestSiftDis);
			return -1;
		}
		bestHistsdis = bestHistogramDistances(queryHist, hist, bins, num);
		if(bestHistsdis == NULL){
			printf(ALLOCATION_ERROR);
			free(nFeatures);
			free(hist);
			free(sift);
			free(queryHist);
			free(bestHistsdis);
			free(queryHist);
			free(bestHistsdis);
			free(bestSiftDis);
			return -1;
		}
		printf("Nearest images using global descriptors:\n");
		fflush(NULL);
		printf("%d, %d, %d, %d, %d\n",bestHistsdis[0],bestHistsdis[1],bestHistsdis[2],bestHistsdis[3],bestHistsdis[4]);
		fflush(NULL);
		bestSiftDis = BestImgSiftDistance(querySift, sift, queryFeatures, nFeatures,num);
		if(bestSiftDis == NULL){
			printf(ALLOCATION_ERROR);
			free(nFeatures);
			free(hist);
			free(sift);
			free(queryHist);
			free(bestHistsdis);
			free(queryHist);
			free(bestHistsdis);
			free(bestSiftDis);
			return -1;
		}
		printf("Nearest images using local descriptors:\n");
		fflush(NULL);
		printf("%d, %d, %d, %d, %d\n",bestSiftDis[0],bestSiftDis[1],bestSiftDis[2],bestSiftDis[3],bestSiftDis[4]);
		fflush(NULL);
		free(queryHist);
		free(bestHistsdis);
		free(bestSiftDis);
	}
	free(nFeatures);
	free(hist);
	free(sift);
	free(queryHist);
	free(bestHistsdis);
	free(bestSiftDis);

}

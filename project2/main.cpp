#include "sp_image_proc_util.h"
#include "main_aux.h"
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <opencv2/xfeatures2d.hpp>//SiftDescriptorExtractor
#include <opencv2/features2d.hpp>
#include <stdlib.h>
#include <cstdio>


#define EXIT_MSG "Exiting...\n"
#define DIRECTORY_MSG "Enter images directory path:\n"
#define PREFIX_MSG "Enter images prefix:\n"
#define IMG_NUM_MSG "Enter number of images:\n"
#define ALLOCATION_ERROR "An error occurred - allocation failure\n"
#define IMG_NUM_ERROR "An error occurred - invalid number of images\n"
#define SUFFIX_MSG "Enter images suffix:\n"
#define BINS_MSG "Enter number of bins:\n"
#define BINS_NUM_ERROR "An error occurred - invalid number of bins\n"
#define FET_MSG "Enter number of features:\n"
#define FET_NUM_ERROR "An error occurred - invalid number of features\n"
#define QUERY_MSG "Enter a query image or # to terminate:\n"
#define GLOBAL_MSG "Nearest images using global descriptors:\n"
#define LOCAL_MSG "Nearest images using local descriptors:\n"

#define WORD_SIZE 1024
#define EXIT_NUM 0
#define NEGATIVE_NUM -1

//#define WORD_SIZE 1024
int main() {
	char dir[WORD_SIZE],pref[WORD_SIZE],suf[WORD_SIZE],query[WORD_SIZE];
	char numOfimages[WORD_SIZE];
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
		/* Wrong input from user*/
		printf(IMG_NUM_ERROR);
		fflush(NULL);
		return EXIT_NUM;
	}

	nFeatures = (int*)malloc(num*sizeof(int));
	if(nFeatures == NULL)
		/* An allocation error */
		return EXIT_NUM;

	printf(SUFFIX_MSG);
	fflush(NULL);
	scanf("%s",suf);
	printf(BINS_MSG);
	fflush(NULL);
	scanf("%d",&bins);

	if (bins<1){
		/* Wrong input from user , free memory */
		printf(BINS_NUM_ERROR);
		fflush(NULL);
		free(nFeatures);
		return EXIT_NUM;
	}

	printf(FET_MSG);
	fflush(NULL);
	scanf("%d",&fet);
	fflush(NULL);

	if (fet<1){
		/* Wrong input from user , free memory */
		printf(FET_NUM_ERROR);
		fflush(NULL);
		free(nFeatures);
		return EXIT_NUM;
	}

	hist = (int***)malloc(num*sizeof(int**));
	if(hist == NULL){
		/* An allocation error , free memory */
		printf(ALLOCATION_ERROR);
		free(nFeatures);
		free(hist);
		return NEGATIVE_NUM;
	}
	for(int i = 0;i<num;i++){
		hist[i] = (int**)malloc(3*sizeof(int*));
		if(hist[i] == NULL){
			/* An allocation error , free memory */
			printf(ALLOCATION_ERROR);
			free(nFeatures);
			for (int j=0; j<i; j++){
				free(hist[j]);
			}
			free(hist);
			return NEGATIVE_NUM;
		}
		for(int j = 0;j<3;j++){
			hist[i][j] = (int*)malloc(bins*sizeof(int));
			/* An allocation error , free memory */
			if(hist[i][j] == NULL){
				printf(ALLOCATION_ERROR);
				free(nFeatures);
				for (int l=0; l<i; l++){
					for (int k=0; k<3; k++){
						free(hist[l][k]);
					}
				}
				for (int k=0; k<j; k++){
					free(hist[i][k]);
				}
				free(hist);
				return NEGATIVE_NUM;
			}
		}
	}
	/* calculating RGB Histogram for each image */
	hist = makeHist(dir, pref, num, suf, bins, hist);
	if(hist == NULL){
		/* An allocation error , free memory */
		printf(ALLOCATION_ERROR);
		free(nFeatures);
		for (int i=0; i<num; i++){
			for (int j=0; j<3; j++){
				free(hist[i][j]);
			}
		}
		free(hist);
		return NEGATIVE_NUM;
	}
	/* calculating SIFT descriptors for each image */
	sift = makeSift(dir, pref, num, suf, fet, nFeatures);
	if(sift == NULL){
		/* An allocation error , free memory */
		printf(ALLOCATION_ERROR);
		
		for (int i=0; i<num; i++){
			for (int j=0; j<3; j++){
				free(hist[i][j]);
			}
		}
		free(hist);
		for (int i=0; i<num; i++){
			for (int j=0; j<nFeatures[i]; j++){
				free(sift[i][j]);
			}
		}
		free(sift);
		free(nFeatures);
		return NEGATIVE_NUM;
	}

	while(true){
		printf(QUERY_MSG);
		fflush(NULL);
		scanf("%s",query);
		fflush(NULL);
		if(strcmp("#", query) == 0){
			/* EXIT the program , free memory */
			printf(EXIT_MSG);
			fflush(NULL);
			
			for (int i=0; i<num; i++){
				for (int j=0; j<3; j++){
					free(hist[i][j]);
				}
			}
			free(hist);
			for (int i=0; i<num; i++){
				for (int j=0; j<nFeatures[i]; j++){
					free(sift[i][j]);
				}
			}
			free(sift);
			free(nFeatures);
			return EXIT_NUM;
		}
		/* calculating RGB Histogram for the query image */
		queryHist =  spGetRGBHist(query, bins);
		if(queryHist == NULL){
			/* An allocation error , free memory */
			printf(ALLOCATION_ERROR);
			
			for (int i=0; i<num; i++){
				for (int j=0; j<3; j++){
					free(hist[i][j]);
				}
			}
			free(hist);
			for (int i=0; i<num; i++){
				for (int j=0; j<nFeatures[i]; j++){
					free(sift[i][j]);
				}
			}
			free(sift);
			for (int j=0; j<3; j++){
				free(queryHist[j]);
			}
			free(nFeatures);
			free(queryHist);
			free(bestHistsdis);
			free(bestSiftDis);
			return NEGATIVE_NUM;
		}
		/* calculating SIFT descriptors for the query image */
		querySift =  spGetSiftDescriptors(query, fet, &queryFeatures);
		if(querySift == NULL){
			/* An allocation error , free memory */
			printf(ALLOCATION_ERROR);
			
			
			for (int i=0; i<num; i++){
				for (int j=0; j<nFeatures[i]; j++){
					free(sift[i][j]);
				}
			}
			free(sift);
			for (int i=0; i<num; i++){
				for (int j=0; j<3; j++){
					free(hist[i][j]);
				}
			}
			free(hist);
			for (int j=0; j<3; j++){
				free(queryHist[j]);
			}
			free(nFeatures);
			free(queryHist);
			free(bestHistsdis);
			free(bestSiftDis);
			return NEGATIVE_NUM;
		}
		/* calculating the closest 5 Histogram to the query image`s Histogram */
		bestHistsdis = bestHistogramDistances(queryHist, hist, bins, num);
		if(bestHistsdis == NULL){
			/* An allocation error , free memory */
			printf(ALLOCATION_ERROR);
			
			
			for (int i=0; i<num; i++){
				for (int j=0; j<nFeatures[i]; j++){
					free(sift[i][j]);
				}
			}
			free(sift);
			for (int i=0; i<num; i++){
				for (int j=0; j<3; j++){
					free(hist[i][j]);
				}
			}
			free(hist);
			for (int j=0; j<3; j++){
				free(queryHist[j]);
			}
			free(nFeatures);
			free(queryHist);
			free(bestHistsdis);
			free(bestSiftDis);
			return NEGATIVE_NUM;
		}
		printf(GLOBAL_MSG);
		fflush(NULL);
		printf("%d, %d, %d, %d, %d\n",bestHistsdis[0],bestHistsdis[1],bestHistsdis[2],bestHistsdis[3],bestHistsdis[4]);
		fflush(NULL);
		/* calculating the closest 5 images with relation to their SIFT descriptors */
		bestSiftDis = BestImgSiftDistance(querySift, sift, queryFeatures, nFeatures,num);
		if(bestSiftDis == NULL){
			/* An allocation error , free memory */
			printf(ALLOCATION_ERROR);
			
			
			for (int i=0; i<num; i++){
				for (int j=0; j<nFeatures[i]; j++){
					free(sift[i][j]);
				}
			}
			free(sift);
			for (int i=0; i<num; i++){
				for (int j=0; j<3; j++){
					free(hist[i][j]);
				}
			}
			free(hist);
			for (int j=0; j<3; j++){
				free(queryHist[j]);
			}
			free(nFeatures);
			free(queryHist);
			free(bestHistsdis);
			free(bestSiftDis);
			return NEGATIVE_NUM;
		}
		printf(LOCAL_MSG);
		fflush(NULL);
		printf("%d, %d, %d, %d, %d\n",bestSiftDis[0],bestSiftDis[1],bestSiftDis[2],bestSiftDis[3],bestSiftDis[4]);
		fflush(NULL);
		for (int j=0; j<3; j++){
			free(queryHist[j]);
		}
		free(queryHist);
		free(bestHistsdis);
		free(bestSiftDis);
	}
	free(nFeatures);
	for (int i=0; i<num; i++){
		for (int j=0; j<3; j++){
			free(hist[i][j]);
		}
	}
	free(hist);
	for (int i=0; i<num; i++){
		for (int j=0; j<nFeatures[i]; j++){
			free(sift[i][j]);
		}
	}
	free(sift);
	for (int j=0; j<3; j++){
		free(queryHist[j]);
	}
	free(queryHist);
	free(bestHistsdis);
	free(bestSiftDis);
	return EXIT_NUM;

}

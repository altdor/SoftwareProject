/*
 * main_aux.cpp
 *
 *  Created on: 4 במאי 2016
 *      Author: DorAlt
 */
#include "main_aux.h"
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <opencv2/xfeatures2d.hpp>//SiftDescriptorExtractor
#include <opencv2/features2d.hpp>

#define WORD_SIZE 1024
#define NUM_OF_CLOSEST 5
#define NEGATIVE_NUM -1


/*
 * the function builds the path of the images (directory + prefix + num + suffix),
 * the number of bins in the picture
 * and calculates the histogram for all images
*/
int*** makeHist(char* dir, char* pref, int num, char* suf, int bins, int*** hist){
	hist = (int***)malloc(num*sizeof(int**));
	char imagePath[3*WORD_SIZE];
	if(hist == NULL){
		return NULL;
	}
	for(int i = 0;i<num;i++){
		hist[i] = (int**)malloc(3*sizeof(int*));
		if(hist[i] == NULL){
				return NULL;
			}
		for(int j = 0;j<3;j++){
			hist[i][j] = (int*)malloc(bins*sizeof(int));
			if(hist[i][j] == NULL){
					return NULL;
			}
		}
	}
	for (int i=0; i<num; i++){
		sprintf(imagePath, "%s%s%d%s", dir,pref,i,suf);
		hist[i] =  spGetRGBHist(imagePath, bins);
		if(hist[i] == NULL){
			return NULL;
		}
	}

	return hist;
}

/* the function builds the path of the images (directory + prefix + num + suffix),
 * using the number of features in each picture
 * and returns the sift arrays representing the pictures
 * and how many features were extracted from each picture
*/
double*** makeSift(char* dir, char* pref, int num, char* suf, int fet, int* nFeatures){
	char imagePath[3*WORD_SIZE];
	int features;
	double*** sift = (double***)malloc(sizeof(double**)*num);
	if(sift == NULL){
		return NULL;
	}

	for (int i=0; i<num; i++){
		sprintf(imagePath, "%s%s%d%s", dir,pref,i,suf);
		sift[i] =  spGetSiftDescriptors(imagePath, fet, &features);
		if(sift[i] == NULL)
			return NULL;
		nFeatures[i] = features;
	}

	return sift;
}

/* the function receives the query's histogram
 * and a 3D array containing the histograms of the images
 * and it returns the pictures with the closest histogram
 * to the query's histogram
 */
int* bestHistogramDistances(int** histA, int*** histB, int nBins,int num){

	int* bests;
	int min;
	/* an array containing the distances between the query's histogram and each of the other image's histogram */
	double** arr = (double**)malloc(num*sizeof(double*));
	if(arr == NULL){
		return NULL;
	}
	for(int i = 0;i<num;i++){
		arr[i] = (double*)malloc(NUM_OF_CLOSEST*sizeof(double));
		if(arr[i] == NULL){
			return NULL;
		}
		arr[i][0] = spRGBHistL2Distance(histA, histB[i], nBins);
		arr[i][1] = i;
	}

	/*the list of the closest images*/
	bests = (int*)malloc(NUM_OF_CLOSEST*sizeof(int));
	if(bests == NULL){
		return NULL;
	}

	/* extracting the images with the closest histograms by extracting the minimal in each level of the loop */
	for(int i =0;i<NUM_OF_CLOSEST;i++){
		min = 0;
		for(int j =0;j<num;j++){
			if(arr[min][0]<0){
				min=j;
			}
			if(arr[j][0]>=0 && arr[min][0]>=0 && arr[j][0]<arr[min][0]){
				min = j;
			}
			if(arr[j][0]>=0 && arr[min][0]>=0 && arr[j][0]==arr[min][0] && arr[j][1]<arr[min][1]){
				min = j;
			}
		}
		bests[i] = (int)(arr[min][1]);

		arr[min][0] = NEGATIVE_NUM;

	}
	for (int i=0; i<num; i++){
		free(arr[i]);
	}
	free(arr);
	return bests;
}

/* the function receives the query's SIFT for each feature
 * and a 3D array containing the SIFT descriptors for each feature in every image
 * and it returns the pictures with the closest features
 * to each of the query's features
 */
int* BestImgSiftDistance(double** querySift, double*** histDes,int queryFeatures,int* nFeatures,int numberOfImages){
	int** arr;
	int* bests;
	int max;
	int* shows;

	/* an array containing the images with the closest feature for each of the query's features */
	arr = (int**)malloc(queryFeatures*sizeof(int*));
	if(arr==NULL){
		return NULL;
	}
	for(int i=0; i<queryFeatures;i++){
		arr[i] = spBestSIFTL2SquaredDistance(NUM_OF_CLOSEST, querySift[i], histDes, numberOfImages, nFeatures);
		if(arr[i]==NULL)
			return NULL;
	}

	/* an array containing the number of shows per image in the list of the closest images for each feature */
	shows = (int*)malloc(numberOfImages*sizeof(int));
	if(shows==NULL){
		return NULL;
	}
	for(int i =0;i<numberOfImages;i++)
	{
		shows[i] = 0;
	}
	for(int i=0; i<queryFeatures;i++){
		for(int j=0;j<NUM_OF_CLOSEST;j++){
			shows[arr[i][j]] +=1;
		}
	}
	for (int i=0; i<queryFeatures; i++){
		free(arr[i]);
	}
	free(arr);

	/* extracting the images with the closest features by extracting the one with the maximal shows in each level of the loop */
	bests = (int*)malloc(NUM_OF_CLOSEST*sizeof(int));
	if(bests ==NULL){
		return NULL;
	}
	for(int i =0;i<NUM_OF_CLOSEST;i++){
		max = 0;
		for(int j =0;j<numberOfImages;j++){
			if(shows[j]>shows[max]){
				max = j;
			}
		}
		bests[i] = max;
		shows[max] = NEGATIVE_NUM;
	}
	free(shows);
	return bests;
}

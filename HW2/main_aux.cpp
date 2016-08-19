/*
 * main_aux.cpp
 *
 *  Created on: 4 ���� 2016
 *      Author: DorAlt
 */
#include "main_aux.h"
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <opencv2/xfeatures2d.hpp>//SiftDescriptorExtractor
#include <opencv2/features2d.hpp>


//the function get the directory of the images (dir + pref + num + suf) the number of bins in the picture
//and calculate the histogram for all images
int*** makeHist(char* dir, char* pref, int num, char* suf, int bins, int*** hist){
	hist = (int***)malloc(num*sizeof(int**));
	char imagePath[3*1024];
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
//the function get the directory of the images (dir + pref + num + suf) the number of features in the picture
//returns the sift arrays representing the pictures and how much features are in each picture
double*** makeSift(char* dir, char* pref, int num, char* suf, int fet, int* nFeatures){
	char imagePath[3*1024];
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
//the method get hisA and histB and calculates the closest histogram to histA from all the pictures
//which are represented in histB
int* bestHistogramDistances(int** histA, int*** histB, int nBins,int num){

	int* bests;
	int min;
	double** arr = (double**)malloc(num*sizeof(double*));
	if(arr == NULL){
		return NULL;
	}
	for(int i = 0;i<num;i++){
		arr[i] = (double*)malloc(5*sizeof(double));
		if(arr[i] == NULL){
			return NULL;
		}
		arr[i][0] = spRGBHistL2Distance(histA, histB[i], nBins);
		arr[i][1] = i;
	}

	bests = (int*)malloc(5*sizeof(int));
	if(bests == NULL){
		return NULL;
	}
	for(int i =0;i<5;i++){
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

		arr[min][0] = -1;

	}

	free(arr);
	return bests;
}
//calculates the closest 5 images to the query image using sift distance
int* BestImgSiftDistance(double** querySift, double*** histDes,int queryFeatures,int* nFeatures,int numberOfImages){
	int** arr;
	int* bests;
	int max;
	int* shows;

	arr = (int**)malloc(queryFeatures*sizeof(int*));
	if(arr==NULL){
		return NULL;
	}
	for(int i=0; i<queryFeatures;i++){
		arr[i] = spBestSIFTL2SquaredDistance(5, querySift[i], histDes, numberOfImages, nFeatures);
		if(arr[i]==NULL)
			return NULL;
	}
	shows = (int*)malloc(numberOfImages*sizeof(int));
	if(shows==NULL){
		return NULL;
	}
	for(int i =0;i<numberOfImages;i++)
	{
		shows[i] = 0;
	}
	for(int i=0; i<queryFeatures;i++){
		for(int j=0;j<5;j++){
			shows[arr[i][j]] +=1;
		}
	}
	free(arr);
	bests = (int*)malloc(5*sizeof(int));
	if(bests ==NULL){
		return NULL;
	}
	for(int i =0;i<5;i++){
		max = 0;
		for(int j =0;j<numberOfImages;j++){
			if(shows[j]>shows[max]){
				max = j;
			}
		}
		bests[i] = max;
		shows[max] = -1;
	}
	free(shows);
	return bests;
}

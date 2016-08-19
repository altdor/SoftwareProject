/*
 * main_aux.h
 *
 *  Created on: 4 במאי 2016
 *      Author: DorAlt
 */
#include "sp_image_proc_util.h"

int*** makeHist(char* dir, char* pref, int num, char* suf, int bins, int*** hist);
double*** makeSift(char* dir, char* pref, int num, char* suf,int fet, int* nFeatures);
int* bestHistogramDistances(int** histA, int*** histB, int nBins,int num);
int* BestImgSiftDistance(double** querySift, double*** histDes,int queryFeatures,int* nFeatures,int numberOfImages);

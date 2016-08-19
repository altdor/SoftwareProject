/*
 * sp_image_proc_util.cpp
 *
 *  Created on: 20 באפר 2016
 *      Author: DorAlt
 */
#include "sp_image_proc_util.h"
#include <opencv2/imgproc.hpp>//calcHist
#include <opencv2/core.hpp>//Mat
#include <opencv2/highgui.hpp>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <opencv2/xfeatures2d.hpp>//SiftDescriptorExtractor
#include <opencv2/features2d.hpp>
#include <cstdio>
#include <cstring>
using namespace cv;
/*
 * Calculates the RGB channels histogram. The histogram will be stored in a
 * two dimensional array of dimensions 3XnBins . The first row is the
 * red channel histogram, the second row is the green channel histogram
 * and the third row is the blue channel histogram.
 *
 * @param str - The path of the image for which the histogram will be calculated
 * @param nBins - The number of subdivision for the intensity histogram
 * @return NULL if str is NULL or nBins <= 0 or allocation error occurred,
 *  otherwise a two dimensional array representing the histogram.
 */
int** spGetRGBHist(char* str , int nBins){
	int** arr= (int**)malloc(3*sizeof(int*));
	if(arr == NULL || nBins<=0){
		return NULL;
	}

	for(int i =0;i<3;i++){
		arr[i]= (int*)malloc(nBins*sizeof(int));
		if(arr[i] == NULL){
			return NULL;
		}
	}

	Mat src;
	src = imread(str, CV_LOAD_IMAGE_COLOR);
	if (src.empty()) {
		return NULL;
	}
	std::vector<Mat> bgr_planes;
	split(src, bgr_planes);

	/// Set the ranges ( for B,G,R) )
	float range[] = { 0, 256 };
	const float* histRange = { range };
	/// Set the other parameters:
	int nImages = 1;
	//Output
	Mat b_hist, g_hist, r_hist;
	/// Compute the histograms:
	/// The results will be store in b_hist,g_hist,r_hist.
	/// The output type of the matrices is CV_32F (float)

	calcHist(&bgr_planes[0], nImages, 0, Mat(), b_hist, 1, &nBins, &histRange);
	calcHist(&bgr_planes[1], nImages, 0, Mat(), g_hist, 1, &nBins, &histRange);
	calcHist(&bgr_planes[2], nImages, 0, Mat(), r_hist, 1, &nBins, &histRange);


	for(int i = 0;i<nBins;i++){
		arr[0][i] = cvRound(r_hist.at<float>(i));
		arr[1][i] = cvRound(g_hist.at<float>(i));
		arr[2][i] = cvRound(b_hist.at<float>(i));
	}

	return arr;
}

/**
 * Returns the average L2-squared distance between histA and histB. The
 * histA and histB are histograms of the RGB channels. Both histograms
 * must be in the same dimension (3 X nBins).
 * @param histA - RGB histogram of image A
 * @param histB - RGB histogram of image B
 * @return -1 if nBins <= 0 or histA/histB is null, otherwise the average L@-squared distance.
 */
double spRGBHistL2Distance(int** histA, int** histB, int nBins){
	double redDis = 0;
	double greenDis = 0;
	double blueDis = 0;
	double avg;

	if(histA==NULL || histB==NULL || nBins<=0){

		return -1;
	}


	for(int i=0; i<nBins;i++){
		redDis += pow(histA[0][i] - histB[0][i],2);

		greenDis += pow(histA[1][i] - histB[1][i],2);

		blueDis += pow(histA[2][i] - histB[2][i],2);

	}

	avg = 0.33*redDis + 0.33*greenDis + 0.33*blueDis;

	return avg;
}

double** spGetSiftDescriptors(char* str, int maxNFeautres, int *nFeatures){
	//Loading img
	cv::Mat src;
	if(str == NULL || nFeatures == NULL || maxNFeautres <= 0){
		return NULL;
	}
	double** arr;

	src = cv::imread(str, CV_LOAD_IMAGE_GRAYSCALE);
	if (src.empty()) {
		return NULL;
	}

	//Key points will be stored in kp1;
	std::vector<cv::KeyPoint> kp1;
	//Feature values will be stored in ds1;
	cv::Mat ds1;
	//Creating  a Sift Descriptor extractor
	cv::Ptr<cv::xfeatures2d::SiftDescriptorExtractor> detect =
			cv::xfeatures2d::SIFT::create(maxNFeautres);
	//Extracting features
	//The features will be stored in ds1
	//The output type of ds1 is CV_32F (float)
	detect->detect(src, kp1, cv::Mat());
	detect->compute(src, kp1, ds1);
	arr= (double**)malloc(ds1.rows*sizeof(double*));
	if(arr == NULL)
		return NULL;
	for(int i =0;i<ds1.rows;i++){
		arr[i]= (double*)malloc(128*sizeof(double));
		if(arr[i] == NULL)
			return NULL;
	}
	for(int i = 0;i<ds1.rows;i++){
		for(int j =0;j<128;j++)
			arr[i][j] = ds1.at<float>(i,j);
	}
	*nFeatures = ds1.rows;

	return arr;

}
double spL2SquaredDistance(double* featureA, double* featureB){
	double distance = 0;
	if(featureA == NULL || featureB == NULL){
		return -1;
	}
	for(int i = 0; i<128;i++){
		distance += pow(featureA[i] - featureB[i],2);
	}

	return distance;
}
int* spBestSIFTL2SquaredDistance(int bestNFeatures, double* featureA,
		double*** databaseFeatures, int numberOfImages,
		int* nFeaturesPerImage){
		double** arr;
		int* bests;
		int min;
		int k = 0;
		int numOfFeatures = 0;
		int tempDis;
		if(featureA == NULL|| nFeaturesPerImage==NULL || databaseFeatures ==NULL || numberOfImages<=1)
			return NULL;
		for(int i = 0;i<numberOfImages;i++){
			numOfFeatures += nFeaturesPerImage[i];
		}
		arr= (double**)malloc(numOfFeatures*sizeof(double*));

		if(arr == NULL)
			return NULL;
		for(int i =0;i<numOfFeatures;i++){
			arr[i]= (double*)malloc(2*sizeof(double));
			if(arr[i] == NULL)
				return NULL;
		}

		for(int i = 0;i<numberOfImages;i++){
			for(int j= 0; j<nFeaturesPerImage[i];j++){
				tempDis  = spL2SquaredDistance(featureA, databaseFeatures[i][j]);
				arr[k][0] = tempDis;
				arr[k][1] = i;
				k++;

			}
		}
		bests = (int*)malloc(bestNFeatures*sizeof(int));
		for(int i =0;i<bestNFeatures;i++){
			min = 0;
			for(int j =0;j<numOfFeatures;j++){
				if(arr[min][0]<0){
					min=j;
				}
				if(arr[j][0]>=0 && arr[min][0]>=0 && arr[j][0]<arr[min][0])
					min = j;
				if(arr[j][0]==arr[min][0] && arr[j][1]<arr[min][1])
					min = j;
			}
			bests[i] = (int)(arr[min][1]);
			arr[min][0] = -1;
		}
		free(arr);

		return bests;
}

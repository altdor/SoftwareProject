/*
 * SPKDArray.c
 *
 *  Created on: 25 ���� 2016
 *      Author: yahav
 */
#include "SPKDArray.h"
#include "SPPoint.c"
#include <stdio.h>
#include <stdlib.h>

struct sp_kdarray_t{
	SPPoint* pointArr;
	int size;
	int** indMat;
};

SPKDArray spKdarrayInit(SPPoint* arr, int size){
	if (arr==NULL || int<=0)
		return NULL;
	int i,j;
	int d = spPointGetDimension(*arr); //need to check if necessary to check that all the dimensions are equall
	SPKDArray kda = (SPKDArray)malloc(sizeof(*kda));
	SPPoint* p = (SPPoint)malloc(size*sizeof(SPPoint));
	if(p == NULL){//allocation fails
		return NULL;
	}
	int** a = (int**)malloc(d*sizeof(int*));
	if(a == NULL){//allocation fails
		return NULL;
	}
	for (i=0; i<d; i++){
		(*a)[i] = (int*)malloc(size*sizeof(int));
		if((*a)[i] == NULL){//allocation fails
			return NULL;
		}
	}
	for (i=0; i<size; i++){
		p[i] = arr[i];
	}
	for (i=0; i<d; i++){
		qsort_r(p,size,sizeof(SPPoint),compByAxis,i);
		for(j=0; j<size; j++){
			a[i][j] = p[j];
		}
	}
	kda->size = size;
	for (i=0; i<d; i++){
		kda->pointArr[i] = arr[i];
		for(j=0; j<size; j++){
			kda->indMat[i][j] = a[i][j];
		}
	}
	return kda;
}

SPKDArray spKdarraySplit(SPKDArray kdArr, int coor);{

}

int compByAxis (const void* p1, const void* p2, void* axis){
    int a = (int)spPointGetAxisCoor(*(SPPoint*)p1,*(int*)axis);
    int b = (int)spPointGetAxisCoor(*(SPPoint*)p2,*(int*)axis);
    if (a > b) return  1;
    if (a < b) return -1;
    return 0;
}


/*
 * SPKDArray.c
 *
 *  Created on: 25 αιεμ 2016
 *      Author: yahav
 */
#include "SPKDArray.h"
#include "SPPoint.h"
#include <stdio.h>
#include <stdlib.h>
#define ABORT -1
#define START -1
#define NEGATIVE -1
#define INIT 0
#define FIRST 0
#define EQUAL 0
#define POSITIVE 1
#define SECOND 1

struct sp_kdarray_t{
	SPPoint* pointArr;
	int size;
	int** indMat;
	int dim;
};

struct sp_pcoor_t{
	double val;
	int index;
};

void spCoorSetIndex(SPPCoor spcoor,int index){
	if(spcoor == NULL)
		return;
	spcoor->index = index;
}

void spCoorSetVal(SPPCoor spcoor,double val){
	if(spcoor == NULL)
		return;
	spcoor->val = val;
}

int spCoorGetIndex(SPPCoor spcoor){
	if(spcoor == NULL)
		return ABORT;
	return spcoor->index;
}

double spCoorGetVal(SPPCoor spcoor){
	if(spcoor == NULL)
		return ABORT;
	return spcoor->val;
}

SPKDArray spKdarrayInit(SPPoint* arr, int size){
	int i,j,d,k;
	SPPCoor* p;
	int** a;
	SPKDArray kda;
	if (arr==NULL || size<=0)
			return NULL;
	kda = (SPKDArray)malloc(sizeof(*kda));
	if(kda == NULL){
		return NULL;
	}
	p = (SPPCoor*)malloc(size*sizeof(*p));
	if(p == NULL){
		free(kda);
		return NULL;
	}
	d = spPointGetDimension(*arr);
	kda->dim = d;
	a = (int**)malloc(d*sizeof(int*));
	if(a == NULL){
		free(kda);
		free(p);
		return NULL;
	}
	for (i=0; i<d; i++){
		a[i] = (int*)malloc(size*sizeof(int));
		if(a[i] == NULL){
			free(kda);
			free(p);
			free(a);
			return NULL;
		}
	}
	kda->pointArr = (SPPoint*)malloc(size*sizeof(SPPoint));
	if(kda->pointArr==NULL){
		free(kda);
		free(p);
		for (i=0; i<d; i++){
			free(a[i]);
		}
		free(a);
		return NULL;
	}
	kda->indMat = (int**)malloc(d*sizeof(int*));
	for (i=0; i<d; i++){
		kda->indMat[i] = (int*)malloc(size*sizeof(int));
		if(kda->indMat[i]==NULL){
			for(int k=0;k<i;k++){
				free(kda->indMat[k]);
			}
			free(kda->indMat);
			for(int k=0;k<size;k++){
				free(kda->indMat[k]);
			}
			free(kda->pointArr);
			free(kda);
			free(p);
			for (i=0; i<d; i++){
				free(a[i]);
			}
			free(a);
			return NULL;
		}
	}

	for(j=0; j<size; j++){
		p[j] = (SPPCoor)malloc(sizeof(SPPCoor));
		if(p[j]==NULL){
			for(k=0;k<j;k++){
				free(p[k]);
			}
			spKDArrayDestroy(kda);
			free(p);
			for (i=0; i<d; i++){
				free(a[i]);
			}
			free(a);
			return NULL;
		}
	}
	for (i=0; i<d; i++){
		for(j=0; j<size; j++){
			p[j]->val = spPointGetAxisCoor(arr[j],i);
			p[j]->index=j;
		}

		qsort(p,size,sizeof(SPPCoor),compByAxis);

		for(j=0; j<size; j++){
			a[i][j] = p[j]->index;
		}
	}
	kda->size = size;
	for (i=0; i<size; i++){
		kda->pointArr[i] = spPointCopy(arr[i]);
	}
	for (i=0; i<d; i++){
		for(j=0; j<size; j++){
			kda->indMat[i][j] = a[i][j];
		}
	}
	for (i=0; i<d; i++){
		free(a[i]);
	}
	free(a);
	for(j=0; j<size; j++){
		free(p[j]);
	}
	free(p);
	return kda;
}

SPKDArray* spKdarraySplit(SPKDArray kdArr, int coor){
	int i,j,d,s,ind,pointerL,pointerR,h,g,leftHalf,rightHalf;
	int* x;
	int* map1;
	int* map2;
	SPPoint* p1;
	SPPoint* p2;
	SPKDArray kdLeft;
	SPKDArray kdRight;
	SPKDArray* splitted;
	if (kdArr==NULL||coor<0)
		return NULL;
	h=INIT;
	g=INIT;
	d = spPointGetDimension(*kdArr->pointArr);
	s = kdArr->size;
	x = (int*)malloc(s*sizeof(int));
	if (x==NULL){
		return NULL;
	}
	map1 = (int*)malloc(s*sizeof(int));
	if (map1==NULL){
		free(x);
		return NULL;
	}
	map2 = (int*)malloc(s*sizeof(int));
	if (map2==NULL){
		free(x);
		free(map1);
		return NULL;
	}
	leftHalf = (s-(s/2));
	rightHalf = (s/2);
	p1 = (SPPoint*)malloc(leftHalf*sizeof(*p1));
	if (p1==NULL){
		free(x);
		free(map1);
		free(map2);
		return NULL;
	}
	p2 = (SPPoint*)malloc(rightHalf*sizeof(*p2));
	if (p2==NULL){
		free(x);
		free(map1);
		free(map2);
		free(p1);
		return NULL;
	}
	kdLeft = (SPKDArray)malloc(sizeof(*kdLeft));
	if (kdLeft==NULL){
		free(x);
		free(map1);
		free(map2);
		free(p1);
		free(p2);
		return NULL;
	}
	kdRight = (SPKDArray)malloc(sizeof(*kdRight));
	if (kdRight==NULL){
		free(x);
		free(map1);
		free(map2);
		free(p1);
		free(p2);
		free(kdLeft);
		return NULL;
	}
	kdLeft->dim = d;
	kdRight->dim = d;
	splitted = (SPKDArray*)malloc(2*sizeof(SPKDArray));
	if (splitted==NULL){
		free(x);
		free(map1);
		free(map2);
		free(p1);
		free(p2);
		free(kdLeft);
		free(kdRight);
		return NULL;
	}


	kdLeft->indMat = (int**)malloc(d*sizeof(int*));
	if (kdLeft->indMat==NULL){
		free(splitted);
		free(x);
		free(map1);
		free(map2);
		free(p1);
		free(p2);
		free(kdLeft);
		free(kdRight);
		return NULL;
	}
	kdRight->indMat = (int**)malloc(d*sizeof(int*));
	if (kdRight->indMat==NULL){
		free(splitted);
		free(x);
		free(map1);
		free(map2);
		free(p1);
		free(p2);
		free(kdLeft->indMat);
		free(kdLeft);
		free(kdRight);
		return NULL;
	}
	for (i=0; i<s; i++){
		map1[i]=START;
		map2[i]=START;
	}
	for (i=0; i<leftHalf; i++){
		x[(kdArr->indMat)[coor][i]] = 0;
	}

	for (i=leftHalf; i<s; i++){
		x[(kdArr->indMat)[coor][i]] = 1;
	}

	for (i=0; i<s; i++){
		if (x[i]==0){
			p1[h] = kdArr->pointArr[i];
			map1[i]=h;
			h++;
		}
		else{
			p2[g] = kdArr->pointArr[i];
			map2[i]=g;
			g++;
		}
	}
	for (i=0; i<d; i++){
		kdLeft->indMat[i] = (int*)malloc(leftHalf*sizeof(int));
		if (kdLeft->indMat[i]==NULL){
			for(int k=0;k<i;k++){
				free(kdLeft->indMat[k]);
			}
			free(splitted);
			free(x);
			free(map1);
			free(map2);
			free(p1);
			free(p2);
			free(kdLeft->indMat);
			free(kdRight->indMat);
			free(kdLeft);
			free(kdRight);
			return NULL;
		}
	}

	for (i=0; i<d; i++){
		kdRight->indMat[i] = (int*)malloc(rightHalf*sizeof(int));
		if (kdRight->indMat[i]==NULL){
			for(int k=0;k<d;k++){
				free(kdLeft->indMat[k]);
			}
			for(int k=0;k<i;k++){
				free(kdRight->indMat[k]);
			}
			free(splitted);
			free(x);
			free(map1);
			free(map2);
			free(p1);
			free(p2);
			free(kdLeft->indMat);
			free(kdRight->indMat);
			free(kdLeft);
			free(kdRight);
			return NULL;
		}
		pointerL = 0;
		pointerR = 0;
		for (j=0; j<s; j++){
			ind = kdArr->indMat[i][j];
			if (x[ind]==0){
				kdLeft->indMat[i][pointerL] = map1[ind];
				pointerL++;
			}
			else {
				kdRight->indMat[i][pointerR] = map2[ind];
				pointerR++;
			}
		}
	}

	kdLeft->pointArr = p1;
	kdRight->pointArr = p2;
	kdLeft->size = leftHalf;
	kdRight->size = rightHalf;
	splitted[FIRST] = kdLeft;
	splitted[SECOND] = kdRight;
	free(x);
	free(map1);
	free(map2);
	return splitted;
}

int compByAxis(const void* p1, const void* p2){
	SPPCoor a = *(SPPCoor*)p1;
    SPPCoor b = *(SPPCoor*)p2;
    if (a->val>b->val)
    	return POSITIVE;
    else if (a->val<b->val)
    	return NEGATIVE;
    else
    	return EQUAL;
}

int spKdarrayGetSize(SPKDArray arr){
	if (arr==NULL)
		return ABORT;
	return arr->size;
}

SPPoint* spKdarrayGetPointAraay(SPKDArray arr){
	if (arr==NULL)
		return NULL;
	return arr->pointArr;
}
void spKDArrayDestroy(SPKDArray arr){
	int i;
	if(arr==NULL)
		return;
	for(i =0; i<arr->size;i++){
		spPointDestroy(arr->pointArr[i]);
	}
	for(i=0; i<arr->dim; i++){
		free(arr->indMat[i]);
	}
	free(arr->pointArr);
	if(arr->indMat!=NULL)
		free(arr->indMat);
	free(arr);
}

void splittedDestroy(SPKDArray* splitted){
	int i;
	if (splitted==NULL)
		return;
	free(splitted[FIRST]->pointArr);
	free(splitted[SECOND]->pointArr);
	for(i=0; i<splitted[FIRST]->dim; i++){
		free(splitted[FIRST]->indMat[i]);
	}
	for(i=0; i<splitted[SECOND]->dim; i++){
		free(splitted[SECOND]->indMat[i]);
	}
	free(splitted[FIRST]->indMat);
	free(splitted[SECOND]->indMat);
	free(splitted[FIRST]);
	free(splitted[SECOND]);
	free(splitted);
}

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

struct sp_kdarray_t{
	SPPoint* pointArr;
	int size;
	int** indMat;
};

/** Type for defining the SPPoints that can be sorted by specific axis coordinate **/
struct sp_pcoor_t{
	SPPoint point;
	int axis;
	int index;
};

SPKDArray spKdarrayInit(SPPoint* arr, int size){
	int i,j;
	int d;
	SPPCoor* p;
	int** a;
	SPKDArray kda;
	SPPoint* tmpArray;
	SPPCoor pcoor;
	if (arr==NULL || size<=0)
			return NULL;
	kda = (SPKDArray)malloc(sizeof(*kda));
	if(kda == NULL){//allocation fails
		return NULL;
	}
	p = (SPPCoor*)malloc(size*sizeof(SPPCoor));
	if(p == NULL){//allocation fails
		free(kda);
		return NULL;
	}
	d = spPointGetDimension(*arr);//TODO: need to check if necessary to check that all the dimensions are equall
	a = (int**)malloc(d*sizeof(int*));
	if(a == NULL){//allocation fails
		free(kda);
		free(p);
		return NULL;
	}
	tmpArray = (SPPoint*)malloc(size*sizeof(*tmpArray));
	if(tmpArray == NULL){//allocation fails
		free(kda);
		free(p);
		free(a);
		return NULL;
	}
	for (i=0; i<d; i++){
		a[i] = (int*)malloc(size*sizeof(int));
		if(a[i] == NULL){//allocation fails
			free(kda);
			free(p);
			free(a);
			free(tmpArray);
			return NULL;
		}
	}
	pcoor = (SPPCoor)malloc(sizeof(*pcoor));
	if(pcoor == NULL){//allocation fails
		free(kda);
		free(p);
		free(a);
		free(tmpArray);
	}
	for (i=0; i<size; i++){
		tmpArray[i] = spPointCopy(arr[i]);
	}
	for (i=0; i<d; i++){
		for(j=0; j<size; j++){
			pcoor->axis=i;
			pcoor->point = tmpArray[j];
			pcoor->index=j;
			p[j] = pcoor;
		}
		qsort(p,size,sizeof(SPPCoor),compByAxis);
		for(j=0; j<size; j++){
			a[i][j] = p[j]->index;
		}
	}
	kda->size = size;
	for (i=0; i<d; i++){
		kda->pointArr[i] = spPointCopy(arr[i]);
		for(j=0; j<size; j++){
			kda->indMat[i][j] = a[i][j];
		}
	}
	return kda;
}

SPKDArray* spKdarraySplit(SPKDArray kdArr, int coor){
	int i,j,d,s,ind,pointerL,pointerR;
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
	d = spPointGetDimension(*kdArr->pointArr);
	s = kdArr->size;
	pointerR = 0;
	pointerL = 0;
	x = (int*)malloc(s*sizeof(int));
	map1 = (int*)malloc(s*sizeof(int));
	map2 = (int*)malloc(s*sizeof(int));
	p1 = (SPPoint*)malloc((s-(s/2))*sizeof(*p1));
	p2 = (SPPoint*)malloc((s/2)*sizeof(*p2));
	kdLeft = (SPKDArray)malloc(sizeof(*kdLeft));
	kdRight = (SPKDArray)malloc(sizeof(*kdRight));
	splitted = (SPKDArray*)malloc(2*sizeof(SPKDArray)); //TODO: add all the allocation fails and the correct frees
	if(x == NULL)//allocation fails
		return NULL;
	if(p1 == NULL){//allocation fails
		free(x);
		return NULL;
	}
	if(p2 == NULL){//allocation fails
		free(x);
		free(p1);
		return NULL;
	}

	//creating the arrays x, p1, p2, map1 and map2 as instructed

	for (i=0; i<s; i++){
		map1[i]=-1;
		map2[i]=-1;
	}
	for (i=0; i<(s-(s/2)); i++){
		x[(kdArr->indMat)[coor][i]] = 0;
		p1[i] = (kdArr->pointArr)[(kdArr->indMat)[coor][i]];
		map1[kdArr->indMat[coor][i]]=i;
	}
	for (i=(s-(s/2)); i<s; i++){
		x[(kdArr->indMat)[coor][i]] = 1;
		p2[i-(s-(s/2))] = (kdArr->pointArr)[(kdArr->indMat)[coor][i]];
		map2[kdArr->indMat[coor][i]]=i-(s/2);
	}

	//creating the matrices A1 & A2 as instructed

	for (i=0; i<d; i++){
		for (j=0; j<s; j++){
			ind = kdArr->indMat[i][j];
			if (x[ind]==0){
				kdLeft->indMat[i][pointerL] = map1[ind];
				pointerL++;
			}
			if (x[ind]==1){
				kdRight->indMat[i][pointerR] = map2[ind];
				pointerR++;
			}
		}
	}
	kdLeft->pointArr = p1;
	kdRight->pointArr = p2;
	kdLeft->size = (s-(s/2));
	kdRight->size = (s/2);
	splitted[0] = kdLeft;
	splitted[1] = kdRight;
	return splitted;
}

int compByAxis(const void* p1, const void* p2){
    SPPCoor a = (SPPCoor)p1;
    SPPCoor b = (SPPCoor)p2;
    return ((spPointGetAxisCoor(a->point,a->axis))-(spPointGetAxisCoor(b->point,b->axis)));
}

int main2(){
	int a = 3;
	printf("%d",a);
	return 0;
}

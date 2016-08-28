/*
 * SPKDArray.c
 *
 *  Created on: 25 ���� 2016
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
	for (i=0; i<d; i++){
		a[i] = (int*)malloc(size*sizeof(int));
		if(a[i] == NULL){//allocation fails
			free(kda);
			free(p);
			free(a);
			return NULL;
		}
	}
	kda->pointArr = (SPPoint*)malloc(size*sizeof(SPPoint));
	for (i=0; i<size; i++){
		kda->pointArr[i] = (SPPoint)malloc(sizeof(SPPoint));
	}
	kda->indMat = (int**)malloc(d*size*sizeof(int));
	for (i=0; i<d; i++){
		kda->indMat[i] = (int*)malloc(size*sizeof(int));
	}

	for(j=0; j<size; j++){
		p[j] = (SPPCoor)malloc(sizeof(SPPCoor));
	}
	for (i=0; i<d; i++){
		for(j=0; j<size; j++){
			p[j]->axis=i;
			p[j]->point = spPointCopy(arr[j]);
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
	x = (int*)malloc(s*sizeof(int));
	map1 = (int*)malloc(s*sizeof(int));
	map2 = (int*)malloc(s*sizeof(int));
	p1 = (SPPoint*)malloc((s-(s/2))*sizeof(*p1));
	p2 = (SPPoint*)malloc((s/2)*sizeof(*p2));
	kdLeft = (SPKDArray)malloc(sizeof(*kdLeft));
	kdRight = (SPKDArray)malloc(sizeof(*kdRight));
	splitted = (SPKDArray*)malloc(2*sizeof(SPKDArray)*s); //TODO: add all the allocation fails and the correct frees
	for (i=0; i<2; i++){
		splitted[i] = (SPKDArray)malloc(sizeof(SPKDArray)*s);
	}
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
	kdLeft->pointArr = (SPPoint*)malloc((s-(s/2))*sizeof(SPPoint));
	kdRight->pointArr = (SPPoint*)malloc((s/2)*sizeof(SPPoint));
	for (i=0; i<(s-(s/2)); i++){
		kdLeft->pointArr[i] = (SPPoint)malloc(sizeof(SPPoint));
	}
	for (i=0; i<(s/2); i++){
		kdRight->pointArr[i] = (SPPoint)malloc(sizeof(SPPoint));
	}
	kdLeft->indMat = (int**)malloc(d*(s-(s/2))*sizeof(int));
	kdRight->indMat = (int**)malloc(d*(s/2)*sizeof(int));
	for (i=0; i<d; i++){
		kdLeft->indMat[i] = (int*)malloc((s-(s/2))*sizeof(int));
		kdRight->indMat[i] = (int*)malloc((s/2)*sizeof(int));
	}

	//creating the arrays x, p1, p2, map1 and map2 as instructed

	for (i=0; i<s; i++){
		map1[i]=-1;
		map2[i]=-1;
	}
	for (i=0; i<(s-(s/2)); i++){
		x[(kdArr->indMat)[coor][i]] = 0;
		p1[i] = (SPPoint)malloc(sizeof(SPPoint));
		p1[i]= kdArr->pointArr[kdArr->indMat[coor][i]];
		map1[kdArr->indMat[coor][i]]=i;
	}
	for (i=(s-(s/2)); i<s; i++){
		x[(kdArr->indMat)[coor][i]] = 1;
		p2[i-(s-(s/2))] = (SPPoint)malloc(sizeof(SPPoint));
		p2[i-(s-(s/2))] = kdArr->pointArr[kdArr->indMat[coor][i]];
		map2[kdArr->indMat[coor][i]]=i-(s-(s/2));
	}

	//creating the matrices A1 & A2 as instructed

	for (i=0; i<d; i++){
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
	kdLeft->size = (s-(s/2));
	kdRight->size = (s/2);
	splitted[0] = kdLeft;
	splitted[1] = kdRight;
	return splitted;
}

int compByAxis(const void* p1, const void* p2){
	SPPCoor a = *(SPPCoor*)p1;
    SPPCoor b = *(SPPCoor*)p2;
    return (((int)spPointGetAxisCoor(a->point,a->axis))-((int)spPointGetAxisCoor(b->point,b->axis)));
}

/*int main2(){
	int i,j;
	int size = 5;
	int dim = 2;
	SPKDArray* sparr;
	SPKDArray ail1, ail2, kdarr;

	SPPoint* arr = (SPPoint*)malloc(sizeof(SPPoint)*size);
	ail1 = (SPKDArray)malloc(sizeof(*ail1));
	ail2 = (SPKDArray)malloc(sizeof(*ail2));

	double a1[2]= {1,2};
	double b1[2] = {123,70};
	double c1[2] = {2,7};
	double d1[2] = {9,11};
	double e1[2]= {3,4};

	arr[0] = (spPointCreate(a1,dim,0));
	arr[1] = (spPointCreate(b1,dim,1));
	arr[2] = (spPointCreate(c1,dim,2));
	arr[3] = (spPointCreate(d1,dim,3));
	arr[4] = (spPointCreate(e1,dim,4));

	kdarr = spKdarrayInit(arr,size);

	printf("mat of first kdarray is: \n");
	fflush(NULL);
	for (i = 0; i<spPointGetDimension(*arr); i++){
		for (j = 0; j < size;j++) {
			printf("%d ",kdarr->indMat[i][j]);
			fflush(NULL);
		}
		printf("\n");
		fflush(NULL);
	}

	sparr = spKdarraySplit(kdarr,0);
	ail1 = sparr[0];
	ail2 = sparr[1];

	printf("points of left kdarray are: \n");
	for (i = 0; i < 3; ++i) {
		printf("%d place - (%f1,%f1)\n",i,spPointGetAxisCoor(ail1->pointArr[i],0),spPointGetAxisCoor(ail1->pointArr[i],1));
	}
	printf("\n");

	printf("mat of left kdarray is: \n");
	for (i = 0; i < 2; ++i) {
		for (j = 0; j < 3; ++j) {
			printf("%d ",ail1->indMat[i][j]);
		}
		printf("\n");
	}
	printf("points of right kdarray are: \n");
	for (i = 0; i < 2; ++i) {
		printf("%d place - (%f1,%f1)\n",i,spPointGetAxisCoor(ail2->pointArr[i],0),spPointGetAxisCoor(ail2->pointArr[i],1));
	}
	printf("\n");
	printf("mat of right kdarray is: \n");
	for (i = 0; i < 2; ++i) {
		for (j = 0; j < 2; ++j) {
			printf("%d ",ail2->indMat[i][j]);
		}
		printf("\n");
	}

	return 0;
}*/

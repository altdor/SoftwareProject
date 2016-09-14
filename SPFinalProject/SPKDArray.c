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
		return -1;
	return spcoor->index;
}

double spCoorGetVal(SPPCoor spcoor){
	if(spcoor == NULL)
		return -1;
	return spcoor->val;
}
/*struct sp_pcoor_t{
	SPPoint point;
	int axis;
	int index;
};
void spCoorSetPoint(SPPCoor spcoor,SPPoint p){
	if(spcoor == NULL)
		return;
	spcoor->point = p;
}
void spCoorSetAxis(SPPCoor spcoor,int axis){
	if(spcoor == NULL)
		return;
	spcoor->axis = axis;
}
void spCoorSetIndex(SPPCoor spcoor,int index){
	if(spcoor == NULL)
		return;
	spcoor->index = index;
}
SPPoint spCoorGetPoint(SPPCoor spcoor){
	if(spcoor == NULL)
		return NULL;
	return spcoor->point;
}
int spCoorGetAxis(SPPCoor spcoor){
	if(spcoor == NULL)
		return -1;
	return spcoor->axis;
}
int spCoorGetIndex(SPPCoor spcoor){
	if(spcoor == NULL)
		return -1;
	return spcoor->index;
}*/
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
	a = (int**)malloc(d*size*sizeof(int*));
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
	for (i=0; i<size; i++){
		kda->pointArr[i] = (SPPoint)malloc(sizeof(SPPoint));
		if(kda->pointArr[i]==NULL){
			for(int k=0;k<i;k++){
				free(kda->pointArr[k]);
			}
			free(kda->pointArr);
			free(kda);
			free(p);
			free(a);
			return NULL;
		}
	}
	kda->indMat = (int**)malloc(d*size*sizeof(int));
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
			free(a);
			return NULL;
		}
	}
	/*for (i=0; i<d; i++){
		for(j=0; j<size; j++){
			p[j]->axis=i;
			p[j]->point = spPointCopy(arr[j]);
			p[j]->index=j;
		}

		qsort(p,size,sizeof(SPPCoor),compByAxis);

		for(j=0; j<size; j++){
			a[i][j] = p[j]->index;
		}
	}*/
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
	return kda;
}

SPKDArray* spKdarraySplit(SPKDArray kdArr, int coor){
	int i,j,d,s,ind,pointerL,pointerR,h,g;
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
	h=0;
	g=0;
	printf("a");
	fflush(NULL);
	d = spPointGetDimension(*kdArr->pointArr);
	printf("b");
	fflush(NULL);
	s = kdArr->size;
	printf("c");
	fflush(NULL);
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
	p1 = (SPPoint*)malloc((s-(s/2))*sizeof(*p1));
	if (p1==NULL){
		free(x);
		free(map1);
		free(map2);
		return NULL;
	}
	p2 = (SPPoint*)malloc((s/2)*sizeof(*p2));
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
	splitted = (SPKDArray*)malloc(2*sizeof(SPKDArray)*s);
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
	for (i=0; i<2; i++){
		splitted[i] = (SPKDArray)malloc(sizeof(SPKDArray)*s);
		if (splitted[i]==NULL){
			free(splitted[0]);
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
	}
	kdLeft->pointArr = (SPPoint*)malloc((s-(s/2))*sizeof(SPPoint));
	if (kdLeft->pointArr==NULL){
		for(int k=0;k<2;k++)
			free(splitted[k]);
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
	kdRight->pointArr = (SPPoint*)malloc((s/2)*sizeof(SPPoint));
	if (kdRight->pointArr==NULL){
		for(int k=0;k<2;k++){
			free(splitted[k]);
		}
		free(splitted);
		free(x);
		free(map1);
		free(map2);
		free(p1);
		free(p2);
		free(kdLeft->pointArr);
		free(kdLeft);
		free(kdRight);
		return NULL;
	}

	kdLeft->indMat = (int**)malloc(d*(s-(s/2))*sizeof(int));
	if (kdLeft->indMat==NULL){
		for(int k=0;k<2;k++)
			free(splitted[k]);
		free(splitted);
		free(x);
		free(map1);
		free(map2);
		free(p1);
		free(p2);
		free(kdRight->pointArr);
		free(kdLeft->pointArr);
		free(kdLeft);
		free(kdRight);
		return NULL;
	}
	kdRight->indMat = (int**)malloc(d*(s/2)*sizeof(int));
	if (kdRight->indMat==NULL){
		for(int k=0;k<2;k++)
			free(splitted[k]);
		free(splitted);
		free(x);
		free(map1);
		free(map2);
		free(p1);
		free(p2);
		free(kdLeft->indMat);
		free(kdRight->pointArr);
		free(kdLeft->pointArr);
		free(kdLeft);
		free(kdRight);
		return NULL;
	}
	/*for (i=0; i<d; i++){
		kdLeft->indMat[i] = (int*)malloc((s-(s/2))*sizeof(int));
		kdRight->indMat[i] = (int*)malloc((s/2)*sizeof(int));
		if (kdRight->indMat[i]==NULL || kdLeft->indMat[i]==NULL){
			for(int k=0;k<2;k++)
				free(splitted[k]);
			for(int k=0;k<i;k++){
				free(kdRight->indMat[k]);
				free(kdLeft->indMat[k]);
			}
			free(splitted);
			free(x);
			free(map1);
			free(map2);
			free(p1);
			free(p2);
			free(kdLeft->indMat);
			free(kdRight->pointArr);
			free(kdRight->indMat);
			free(kdLeft->pointArr);
			free(kdLeft);
			free(kdRight);
			return NULL;
		}
		for (j=0; j<(s-(s/2)); j++){
			kdLeft->indMat[i][j] = 0;
		}
		for (j=0; j<(s/2); j++){
			kdRight->indMat[i][j] = 0;
		}
	}*/
	printf("d");
	fflush(NULL);
	for (i=0; i<s; i++){
		map1[i]=-1;
		map2[i]=-1;
	}
	printf("e");
	fflush(NULL);
	for (i=0; i<(s-(s/2)); i++){
		x[(kdArr->indMat)[coor][i]] = 0;
	}
	printf("f");
	fflush(NULL);
	for (i=(s-(s/2)); i<s; i++){
		x[(kdArr->indMat)[coor][i]] = 1;
	}
	printf("g");
	fflush(NULL);
	for (i=0; i<s; i++){
		if (x[i]==0){
			//p1[h] = spPointCopy(kdArr->pointArr[i]);
			p1[h] = kdArr->pointArr[i];
			map1[i]=h;
			h++;
		}
		else{
			//p2[g] = spPointCopy(kdArr->pointArr[i]);
			p2[g] = kdArr->pointArr[i];
			map2[i]=g;
			g++;
		}
	}
	for (i=0; i<d; i++){
			kdLeft->indMat[i] = (int*)malloc((s-(s/2))*sizeof(int));
			kdRight->indMat[i] = (int*)malloc((s/2)*sizeof(int));
			if (kdRight->indMat[i]==NULL || kdLeft->indMat[i]==NULL){
				for(int k=0;k<2;k++)
					free(splitted[k]);
				for(int k=0;k<i;k++){
					free(kdRight->indMat[k]);
					free(kdLeft->indMat[k]);
				}
				free(splitted);
				free(x);
				free(map1);
				free(map2);
				free(p1);
				free(p2);
				free(kdLeft->indMat);
				free(kdRight->pointArr);
				free(kdRight->indMat);
				free(kdLeft->pointArr);
				free(kdLeft);
				free(kdRight);
				return NULL;
			}
			for (j=0; j<(s-(s/2)); j++){
				kdLeft->indMat[i][j] = 0;
			}
			for (j=0; j<(s/2); j++){
				kdRight->indMat[i][j] = 0;
			}
			printf("leftMat = %d\n",kdLeft->indMat[i][0]);
			printf("rightMat = %d\n",kdRight->indMat[i][0]);
			printf("i = %d\n",i);
		}
	for (i=0; i<d; i++){
		kdRight->indMat[i][0] = 0;
		printf("i = %d",i);
		fflush(NULL);
	}
	printf("h\n");
	fflush(NULL);
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
	printf("i");
	fflush(NULL);
	kdLeft->pointArr = p1;
	kdRight->pointArr = p2;
	kdLeft->size = (s-(s/2));
	kdRight->size = (s/2);
	splitted[0] = kdLeft;
	splitted[1] = kdRight;
	printf("j");
	fflush(NULL);
	free(x);
	printf("k");
	fflush(NULL);
	free(map1);
	printf("l");
	fflush(NULL);
	free(map2);
	printf("m\n");
	fflush(NULL);
	return splitted;
}

int compByAxis(const void* p1, const void* p2){
	SPPCoor a = *(SPPCoor*)p1;
    SPPCoor b = *(SPPCoor*)p2;
    if (a->val>b->val)
    	return 1;
    else if (a->val<b->val)
    	return -1;
    else
    	return 0;
    //return (((int)spPointGetAxisCoor(a->point,a->axis))-((int)spPointGetAxisCoor(b->point,b->axis)));
}

int spKdarrayGetSize(SPKDArray arr){
	if (arr==NULL)
		return -1;
	return arr->size;
}

SPPoint* spKdarrayGetPointAraay(SPKDArray arr){
	if (arr==NULL)
		return NULL;
	return arr->pointArr;
}
void spKDArrayDestroy(SPKDArray arr){
	if(arr==NULL)
		return;
	for(int i =0; i<spPointGetDimension(*(arr->pointArr));i++){
		free(arr->indMat[i]);
	}
	for(int i =0; i<arr->size;i++){
			spPointDestroy(arr->pointArr[i]);
	}
	free(arr->pointArr);
	free(arr->indMat);
	free(arr);
}


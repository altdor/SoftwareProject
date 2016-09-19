/*
 * SPPoint.c
 *
 *  Created on: 21 במאי 2016
 *      Author: DorAlt
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SPPoint.h"

struct sp_point_t{
	double* data;
	int index;
	int dim;
};

SPPoint spPointCreate(double* data, int dim, int index){
	if(data==NULL || dim<=0 || index<0){
		return NULL;
	}
	SPPoint point = (SPPoint)malloc(sizeof(*point));
	if(point == NULL){//allocation fails
		return NULL;
	}
	point->data = (double*)malloc(dim*sizeof(double));
	if(point->data == NULL){//allocation fails
		free(point);
		return NULL;
	}

	for(int i=0; i<dim;i++){
		point->data[i] = data[i];
	}
	point->index = index;
	point->dim = dim;
	return point;

}

SPPoint spPointCopy(SPPoint source){
	assert(source!=NULL);
	return spPointCreate(source->data, source->dim, source->index);
}

void spPointDestroy(SPPoint point){
	if (point == NULL)
		return;
	free(point->data);
	free(point);
}

int spPointGetDimension(SPPoint point){
	assert(point!=NULL);
	return point->dim;
}

int spPointGetIndex(SPPoint point){
	assert(point!=NULL);
	return point->index;
}

double spPointGetAxisCoor(SPPoint point, int axis){
	assert(point!=NULL);
	assert(axis<point->dim);
	return point->data[axis];
}
double* spPointGetData(SPPoint point){
	assert(point!=NULL);
	return point->data;
}

double spPointL2SquaredDistance(SPPoint p, SPPoint q){
	assert(p!=NULL);
	assert(q!=NULL);
	assert(p->dim == q->dim);
	int dim = p->dim;
	double distance = 0;
	for(int i = 0; i<dim;i++){
		distance += ((p->data[i])-(q->data[i]))*((p->data[i])-(q->data[i]));//summing up distance between each two parallel cells of data
	}

	return distance;
}



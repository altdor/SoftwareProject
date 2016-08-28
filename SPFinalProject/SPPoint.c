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
/**
 * Allocates a copy of the given point.
 *
 * Given the point source, the functions returns a
 * new pint P = (P_1,...,P_{dim-1}) such that:
 * - P_i = source_i (The ith coordinate of source and P are the same)
 * - dim(P) = dim(source) (P and source have the same dimension)
 * - index(P) = index(source) (P and source have the same index)
 *
 * @param source - The source point
 * @assert (source != NUlL)
 * @return
 * NULL in case memory allocation occurs
 * Others a copy of source is returned.
 */
SPPoint spPointCopy(SPPoint source){
	assert(source!=NULL);
	return spPointCreate(source->data, source->dim, source->index);
}
/**
 * Free all memory allocation associated with point,
 * if point is NULL nothing happens.
 */
void spPointDestroy(SPPoint point){
	if (point == NULL)
		return;
	free(point->data);
	free(point);
}
/**
 * A getter for the dimension of the point
 *
 * @param point - The source point
 * @assert point != NULL
 * @return
 * The dimension of the point
 */
int spPointGetDimension(SPPoint point){
	assert(point!=NULL);
	return point->dim;
}
/**
 * A getter for the index of the point
 *
 * @param point - The source point
 * @assert point != NULL
 * @return
 * The index of the point
 */
int spPointGetIndex(SPPoint point){
	assert(point!=NULL);
	return point->index;
}

/**
 * A getter for specific coordinate value
 *
 * @param point - The source point
 * @param axis  - The coordinate of the point which
 * 				  its value will be retreived
 * @assert point!=NULL && axis < dim(point)
 * @return
 * The value of the given coordinate (p_axis will be returned)
 */
double spPointGetAxisCoor(SPPoint point, int axis){
	assert(point!=NULL);
	assert(axis<point->dim);
	return point->data[axis];
}
double* spPointGetData(SPPoint point){
	assert(point!=NULL);
	return point->data;
}
/**
 * Calculates the L2-squared distance between p and q.
 * The L2-squared distance is defined as:
 * (p_1 - q_1)^2 + (p_2 - q_1)^2 + ... + (p_dim - q_dim)^2
 *
 * @param p - The first point
 * @param q - The second point
 * @assert p!=NULL AND q!=NULL AND dim(p) == dim(q)
 * @return
 * The L2-Squared distance between p and q
 */
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



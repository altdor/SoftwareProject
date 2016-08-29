/*
 * SPKDArray.h
 *
 *  Created on: 25 αιεμ 2016
 *      Author: yahav
 */

#ifndef SPKDARRAY_H_
#define SPKDARRAY_H_
#include "SPPoint.h"

/** Type for defining the KD-ARRAY **/
typedef struct sp_kdarray_t* SPKDArray;
typedef struct sp_pcoor_t* SPPCoor;

/**
 * Initializes the kd-array with the data given by arr
 * The complexity of this operation is O(d X nlog(n))
 */
SPKDArray spKdarrayInit(SPPoint* arr, int size);

/**
 * Returns two kd-arrays (kdLeft, kdRight) such that the
 * first [n/2]  points with respect to the coordinate coor are in kdLeft
 * and the rest of the points are in kdRight.
 */
SPKDArray* spKdarraySplit(SPKDArray kdArr, int coor);

/**
 * compares between two sppoints via axis-coordinate "axis"
 */
int compByAxis(const void* p1, const void* p2);
int spKdarrayGetSize(SPKDArray arr);
SPPoint* spKdarrayGetPointAraay(SPKDArray arr);
void spCoorSetPoint(SPPCoor spcoor,SPPoint p);
void spCoorSetAxis(SPPCoor spcoor,int axis);
void spCoorSetIndex(SPPCoor spcoor,int index);
SPPoint spCoorGetPoint(SPPCoor spcoor);
int spCoorGetAxis(SPPCoor spcoor);
int spCoorGetIndex(SPPCoor spcoor);
int main2();
#endif /* SPKDARRAY_H_ */

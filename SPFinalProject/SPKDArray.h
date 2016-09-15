/*
 * SPKDArray.h
 *
 *  Created on: 25 αιεμ 2016
 *      Author: yahav
 */

#ifndef SPKDARRAY_H_
#define SPKDARRAY_H_
#include "SPPoint.h"

/**
 * Type for defining the KD-ARRAY
*/
typedef struct sp_kdarray_t* SPKDArray;
/*
 * Type for defining a SPPoint that includes an additional field for coordinate
 */
typedef struct sp_pcoor_t* SPPCoor;

/**
 * Initializes the kd-array with the data given by arr
 * we create a dXn matrix of indices (where d = dimension and n = number of features)
 * such that the i'th row contains the indices of the points sorted according to the i'th dimension
 *
 * @return
 * NULL in case allocation failure occurred OR ARRAY is NULL OR size <=0
 * Otherwise, the new KD-ARRAY is returned
 * The complexity of this operation is O(d X nlog(n))
 */
SPKDArray spKdarrayInit(SPPoint* arr, int size);

/**
 * splits one KD-ARRAY into two KD-ARRAYs (kdLeft, kdRight) such that the
 * first [n/2]  points with respect to the coordinate "coor" are in kdLeft
 * and the rest of the points are in kdRight.
 * we create arrays indicating if a point is moved after the split to
 * the left or right sub KD-ARRAY, and with the matrix of the indices we build two
 * new matrices each regarding to a sub KD-ARRAY.
 *
 * @return
 * NULL in case allocation failure occurred OR KDARRAY is NULL OR coor <0
 * Otherwise, the two KD-ARRAYs are returned
 * The complexity of this operation is O(d X n)
 */
SPKDArray* spKdarraySplit(SPKDArray kdArr, int coor);

/**
 * compares between two SPPOINTs via axis-coordinate "axis"
 */
int compByAxis(const void* p1, const void* p2);

/**
 * a getter for the KD-ARRAY size field.
 */
int spKdarrayGetSize(SPKDArray arr);

/**
 * a getter for the KD-ARRAY array of points field.
 */
SPPoint* spKdarrayGetPointAraay(SPKDArray arr);

/**
 * a setter for the SPPCOOR point field.
 */
//void spCoorSetPoint(SPPCoor spcoor,SPPoint p);

/**
 * a setter for the SPPCOOR axis field.
 */
//void spCoorSetAxis(SPPCoor spcoor,int axis);

/**
 * a setter for the SPPCOOR index field.
 */
void spCoorSetIndex(SPPCoor spcoor,int index);

void spCoorSetVal(SPPCoor spcoor,double val);
/**
 * a getter for the SPPCOOR point field.
 */
//SPPoint spCoorGetPoint(SPPCoor spcoor);

/**
 * a getter for the SPPCOOR axis field.
 */
//int spCoorGetAxis(SPPCoor spcoor);

/**
 * a setter for the SPPCOOR index field.
 */
int spCoorGetIndex(SPPCoor spcoor);

double spCoorGetVal(SPPCoor spcoor);

void splittedDestroy(SPKDArray* splitted);

/**
 * Free all memory allocation associated with KD-ARRAY,
 * if point is NULL nothing happens.
 */
void spKDArrayDestroy(SPKDArray arr);

#endif /* SPKDARRAY_H_ */

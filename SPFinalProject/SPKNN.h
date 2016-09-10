/*
 * SPKNN.h
 *
 *  Created on: 29 баев 2016
 *      Author: yahav
 */

#ifndef SPKNN_H_
#define SPKNN_H_
#include "SPPoint.h"
#include "KDTree.h"
#include "SPListElement.h"
#include "SPBPriorityQueue.h"

/*
 * Type for defining a SPBPQ that includes an additional field for its maximum size
 */
typedef struct sp_knn_t* SPKNN;

/*
 * initializing a SPKNN queue with maximum size spKNN
 *
 *@return
 * NULL in case allocation failure occurred OR maximum size <=0
 * Otherwise, the new spknn is returned
 */
SPKNN spKinit(int spKNN);

/*
 * creating an element for the specific image
 * it is used when calling the Enqueue function in the kNearestNeighbours
 */
SPListElement imElement (SPPoint image, SPPoint curr);

/**
 * a getter for the SPKNN bpq field.
 */
SPBPQueue GetKnnBpq (SPKNN spk);

/*
 * inserting the k points from the KDTREE that are nearest to the given point into the SPBPQueue
 *
 *@return
 * void function returns anyway
 */
void kNearestNeighbors(KDTreeNode curr, SPBPQueue bpq, SPPoint point);

/**
 * Free all memory allocation associated with SPKNN,
 * if point is NULL nothing happens.
 */
void SPKNNDestroy(SPKNN knn);
#endif /* SPKNN_H_ */

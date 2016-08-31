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

typedef struct sp_knn_t* SPKNN;
SPKNN spKinit(int spKNN);
SPListElement imElement (SPPoint image, SPPoint curr);
SPBPQueue GetKnnBpq (SPKNN spk);
void kNearestNeighbors(KDTreeNode curr, SPBPQueue bpq, SPPoint point);
void SPKNNDestroy(SPKNN knn);
#endif /* SPKNN_H_ */

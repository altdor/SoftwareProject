/*
 * SPKNN.c
 *
 *  Created on: 29 ���� 2016
 *      Author: yahav
 */

#include "SPBPriorityQueue.h"
#include "SPKNN.h"
#include "KDTree.h"
#include <stdio.h>
#include <stdlib.h>
#include "SPLogger.h"
struct sp_knn_t{
	SPBPQueue bpq;
	int spKNN;
};

SPKNN spKinit(int spKNN){
	SPKNN spk = (SPKNN)malloc(sizeof(*spk));
	if(spk==NULL){
		return NULL;
	}
	spk->bpq = spBPQueueCreate(spKNN);
	if(spk->bpq==NULL){
		free(spk);
		return NULL;
	}
	spk->spKNN = spKNN;
	return spk;
}
SPBPQueue GetKnnBpq (SPKNN spk){
	if (spk==NULL)
		return NULL;
	return spk->bpq;
}

SPListElement imElement (SPPoint image, SPPoint curr){
	return spListElementCreate(spPointGetIndex(curr),spPointL2SquaredDistance(image,curr));
}

void kNearestNeighbors(KDTreeNode curr, SPBPQueue bpq, SPPoint point){
	int dis;
	bool left;
	if (curr==NULL || bpq==NULL || point==NULL)
		return;
	if(isLeaf(curr))
		spBPQueueEnqueue(bpq,imElement(point,spKDTreeGetData(curr)));
	if (spPointGetData(point)[spKDTreeGetDim(curr)]<=spKDTreeGetVal(curr)){
		kNearestNeighbors(spKDTreeGetLeft(curr), bpq, point);
		left = true;
	}
	else
		kNearestNeighbors(spKDTreeGetRight(curr), bpq, point);
	dis  = (spPointGetData(point)[spKDTreeGetDim(curr)]-spKDTreeGetVal(curr));
	dis = dis*dis;
	if (!spBPQueueIsFull(bpq) || dis<spBPQueueMaxValue(bpq)){
		if (left)
			kNearestNeighbors(spKDTreeGetRight(curr), bpq, point);
		else
			kNearestNeighbors(spKDTreeGetLeft(curr), bpq, point);
	}
}
void SPKNNDestroy(SPKNN knn){
	spBPQueueDestroy(knn->bpq);
	free(knn);
}
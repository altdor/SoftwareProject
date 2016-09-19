#include "KDTree.h"
#include "SPConfig.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SPLogger.h"
#include "SPPoint.h"
#include "SPKDArray.h"
#define LEAF 1
#define SECOND 1
#define EVEN 0
#define FIRST 0
#define INIT 0
#define ABORT -1

struct sp_KDTree_Node {
	int Dim;
	double Val;
	SPPoint Data;
	struct sp_KDTree_Node* left;
	struct sp_KDTree_Node* right;
};

KDTreeNode buildKDTree(SPKDArray array, SP_KDTREE_SPLIT_METHOD splitMethod, int incPointer){
	KDTreeNode kdtree;
	int coor=INIT;
	int spread;
	int dim;
	SPPCoor* p;
	SPPoint* arr;
	if(spKdarrayGetSize(array) <= 0 || incPointer<0){
		return NULL;
	}
	arr = spKdarrayGetPointAraay(array);
	kdtree= (KDTreeNode)malloc(sizeof(*kdtree));
	if(kdtree == NULL){
		return NULL;
	}
	if(spKdarrayGetSize(array) == LEAF){//if the tree node is a leaf
		kdtree->Data = spPointCopy(spKdarrayGetPointAraay(array)[0]);
		kdtree->left=NULL;
		kdtree->right=NULL;
		return kdtree;
	}
	else{
		SPKDArray* splitted;
		dim =spPointGetDimension(arr[0]);
		switch(splitMethod){
		case(MAX_SPREAD):
			p = (SPPCoor*)malloc(sizeof(SPPCoor)*spKdarrayGetSize(array));
			if(p==NULL){
				KDTreeDestroy(kdtree);
				free(kdtree);
			}
			for(int i = 0;i<spKdarrayGetSize(array);i++){
				p[i] = (SPPCoor)malloc(sizeof(SPPCoor));
			}
			for (int i=0; i<dim; i++){
				int tempSpread;
				for(int j=0; j<spKdarrayGetSize(array); j++){
					spCoorSetIndex(p[j],j);
					spCoorSetVal(p[j],spPointGetAxisCoor(arr[j],i));
				}
				qsort(p,spKdarrayGetSize(array),sizeof(SPPCoor),compByAxis);
				tempSpread = spCoorGetVal(p[spKdarrayGetSize(array)-1])-spCoorGetVal(p[FIRST]);
				if(tempSpread>spread){
					spread = tempSpread;
					coor = i;
				}
			}
			for(int i = 0;i<spKdarrayGetSize(array);i++){
				free(p[i]);
			}
			free(p);
			break;
		case(RANDOM):
				coor = rand()%spPointGetDimension(arr[FIRST]);
				break;
		case(INCREMENTAL):
				coor = incPointer%spPointGetDimension(arr[FIRST]);
				break;
		}
		splitted=spKdarraySplit(array,coor);
		if((spKdarrayGetSize(array))%2==EVEN){//the size of the array is even
			kdtree->Val=spPointGetData(spKdarrayGetPointAraay(splitted[SECOND])[FIRST])[coor];
		}
		else{
			kdtree->Val=spPointGetData(spKdarrayGetPointAraay(splitted[FIRST])[(spKdarrayGetSize(array))/2])[coor];
		}
		kdtree->left = buildKDTree(splitted[FIRST],splitMethod,incPointer+1);
		kdtree->right = buildKDTree(splitted[SECOND], splitMethod,incPointer+1);
		kdtree->Data = NULL;
		kdtree->Dim = coor;
		splittedDestroy(splitted);
	}
	return kdtree;
}

bool isLeaf (KDTreeNode kdtree){
	if (kdtree->left==NULL && kdtree->right==NULL)
		return true;
	return false;
}
SPPoint spKDTreeGetData(KDTreeNode tree){
	if(tree == NULL){
		return NULL;
	}
	return tree->Data;
}
int spKDTreeGetVal(KDTreeNode tree){
	if(tree == NULL){
		return ABORT;
	}
	return tree->Val;
}
int spKDTreeGetDim(KDTreeNode tree){
	if(tree == NULL){
		return ABORT;
	}
	return tree->Dim;
}
KDTreeNode spKDTreeGetLeft(KDTreeNode tree){
	if(tree == NULL){
		return NULL;
	}
	return tree->left;
}
KDTreeNode spKDTreeGetRight(KDTreeNode tree){
	if(tree == NULL){
		return NULL;
	}
	return tree->right;
}
void KDTreeDestroy(KDTreeNode tree){
	if(tree==NULL)
		return;
	KDTreeDestroy(tree->left);
	KDTreeDestroy(tree->right);
	if(tree->Data!=NULL){
		spPointDestroy(tree->Data);
	}
	free(tree);
}

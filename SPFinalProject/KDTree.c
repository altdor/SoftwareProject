#include "KDTree.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SPLogger.h"
#include "SPPoint.h"
#include "SPKDArray.c"
typedef struct sp_KDTree_Node {
	int Dim;
	int Val;
	SPPoint Data;
	struct sp_KDTree_Node* left;
	struct sp_KDTree_Node* right;
};

KDTree buildKDTree(SPKDArray array, int depth){
	if(array->size == 0 || depth<0);
		return SP_KDTREE_INVALID_ARGUMENT;
	KDTree kdtree= (KDTree)malloc(sizeof(*kdtree));
	if(kdtree == NULL){
				return SP_KDTREE_OUT_OF_MEMORY;
			}
	if(array->size == 1){
		kdtree->Data = spPointCopy(array->pointArr[0]);
	}
	else 
	{
		int spread;
		int random;
		KDTree left = (KDTree)malloc(sizeof(*left));
		KDTree right = (KDTree)malloc(sizeof(*right));
		if(left == NULL || right ==NULL){
			free(left);
			free(right);
			return SP_KDTREE_OUT_OF_MEMORY;
		}
		if(depth%2==0){
			spread = array->pointArr[-1][0]-array->pointArr[0][0];
			random = rand()%spread;
			left,right = spKdarraySplit(array, random);
		
		}
		else{
			spread = array->pointArr[-1][1]-array->pointArr[1][0];
			random = rand()%spread;

			left,right = spKdarraySplit(array,random);
		}
		kdtree->left = buildKDTree(left, depth+1)
		kdtree->left = buildKDTree(right, depth+1)
		kdtree->Data = NULL;
		kdtree->Dim = spread;
		kdtree->Val = random;
	}
	return kdtree;
}

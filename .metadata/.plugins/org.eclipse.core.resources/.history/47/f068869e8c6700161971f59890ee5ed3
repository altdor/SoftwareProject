#include "KDTree.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SPLoger.h"
#include "SPPoint.h"
#include "SPKDArray.c"
typedef struct sp_KDTree_Node {
	int Dim;
	int Val;
	SPPoint* Data;
	struct sp_KDTree_Node* left;
	struct sp_KDTree_Node* right;
};

KDTreeNode buildKDTree(SPKDArray array, int depth){
	if(array->size == 0 || depth<0;)
		return SP_KDTREE_INVALID_ARGUMENT;
	if(array->size == 1){
		SPKDTreeNode kdtree= (SPKDTreeNode)malloc(sizeof(*kdtree));
		if(kdtree == NULL){
			return SP_KDTREE_OUT_OF_MEMORY;
		}
		kdtree->Data = array->arr[0];
		kdtree->Dim = array->dim;
		kdtree->Dim = kdtree->Data->data[0];
	}
	else 
	{
		SPKDTreeNode kdtree= (SPKDTreeNode)malloc(sizeof(*kdtree));
		if(kdtree == NULL){
			return SP_KDTREE_OUT_OF_MEMORY;
		}
		sp_KDTree_Node left = (SPKDTreeNode)malloc(sizeof(*left));
		sp_KDTree_Node right = (SPKDTreeNode)malloc(sizeof(*right));
		if(left == NULL || right ==NULL){
			free(left);
			free(right);
			return SP_KDTREE_OUT_OF_MEMORY;
		}
		if(depth%2==0){
			SPPoint* l = medianX(array);
			left,right = SplitX(array, l->data[0]);
			kdtree->Dim =l->data[0];
		
		}
		else{
			SPPoint* l = medianY(array);
			left,right = SplitY(array, l->data[1]);
			kdtree->Dim =l->data[1];
		}
		kdtree->left = buildKDTree(left, depth+1)
		kdtree->left = buildKDTree(right, depth+1)
		kdtree->data = l;
		kdtree->Dim = array->dim;
	}
	
}
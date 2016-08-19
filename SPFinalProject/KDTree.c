#include "KDTree.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
	if(array.size == 1){
		SPKDTreeNode kdtree= (SPKDTreeNode)malloc(sizeof(*kdtree));
	}
	else if(depth%2==0){
		
	}
}
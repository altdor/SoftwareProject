/*#include "KDTree.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SPLogger.h"
#include "SPPoint.h"
#include "SPKDArray.c"
struct sp_KDTree_Node {
	int Dim;
	int Val;
	SPPoint Data;
	struct sp_KDTree_Node* left;
	struct sp_KDTree_Node* right;
};

KDTree buildKDTree(SPKDArray array, int depth){
	int spread;
	int random;
	KDTree kdtree;

	if(array->size == 0 || depth<0){
		return NULL;
	}
	kdtree= (KDTree)malloc(sizeof(*kdtree));
	if(kdtree == NULL){
				return NULL;
			}
	if(array->size == 1){
		kdtree->Data = spPointCopy(array->pointArr[0]);
	}
	else 
	{
		KDTree left = (KDTree)malloc(sizeof(*left));
		KDTree right = (KDTree)malloc(sizeof(*right));
		if(left == NULL || right ==NULL){
			free(left);
			free(right);
			return NULL;
		}
		if(depth%2==0){
			spread = (int)((array->pointArr[-1][0])-(array->pointArr[0][0]));
			random = rand()%spread;
			left,right = spKdarraySplit(array, random);
		
		}
		else{
			spread = (int)((array->pointArr[-1][1])-(array->pointArr[1][0]));
			random = rand()%spread;

			left,right = spKdarraySplit(array,random);
		}
		kdtree->left = buildKDTree(left, depth+1);
		kdtree->left = buildKDTree(right, depth+1);
		kdtree->Data = NULL;
		kdtree->Dim = spread;
		kdtree->Val = random;
	}
	return kdtree;
}
*/

#include "KDTree.h"
#include "SPConfig.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SPLogger.h"
#include "SPPoint.h"
#include "SPKDArray.h"
struct sp_KDTree_Node {
	int Dim;
	int Val;
	SPPoint Data;
	struct sp_KDTree_Node* left;
	struct sp_KDTree_Node* right;
};

KDTreeNode buildKDTree(SPKDArray array, SP_KDTREE_SPLIT_METHOD splitMethod, int incPointer){
	KDTreeNode kdtree;
	int coor=0;
	int spread;
	int dim;
	SPPCoor* p;
	SPPoint* arr = spKdarrayGetPointAraay(array);;
	if(spKdarrayGetSize(array) <= 0 || incPointer<0){
		return NULL;
	}
	kdtree= (KDTreeNode)malloc(sizeof(*kdtree));
	if(kdtree == NULL){
		return NULL;
	}
	if(spKdarrayGetSize(array) == 1){
		//kdtree->Data = spPointCopy(spKdarrayGetPointAraay(array)[0]);
		kdtree->Data = spKdarrayGetPointAraay(array)[0];
		return kdtree;
	}
	else{

		SPKDArray* splitted = (SPKDArray*)malloc(2*sizeof(*splitted));
		if(splitted == NULL){
			KDTreeDestroy(kdtree);
			free(kdtree);
			free(splitted);
			return NULL;
		}
		dim =spPointGetDimension(arr[0]);
		switch(splitMethod){
		case(MAX_SPREAD):
				p = (SPPCoor*)malloc(sizeof(SPPCoor)*spKdarrayGetSize(array));
				if(p==NULL){
					KDTreeDestroy(kdtree);
					free(kdtree);
				}

				for (int i=0; i<dim; i++){
					int tempSpread;
					for(int j=0; j<spKdarrayGetSize(array); j++){
						//spCoorSetAxis(p[j],i);
						//spCoorSetPoint(p[j],spPointCopy(arr[j]));
						spCoorSetIndex(p[j],j);
						spCoorSetVal(p[j],spPointGetAxisCoor(arr[j],i));
					}
					qsort(p,spKdarrayGetSize(array),sizeof(SPPCoor),compByAxis);
					//tempSpread = spPointGetData(spCoorGetPoint(p[spKdarrayGetSize(array)-1]))[i]-spPointGetData(spCoorGetPoint(p[0]))[i];
					tempSpread = spCoorGetVal(p[spKdarrayGetSize(array)-1])-spCoorGetVal(p[0]);
					if(tempSpread>spread){
						spread = tempSpread;
						coor = i;
					}
				}
				free(p);
				break;
		case(RANDOM):
				fflush(NULL);
				coor = rand()%spPointGetDimension(arr[0]);
				break;
		case(INCREMENTAL):
				coor = incPointer%spPointGetDimension(arr[0]);
				break;
		}
		splitted=spKdarraySplit(array,coor);
		if(dim%2==0){
			kdtree->Val=spPointGetData(spKdarrayGetPointAraay(splitted[1])[0])[coor];
		}
		else{
			kdtree->Val=spPointGetData(spKdarrayGetPointAraay(splitted[0])[(dim)/2-1])[coor];
		}
		kdtree->left = buildKDTree(splitted[0],splitMethod,incPointer+1);
		kdtree->right = buildKDTree(splitted[1], splitMethod,incPointer+1);
		kdtree->Data = NULL;
		kdtree->Dim = coor;
		free(splitted[0]);
		free(splitted[1]);
		free(splitted);
	}
	return kdtree;
}

bool isLeaf (KDTreeNode kdtree){
	return (kdtree->left==NULL && kdtree->right==NULL);
}
SPPoint spKDTreeGetData(KDTreeNode tree){
	if(tree == NULL){
		return NULL;
	}
	return tree->Data;
}
int spKDTreeGetVal(KDTreeNode tree){
	if(tree == NULL){
		return -1;
	}
	return tree->Val;
}
int spKDTreeGetDim(KDTreeNode tree){
	if(tree == NULL){
		return -1;
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
	if(tree->Data!=NULL)
		spPointDestroy(tree->Data);
	free(tree);
}

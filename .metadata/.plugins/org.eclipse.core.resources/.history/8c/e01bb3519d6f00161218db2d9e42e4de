#ifndef KDTREE_H_
#define KDTREE_H_
#include "SPKDArray.h"
#include "SPConfig.h"
typedef struct sp_KDTree_Node* KDTreeNode;

typedef enum sp_kd_tree_msg_t {
	SP_KDTREE_OUT_OF_MEMORY,
	SP_KDTREE_FULL,
	SP_KDTREE_EMPTY,
	SP_KDTREE_INVALID_ARGUMENT,
	SP_KDTREE_SUCCESS
} SP_KDTREE_MSG;


KDTreeNode buildKDTree(SPKDArray array, SP_KDTREE_SPLIT_METHOD splitMethod, int incPointer);
bool isLeaf (KDTreeNode kdtree);
SPPoint spKDTreeGetData(KDTreeNode tree);
int spKDTreeGetVal(KDTreeNode tree);
int spKDTreeGetDim(KDTreeNode tree);
KDTreeNode spKDTreeGetLeft(KDTreeNode tree);
KDTreeNode spKDTreeGetRight(KDTreeNode tree);
#endif

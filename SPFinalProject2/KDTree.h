/*
 * KDTree.h
 *
 *  Created on: 28 баев 2016
 *      Author: DorAlt
 */



#define KDTREE_H_
#include "SPKDArray.h"
typedef struct sp_KDTree_Node* KDTree;

typedef enum sp_kd_tree_msg_t {
	SP_KDTREE_OUT_OF_MEMORY,
	SP_KDTREE_FULL,
	SP_KDTREE_EMPTY,
	SP_KDTREE_INVALID_ARGUMENT,
	SP_KDTREE_SUCCESS
} SP_KDTREE_MSG;


KDTree buildKDTree(SPKDArray array, int depth);

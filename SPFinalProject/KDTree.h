#ifndef KDTREE_H_
#define KDTREE_H_
#include "SPKDArray.h"
#include "SPConfig.h"

/**
 * Type for defining a KD-TREE.
 */
typedef struct sp_KDTree_Node* KDTreeNode;

/**
 * defining the messages for the KD-TREE.
 */
typedef enum sp_kd_tree_msg_t {
	SP_KDTREE_OUT_OF_MEMORY,
	SP_KDTREE_FULL,
	SP_KDTREE_EMPTY,
	SP_KDTREE_INVALID_ARGUMENT,
	SP_KDTREE_SUCCESS
} SP_KDTREE_MSG;

/**
 * initializing a KD-TREE given a KD-ARRAY and a certain split method.
 * for MAX_SPREAD we first calculate the difference between the max and min of the i'th
 * 				  coordinate of all points, and then split the KD-ARRAY according to the dimension
 *                with the highest difference.
 * for RANDOM we choose a random dimension
 * for INCREMENTAL we increment the dimension by 1 in each successive level
 *
 * @return
 * NULL in case allocation failure occurred OR KDARRAY is NULL OR pointer <0
 * Otherwise, the new KD-TREE is returned
 */
KDTreeNode buildKDTree(SPKDArray array, SP_KDTREE_SPLIT_METHOD splitMethod, int incPointer);

/**
 * checks if a KD-TREE node has nothing to its left or right, meaning it's a leaf.
 */
bool isLeaf (KDTreeNode kdtree);

/**
 * a getter for the KD-TREE data field.
 */
SPPoint spKDTreeGetData(KDTreeNode tree);

/**
 * a getter for the KD-TREE value field.
 */
int spKDTreeGetVal(KDTreeNode tree);

/**
 * a getter for the KD-TREE dimension field.
 */
int spKDTreeGetDim(KDTreeNode tree);

/**
 * a getter for the KD-TREE left field.
 */
KDTreeNode spKDTreeGetLeft(KDTreeNode tree);

/**
 * a getter for the KD-TREE right field.
 */
KDTreeNode spKDTreeGetRight(KDTreeNode tree);

/**
 * Free all memory allocation associated with KD-TREE,
 * if point is NULL nothing happens.
 */
void KDTreeDestroy(KDTreeNode tree);

#endif

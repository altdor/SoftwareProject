## Type for defining the point

typedef struct sp_KDTree_Node* SPKDTreeNode;

typedef enum sp_kd_tree_msg_t {
	SP_KDTREE_OUT_OF_MEMORY,
	SP_KDTREE_FULL,
	SP_KDTREE_EMPTY,
	SP_KDTREE_INVALID_ARGUMENT,
	SP_KDTREE_SUCCESS
} SP_KDTREE_MSG;

##build a tree
KDTreeNode buildKDTree(SPKDArray array, int depth)
/*
 * MainAux.h
 *
 *  Created on: 28 баев 2016
 *      Author: DorAlt
 */
#include "SPPoint.h"
#include "SPConfig.h"
#include "KDTree.h"
#include "SPBPriorityQueue.h"

/**
 * prints the correct ERROR regarding the level
 */
void ErrorLogger(int level,const char* msg, const char* file,const char* function, const int line);

/**
 * checks if the file name is valid
 *
 * @return
 * false in case it isn't
 * Otherwise returns true
 */
bool checkFileName(const char* filename);

/**
 * extracts the feature to a file
 *
 * @return
 * false in case the file doesn't open
 * Otherwise returns true
 */
bool extractToFile(char* imagePathnosuf,SPPoint* features, int numOfFeatures,int level);

/**
 * extracts the feature that are already located inside a file
 *
 * @return
 * NULL in case the file doesn't open
 * Otherwise returns an array containing the features
 */
SPPoint* extractFromFiles(char* imagePathnosuf,int level);

/**
 * executes the functions that need to be done in the not minimal gui mode
 */
void notMinimalGui(int* arr, char* imgpath, SPConfig config,int size);

/**
 * finds how many times each image was selected as one of the K nearest.
 *
 * @return
 * NULL in case of an allocation problem or if the tree OR the Points' array are NULL OR size<=0
 * Otherwise returns an array where each cell corresponds to an image (by the index) and contains
 * the number of times that image was selected as one of the K nearest
 */
int* kNearest(KDTreeNode tree, SPPoint* features, SPConfig config, int size);

/**
 * finds the image that is the nearest to the original one.
 *
 * @return
 * the index of the nearest image
 */
int maxIndex(int* count, int size);

/**
 * receives a 2 dimensional array and inserts it into a 1 dimensional array
 *
 * @return
 * NULL in case of an allocation problem
 * Otherwise returns an array that contains all the points that where in the 2D array
 */
SPPoint* make2DTo1D(SPPoint** array, int numOfImages, int numOfFeatures,SPConfig config);

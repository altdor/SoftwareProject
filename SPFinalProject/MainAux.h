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
void ErrorLogger(int level,const char* msg, const char* file,const char* function, const int line);
bool checkFileName(const char* filename);
bool extractToFile(char* imagePathnosuf,SPPoint* features, int numOfFeatures,int level);
SPPoint* extractFromFiles(char* imagePathnosuf,int level);
void notMinimalGui(int* arr, char* imgpath, SPConfig config,int size);
int* kNearest(KDTreeNode tree, SPPoint* features, SPConfig config, int size);
int maxIndex(int* count, int size);
SPPoint* make2DTo1D(SPPoint** array, int numOfImages, int numOfFeatures,SPConfig config);

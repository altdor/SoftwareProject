/*
 * MainAux.h
 *
 *  Created on: 28 ���� 2016
 *      Author: DorAlt
 */
#include "SPPoint.h"
#include "SPConfig.h"
#include "KDTree.h"
#include "SPBPriorityQueue.h"
bool checkFileName(const char* filename);
bool extractToFile(char* imagePathnosuf,SPPoint* features, int numOfFeatures,int level);
SPPoint* extractFromFiles(char* imagePathnosuf,int level);
void ErrorLogger(int level,const char* msg, const char* file,const char* function, const int line);
void notMinimalGui(int* arr, char* imgpath, SPConfig config,int size);
int* kNearest(KDTreeNode tree, SPPoint* features, SPConfig config, int size);
int maxIndex(int* count, int size);

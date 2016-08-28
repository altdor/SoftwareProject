/*
 * MainAux.h
 *
 *  Created on: 28 баев 2016
 *      Author: DorAlt
 */
#include "SPPoint.h"
bool checkFileName(const char* filename);
bool extractToFile(char* imagePathnosuf,SPPoint* features, int numOfFeatures,int level);
SPPoint* extractFromFiles(char* imagePathnosuf,int level);
void ErrorLogger(int level, const char* msg, const char* file,
		const char* function, const int line);

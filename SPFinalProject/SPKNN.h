/*
 * SPKNN.h
 *
 *  Created on: 29 ���� 2016
 *      Author: yahav
 */

#ifndef SPKNN_H_
#define SPKNN_H_
#include "SPPoint.h"
typedef struct sp_knn_t* SPKNN;
SPKNN spKinit(int spKNN);
SPListElement imElement (SPPoint image, SPPoint curr);
#endif /* SPKNN_H_ */

#include "../SPPoint.h"
#include "unit_test_util.h"
#include <stdbool.h>

bool testPointEdgeCases()
{
	double data1[2] = { 1.5, 2.0 };

	SPPoint p1, p2, p3, p4, p5;

	p1 = spPointCreate((double *)data1, 0, 2);
	if(p1 != NULL)
		printf("spPointCreate did not return NULL with dim = 0...\n");
	else
		printf("spPointCreate for p1 OK\n");

	p2 = spPointCreate((double *)data1, 1, -1);
	if(p2 != NULL)
		printf("spPointCreate did not return NULL with index = -1...\n");
	else
			printf("spPointCreate for p2 OK\n");

	p3 = spPointCreate(NULL, 1, 1);
	if(p3 != NULL)
		printf("spPointCreate did not return NULL with data = NULL...\n");
	else
			printf("spPointCreate for p3 OK\n");

	p4 = spPointCreate((double *)data1, 2, 1);
	printf("p4 index is: %d\n", spPointGetIndex(p4));

	p5 = spPointCopy(p4);
	printf("p5 index is: %d\n", spPointGetIndex(p5));

	spPointDestroy(p1);
	printf("After destroy of p1\n");
	spPointDestroy(p2);
	spPointDestroy(p3);
	spPointDestroy(p4);

	printf("p5 index after p4 destroy is: %d\n", spPointGetIndex(p5));

	spPointDestroy(p5);
	return true;
}
/*

int main()
{
	testPointEdgeCases();
	return 0;
}
*/

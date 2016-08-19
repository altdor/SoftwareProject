
#include "unit_test_util.h"
#include "../SPListElement.h"
#include "../SPList.h"
#include "../SPBPriorityQueue.h"
#include <stdarg.h>
#define MAXSIZE 100

struct sp_bp_queue_t
{
	int maxSize;
	SPList list;
};

static bool testQueue()
{
	SPBPQueue q1 = spBPQueueCreate(4), q2 = spBPQueueCreate(3), q3 = spBPQueueCreate(0), q4, q5, q6 = NULL, q7;
	SPListElement e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16;
	SP_BPQUEUE_MSG q_msg;
	SP_ELEMENT_MSG e_msg;

	//
	e1 = spListElementCreate(2, 22.22);
	e2 = spListElementCreate(1, 11.11);
	e3 = spListElementCreate(3, 33.33);
	e4 = spListElementCreate(4, 44.44);
	e5 = spListElementCreate(5, 55.55);
	e6 = NULL;
	e7 = spListElementCreate(7, 77.77);
	e8 = spListElementCreate(8, 88.88);
	e9 = spListElementCreate(9, 99.00);
	e12 = spListElementCreate(12, 12.12);
	e13 = spListElementCreate(13, 13.13);

	e10 = spListElementCopy(e9);
	e11 = spListElementCopy(e6);

	printf("Compare: %d\n", spListElementCompare(e9, e10));	
	printf("%f\n", spListElementGetValue(e2));
	printf("%f\n", spListElementGetValue(e6));


	e_msg = spListElementSetValue(e9, 99.99);
	printf("message: %d\n", e_msg);
	printf("e9 value: %f\n", spListElementGetValue(e9));

	e_msg = spListElementSetValue(e6, 11.22);
	printf("message: %d\n", e_msg);

	e_msg = spListElementSetValue(e9, -33.33);
	printf("message: %d\n", e_msg);
	printf("e9 value: %f\n", spListElementGetValue(e9));

	e_msg = spListElementSetIndex(e9, 99);
	printf("message: %d\n", e_msg);
	printf("message: %d\n", spListElementGetIndex(e9));

	e_msg = spListElementSetIndex(e9, -33.33);
	printf("message: %d\n", e_msg);
	printf("message: %d\n", spListElementGetIndex(e9));

	printf("%d\n", spListElementCompare(e9, e10));

	/////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////  queue  ///////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////


	if(q3 == NULL)
		printf("q3 OK\n");
	else
		printf("q3 is NOT NULL\n");


	q4 = spBPQueueCopy(NULL);
	if(q4 == NULL)
		printf("q4 OK\n");
	else
		printf("q4 is NOT NULL\n");

	
	printf("%d, %d, %d, %d\n", spBPQueueGetMaxSize(q1), spBPQueueGetMaxSize(q2), spBPQueueGetMaxSize(q3), spBPQueueGetMaxSize(q4));

	spBPQueueEnqueue(q1, e1);
	spBPQueueEnqueue(q1, e2);
	q5 = spBPQueueCopy(q1);
	spBPQueueEnqueue(q1, e3);

	printf("Size of q1: %d\n", spBPQueueSize(q1));
	printf("Size of q5: %d\n", spBPQueueSize(q5));
	printf("Size of q6: %d\n", spBPQueueSize(q6));

	printf("Min value of q1: %f\n", spBPQueueMinValue(q1));
	printf("Max value of q1: %f\n", spBPQueueMaxValue(q1));

	q_msg = spBPQueueEnqueue(q1, e5);
	printf("message: %d\n", q_msg);
	q_msg = spBPQueueEnqueue(q1, e4);
	printf("message: %d\n", q_msg);
	q_msg = spBPQueueEnqueue(q1, e5);
	printf("message: %d\n", q_msg);

	q_msg = spBPQueueEnqueue(q2, e6);
	printf("message: %d\n", q_msg);
	q_msg = spBPQueueEnqueue(q3, e5);
	printf("message: %d\n", q_msg);
	q_msg = spBPQueueEnqueue(q3, e6);
	printf("message: %d\n", q_msg);


	q4 = spBPQueueCopy(q1);
	printf("Min value of q1: %f\n", spBPQueueMinValue(q1));
	printf("Max value of q1: %f\n", spBPQueueMaxValue(q1));

	printf("IsEmpty for q5: %d\n", spBPQueueIsEmpty(q5));
	printf("IsFull for q5: %d\n", spBPQueueIsFull(q5));
	printf("IsEmpty for q1: %d\n", spBPQueueIsEmpty(q1));
	printf("IsFull for q1: %d\n", spBPQueueIsFull(q1));

	q7 = spBPQueueCreate(4);
	e16 = spBPQueuePeek(q7);
	
	spBPQueueEnqueue(q7, e12);
	spBPQueueEnqueue(q7, e13);
	e14 = spBPQueuePeek(q7);
	printf("e14 value: %f\n", spListElementGetValue(e14));
	e15 = spBPQueuePeekLast(q7);
	printf("e15 value: %f\n", spListElementGetValue(e15));
	spListElementSetValue(e15, 15.15);
	printf("e15 value: %f\n", spListElementGetValue(e15));
	printf("Min value of q7: %f\n", spBPQueueMinValue(q7));
	printf("Max value of q7: %f\n", spBPQueueMaxValue(q7));

	spBPQueueDequeue(q7);
	printf("Min value of q7: %f\n", spBPQueueMinValue(q7));
	printf("Max value of q7: %f\n", spBPQueueMaxValue(q7));

	spBPQueueDequeue(q7);
	printf("Min value of q7: %f\n", spBPQueueMinValue(q7));
	printf("Max value of q7: %f\n", spBPQueueMaxValue(q7));

	q_msg = spBPQueueDequeue(q7);
	printf("message: %d\n", q_msg);


	// free memory
	spBPQueueDestroy(q1);
	spBPQueueDestroy(q2);
	spBPQueueDestroy(q3);
	spBPQueueDestroy(q4);
	spBPQueueDestroy(q5);
	spBPQueueDestroy(q6);
	spBPQueueDestroy(q7);
	
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e4);
	spListElementDestroy(e5);
	spListElementDestroy(e6);
	spListElementDestroy(e7);
	spListElementDestroy(e8);
	spListElementDestroy(e9);
	spListElementDestroy(e10);
	spListElementDestroy(e11);
	spListElementDestroy(e12);
	spListElementDestroy(e13);
	spListElementDestroy(e14);
	spListElementDestroy(e15);
	spListElementDestroy(e16);

	printf("testQueue ended successfully!!!");
	return true;
}/*

int main()
{
	testQueue();
	return 0;
}*/

/*
 * SPBPriorityQueue.c
 *
 *  Created on: 24 במאי 2016
 *      Author: DorAlt
 */
#include "SPList.h"
#include "SPBPriorityQueue.h"
#include "SPListElement.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct sp_bp_queue_t{
	SPList list;
	int capacity;
};
SPBPQueue spBPQueueCreate(int maxSize){
	if(maxSize==0)
		return NULL;
	SPBPQueue queue = (SPBPQueue)malloc(sizeof(*queue));
	queue->capacity = maxSize;
	queue -> list = spListCreate();
	return queue;
}
SPBPQueue spBPQueueCopy(SPBPQueue source){
	if (source==NULL){
		return NULL;
	}
	SPBPQueue queue = (SPBPQueue)malloc(sizeof(*queue));
	if(queue == NULL){
		return NULL;
	}
	queue->capacity = source->capacity;
	queue -> list = spListCopy(source->list);
	return queue;
}
void spBPQueueDestroy(SPBPQueue source){
	if(source != NULL){
		spListDestroy(source->list);
	}
	free(source);
}
void spBPQueueClear(SPBPQueue source){
	if(source != NULL){
		spListClear(source->list);
	}
}
int spBPQueueSize(SPBPQueue source){
	if(source == NULL)
		return -1;
	return spListGetSize(source->list);
}
int spBPQueueGetMaxSize(SPBPQueue source){
	if(source == NULL)
			return -1;
		return source->capacity;
}

SP_BPQUEUE_MSG spBPQueueEnqueue(SPBPQueue source, SPListElement element){
	SP_LIST_MSG msg;
	SPListElement insertionPlace;
	SPListElement deletePlace;
	if(source == NULL || element == NULL || source->list == NULL){
		return SP_BPQUEUE_INVALID_ARGUMENT;
	}
	if(source->capacity > 0){
		if(spBPQueueIsEmpty(source)){//if queue is empty insert
			msg = spListInsertFirst(source->list, element);
		}
		else{
			if(spBPQueueIsFull(source)){//if queue is full
				insertionPlace = spBPQueuePeekLast(source);
				if(spListElementGetValue(insertionPlace) < spListElementGetValue(element)){//if the element is value is greater or equal to max
					spListElementDestroy(insertionPlace);
					return SP_BPQUEUE_SUCCESS;
				}
				else if(spListElementGetValue(insertionPlace) == spListElementGetValue(element) &&  spListElementGetIndex(insertionPlace) <= spListElementGetIndex(element)){//if element is equal to greater and index is bigger
					spListElementDestroy(insertionPlace);
					return SP_BPQUEUE_SUCCESS;
				}
				else{//the greatest elements need to be deleted of the queue
					deletePlace = spListGetFirst(source->list);
					for(int i = 0; i<spListGetSize(source->list)-1;i++){
						deletePlace = spListGetNext(source->list);
					}

					msg = spListRemoveCurrent(source->list);
					deletePlace = spListElementCopy(deletePlace);
					spListElementDestroy(deletePlace);
					spListElementDestroy(insertionPlace);
				}

			}
			if(spBPQueueMinValue(source)>spListElementGetValue(element)){//if needed to be entered first
				msg = spListInsertFirst(source->list,element);
			}
			else if(spBPQueueMaxValue(source)<spListElementGetValue(element)){//if needed to be entered last
				msg = spListInsertLast(source->list,element);
			}
			else{
				insertionPlace = spListGetFirst(source->list);
				while(spListElementGetValue(insertionPlace)<spListElementGetValue(element)){//find the place to enter
					insertionPlace=spListGetNext(source->list);
				}
				if(insertionPlace != NULL){
					if(spListElementGetValue(insertionPlace)==spListElementGetValue(element) && spListElementGetIndex(insertionPlace)<spListElementGetIndex(element)){
						msg = spListInsertAfterCurrent(source->list, element);
					}
					else{
						msg = spListInsertBeforeCurrent(source->list,element);
					}
				}
			}
		}

	}
	if (msg==SP_LIST_NULL_ARGUMENT || msg==SP_LIST_INVALID_CURRENT)
		return SP_BPQUEUE_INVALID_ARGUMENT;
	else if (msg==SP_LIST_OUT_OF_MEMORY)
		return SP_BPQUEUE_OUT_OF_MEMORY;
	return SP_BPQUEUE_SUCCESS;
}

SP_BPQUEUE_MSG spBPQueueDequeue(SPBPQueue source){
	SP_LIST_MSG msg;
	SPListElement deletePlace;
	if(source == NULL){
		return SP_BPQUEUE_INVALID_ARGUMENT;
	}
	if(spBPQueueIsEmpty(source)){
		return SP_BPQUEUE_EMPTY;
	}
	deletePlace = spListGetFirst(source->list);
	msg = spListRemoveCurrent(source->list);
	if(msg == SP_LIST_SUCCESS){
		return SP_BPQUEUE_SUCCESS;
	}
	deletePlace = spListElementCopy(deletePlace);
	spListElementDestroy(deletePlace);
	return msg;
}

SPListElement spBPQueuePeek(SPBPQueue source){
	SPListElement element = spListGetFirst(source->list);
	return spListElementCopy(element);
}

SPListElement spBPQueuePeekLast(SPBPQueue source){
	SPListElement element = spListGetFirst(source->list);
	SPListElement temp=spListGetNext(source->list);
	while(temp!=NULL){
		element = temp;
		temp=spListGetNext(source->list);
	}

	return spListElementCopy(element);
}

double spBPQueueMinValue(SPBPQueue source){
	SPListElement element = spListGetFirst(source->list);
	return spListElementGetValue(element);
}

double spBPQueueMaxValue(SPBPQueue source){
	SPListElement element = spListGetFirst(source->list);
	SPListElement temp=spListGetNext(source->list);
	while(temp!=NULL){
		element = temp;
		temp=spListGetNext(source->list);
	}

	return spListElementGetValue(element);
}

bool spBPQueueIsEmpty(SPBPQueue source){
	assert(source != NULL);
	if(spListGetSize(source->list) == 0 &&  source->capacity != 0){
		return true;
	}
	return false;
}

bool spBPQueueIsFull(SPBPQueue source){
	assert(source != NULL);
	if(spListGetSize(source->list) == source->capacity){
		return true;
	}
	return false;
}



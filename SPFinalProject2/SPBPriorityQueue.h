/*
 * SPBPriorityQueue.h
 *
 *  Created on: 28 баев 2016
 *      Author: DorAlt
 */
#ifndef SPBPRIORITYQUEUE_H_
#define SPBPRIORITYQUEUE_H_
#include "SPListElement.h"
#include <stdbool.h>
/**
 * SP Bounded Priority Queue summary
 * SP Bounded Priority Queue is a data structure similar
 * to a regular minimum priority queue, though it has an upper
 * bound on the capacity of the elements in it.
 *
 * Each element in the BPQ has a value of type double, and
 * an index of type int. If an element is inserted while
 * the BPQ is in full capacity, then the element with the
 * highest value will be ejected from the queue.
 *
 * The following functions are supported:
 * spBPQueueCreate     - creates an empty queue with a given maximum capacity
 * spBPQueueCopy       - creates a copy of a given queue
 * spBPQueueDestroy    - frees all memory allocation associated with the queue
 * spBPQueueClear      - removes all the elements in the queue
 * spBPQueueGetSize    - returns the number of elements in the queue
 * spBPQueueGetMaxSize - returns the maximum capacity of the queue
 * spBPQueueEnqueue    - Inserts a NEW COPY (must be allocated) element to the queue
 * spBPQueueDequeue    - removes the element with the lowest value
 * spBPQueuePeek       - returns a NEW COPY of the element with the lowest value
 * spBPQueuePeekLast   Ц returns a NEW COPY of the element with the highest value
 * spBPQueueMinValue   - returns  the minimum value in the queue
 * spBPQueueMaxValue   - returns the maximum value in the queue
 * spBPQueueIsEmpty    Ц returns true if the queue is empty
 * spBPQueueIsFull     - returns true if the queue is full
 */

/** type used to define Bounded priority queue **/
typedef struct sp_bp_queue_t *SPBPQueue;

/** type for error reporting **/
typedef enum sp_bp_queue_msg_t {
	SP_BPQUEUE_OUT_OF_MEMORY,
	SP_BPQUEUE_FULL,
	SP_BPQUEUE_EMPTY,
	SP_BPQUEUE_INVALID_ARGUMENT,
	SP_BPQUEUE_SUCCESS
} SP_BPQUEUE_MSG;

/**
 * Allocates a new BPQueue.
 *
 * This function creates a new empty BPQueue.
 * @return
 * 	NULL - If allocations failed or maxsize<=0.
 * 	A new BPQueue in case of success.
 */
SPBPQueue spBPQueueCreate(int maxSize);

/**
 * Creates a copy of target BPQueue.
 *
 * The new copy will contain all the elements from the source BPQueue in the same
 * order.
 *
 * @param source The target BPQueue to copy
 * @return
 * NULL if a NULL was sent or a memory allocation failed.
 * A BPQueue containing the same elements with same order as source otherwise.
 */
SPBPQueue spBPQueueCopy(SPBPQueue source);

/**
 * Free all memory allocation associated with the queue,
 * if source is NULL nothing happens.
 *
 * @param source Target queue to free all memory from
 */
void spBPQueueDestroy(SPBPQueue source);

/**
 * Removes all elements from target queue.
 *
 * The elements are deallocated using the Dequeue function
 * @param source Target queue to remove all element from
 */
void spBPQueueClear(SPBPQueue source);

/**
 * Returns the number of elements in the queue.
 *
 * @param source The target queue which size is requested.
 * @return
 * -1 if a NULL pointer was sent.
 * Otherwise the number of elements in the list.
 */
int spBPQueueSize(SPBPQueue source);

/**
 * returns the maximum capacity of the queue.
 *
 * @param source The target queue of which the maximum capacity is requested.
 * @return
 * -1 if a NULL pointer was sent.
 * Otherwise the maximum capacity of the queue.
 */
int spBPQueueGetMaxSize(SPBPQueue source);

/**
 * Insert a new copy element to the queue.
 * if the queue is in full capacity, the new element would be inserted
 * if the value of the element isn`t bigger than the other element`s
 * values. If the element`s value is equal to the highest value in the
 * queue, the element would be inserted only if it`s index is
 * smaller than the index of the element with the same high value
 *
 * @param source The queue for which to add an element
 * @param element The element to insert. A copy of the element
 * will be inserted
 *
 * @return
 * SP_BPQUEUE_INVALID_ARGUMENT if a NULL was sent as queue or an element
 * SP_BPQUEUE_OUT_OF_MEMORY if an allocation failed
 * SP_BPQUEUE_SUCCESS the element has been inserted successfully
 */
SP_BPQUEUE_MSG spBPQueueEnqueue(SPBPQueue source, SPListElement element);

/**
 * Removes the element with the lowest value.
 *
 * @param source The queue from which the current element will be removed
 *
 * @return
 * SP_BPQUEUE_EMPTY if there are no elements to be removed in the queue
 * SP_BPQUEUE_INVALID_ARGUMENT if queue is NULL
 * SP_BPQUEUE_SUCCESS if an element was removed successfully
 */
SP_BPQUEUE_MSG spBPQueueDequeue(SPBPQueue source);

/**
 * Returns a copy of the element with the lowest value.
 *
 * @param source The queue from which the element is copied
 *
 * @return
 * NULL if the queue is empty
 * The element with the lowest value otherwise
 */
SPListElement spBPQueuePeek(SPBPQueue source);

/**
 * Returns a copy of the element with the highest value.
 *
 * @param source The queue from which the element is copied
 *
 * @return
 * NULL if the queue is empty
 * The element with the highest value otherwise
 */
SPListElement spBPQueuePeekLast(SPBPQueue source);

/**
 * Returns the lowest value in the queue.
 *
 * @param source The queue from which the value is taken
 *
 * @return
 * -1 if the queue is empty
 * The lowest value in the queue otherwise
 */
double spBPQueueMinValue(SPBPQueue source);

/**
 * Returns the highest value in the queue.
 *
 * @param source The queue from which the value is taken
 *
 * @return
 * -1 if the queue is empty
 * The highest value in the queue otherwise
 */
double spBPQueueMaxValue(SPBPQueue source);

/**
 * checks if the queue is empty.
 *
 * @param source The queue that is checked
 * @assert (source != NULL)
 * @return
 * TRUE if the queue is empty, FALSE if it isn`t.
 */
bool spBPQueueIsEmpty(SPBPQueue source);

/**
 * checks if the queue reached it`s full capacity.
 *
 * @param source The queue that is checked
 * @assert (source != NULL)
 * @return
 * TRUE if the queue reached it`s full capacity, FALSE if it didn`t.
 */
bool spBPQueueIsFull(SPBPQueue source);

#endif





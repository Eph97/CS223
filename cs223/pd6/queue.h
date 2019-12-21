#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stdbool.h>

struct elt;
struct queue;

//With help from Aspen's notes


struct queue *queueCreate(void);


void enq(struct queue *q, int value);

int queueEmpty(const struct queue *q);

int deq(struct queue *q);

void queuePrint(struct queue *q);

void queueDestroy(struct queue *q);

#endif


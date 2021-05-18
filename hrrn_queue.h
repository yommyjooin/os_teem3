#ifndef __HRRN_QUEUE_H
#define __HRRN_QUEUE_H

#include <stdio.h>

typedef struct Process {

	int p_num;
	int at, bt, ct, wt, tt;
	int completed;
	float ntt;

} Process;

typedef struct Node {

	struct Node *next;
	struct Process *p;

} Node;

 
typedef struct Queue {

	int sum_bt;
	int size;
	int max_size;
	int cursor;
	int cur_time;
	float hrr;
	float avgwt;
	float avgtt;

	struct Node *front;
	struct Node *rear;

} Queue;

void enqueue(struct Queue *queue, struct Node *new_node);
void dequeue(struct Queue *queue);
void sortByArrival(struct Queue *queue);
struct Node *getNode(struct Queue *queue, int cursor);
void moveCursor(struct Queue *queue);

#endif

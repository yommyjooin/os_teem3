#ifndef __HRRN_QUEUE_H
#define __HRRN_QUEUE_H

#include "hrrn_process.h"
#include <string.h>

typedef struct Node {

	struct Node *next;
	struct Process *p;
} Node;

 
typedef struct Queue {

	int sum_bt;	//Total burst time of processes
	int size;	//Number of processes in queue
	int max_size;	//Total number of processes
	int cursor;	//Currently executing process index
	int cur_time;	//Current time
	float hrr;	//Highest response ratio
	float avgwt;	//Total wating time
	float avgtt;	//Total turn-around time
	float avgrt;	//Total response time

	struct Node *front;
	struct Node *rear;

	char* data[];
} Queue;

void enqueue(struct Queue *queue, struct Node *new_node);
void dequeue(struct Queue *queue);
void sortByArrival(struct Queue *queue);
struct Node *getNode(struct Queue *queue, int cursor);
struct Process *moveCursor(struct Queue *queue);
void parseGantt(struct Queue *queue, struct Process *p, int ended_time);

#endif

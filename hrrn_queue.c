#include "hrrn_queue.h"

//Sorting of processes by arrival time
void sortByArrival(struct Queue *queue) {

	struct Node *cur_node = queue->front;
	struct Node *prev_node = NULL;

	//Initalizing queue
	queue->avgtt = 0;
	queue->avgwt = 0;
	queue->hrr = -9999;
	queue->cursor = 0;
	queue->cur_time = queue->front->p->at;

	while (cur_node->next != NULL) {

		//Check for the lesser arrival time
		if (cur_node->p->at > cur_node->next->p->at) {
 
			if (prev_node != NULL) {
				prev_node->next = cur_node->next;
			}

			//Swap earlier process to front
			cur_node->next = cur_node->next->next;
			prev_node->next->next = cur_node;
			cur_node = prev_node->next;
		}

		prev_node = cur_node;
		cur_node = cur_node->next;
	}
}

//Enqueueing process
void enqueue(struct Queue *queue, struct Node *new_node) {

	if (queue->front == NULL || queue->rear == NULL) {
		queue->front = new_node;
		queue->rear = new_node;
	}
	else {
		queue->rear->next = new_node;
		queue->rear = new_node;
	}
	
	queue->size++;
	queue->max_size++;
}

//Dequeueing process with cursor
void dequeue(struct Queue *queue) {

	if (queue->cursor == 0) {
		if (queue->front != NULL) {
			queue->front = queue->front->next;
			if(queue->front == NULL) {
				queue->rear = NULL;		
			}
		}
	}
	else {
		struct Node *prevNode = getNode(queue, queue->cursor-1);
		prevNode->next = getNode(queue, queue->cursor)->next;
		if (queue->cursor == queue->size-1) {
			queue->rear = prevNode;
		}
	}

	queue->size--;
}

//Return node at given index
struct Node *getNode(struct Queue *queue, int cursor) {

	struct Node *cur_node = queue->front;

	for (int i = 0; i < cursor; i++) {
		if (cur_node != NULL) {
			cur_node = cur_node->next;
		}
	}

	return cur_node;

}

//Update cursor to Node with highest response ratio process and return it's process
struct Process *moveCursor(struct Queue *queue) {
	queue->cursor = 0;
	struct Node *cur_node = queue->front;

	float temp; //Response ratio of process in current node
	int i = 0; //Index of process with highest reponse ratio

	while (cur_node != NULL && queue->size > 1) {

		//Check if the process has arrived and incomplete
		if (cur_node->p->at <= queue->cur_time && cur_node->p->completed != 1) {
			
			temp = (cur_node->p->bt + (queue->cur_time - cur_node->p->at)) / cur_node->p->bt;
			
			//Check if it is highest response ratio
			if (queue->hrr < temp) {
				queue->hrr = temp;
				queue->cursor = i;
			}
		}

		cur_node = cur_node->next;
		i++;
	}
	
	return getNode(queue, queue->cursor)->p;
}

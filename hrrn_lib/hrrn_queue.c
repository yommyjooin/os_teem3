#include "hrrn_queue.h"

//Sorting of processes by arrival time
void sortByArrival(struct Queue *queue) {
	
	struct Node *node_array[queue->size];
	int size = queue->size;
	struct Node *temp;

	//Initalizing queue
	queue->avgtt = 0;
	queue->avgwt = 0;
	queue->hrr = -9999;
	queue->cursor = 0;

	for (int i = 0; queue->front != NULL; i++) {
		node_array[i] = queue->front;
		dequeue(queue);
	}

	for (int i = 0; i < size-1; i++) {
		for (int j = i+1; j < size; j++) {
			//Check for the lesser arrival time
			if (node_array[i]->p->at > node_array[j]->p->at) {
				
				//Swap earlier process to front
				temp = node_array[i];
				node_array[i] = node_array[j];
				node_array[j] = temp;
			}
		}
	}

	for (int i = 0; i < size; i++) {
		enqueue(queue, node_array[i]);
	}

	queue->cur_time = queue->front->p->at;
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
}

//Dequeueing process with cursor
void dequeue(struct Queue *queue) {
	
	struct Node *dequeued;
	if (queue->cursor == 0) {
		if (queue->front != NULL) {
			dequeued = queue->front;
			queue->front = queue->front->next;
			if(queue->front == NULL) {
				queue->rear = NULL;		
			}
			dequeued->next = NULL;
		}
	}
	else {
		struct Node *prevNode = getNode(queue, queue->cursor-1);
		dequeued = getNode(queue, queue->cursor);
		prevNode->next = getNode(queue, queue->cursor)->next;
		if (queue->cursor == queue->size-1) {
			queue->rear = prevNode;
		}
		dequeued->next = NULL;
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
	queue->hrr = 0;

	float temp; //Response ratio of process in current node
	int i = 0; //Index of process with highest reponse ratio

	while (cur_node != NULL && queue->size > 1) {

		//Check if the process has arrived and incomplete
		if (cur_node->p->at <= queue->cur_time && cur_node->p->completed != 1) {
			temp = ((float)cur_node->p->bt + ((float)queue->cur_time - (float)cur_node->p->at) + 1) / (float)cur_node->p->bt;
			
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

void parseGantt(struct Queue *queue, struct Process *p, int ended_time) {
	char data_token[10] = "";
	
	sprintf(data_token, "%d ", p->p_num);

	int token_len = strlen(data_token);
	char *data_line = (char*)calloc(token_len * p->bt + 1, sizeof(char));

	//If it is first process
	if (queue->size+1 == queue->max_size) {
		data_line = realloc(data_line, sizeof(char) * (token_len * (p->at + p->bt) + 1));
		for (int i = 0; i < p->at; i++) {
			strcat(data_line, "0 "); //Checking arrival time and storing 0 for running time before first process arrived
		}
	}
	//If there are terms with previous process
	else if (ended_time < p->at) {
		data_line = realloc(data_line, sizeof(char) * (token_len * ((p->at - ended_time) + p->bt) + 1));
		for (int i = 0; i < p->at - ended_time; i++) {
			strcat(data_line, "0 "); //Storing 0 for voided running time
		}		
	}
	else {
		data_line = (char*)calloc(token_len * p->bt + 1, sizeof(char));
	}

	for (int i = 0; i < p->bt; i++) {
		strcat(data_line, data_token); //Storing process number for running time
	}
	
	queue->data[queue->max_size - queue->size - 1] = data_line;
}

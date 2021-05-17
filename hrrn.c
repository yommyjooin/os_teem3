//hrrn.c

#include <stdio.h>
#include <stdlib.h>

typedef struct Process {
	char name;
	int at, bt, ct, wt, tt;
	int completed;
	float ntt;
} Process;

typedef struct Node {
	struct Node *next;
	struct Process *p;
} Node;

typedef struct Queue {
	struct Node *front;
	struct Node *rear;
} Queue;

void sortByArrival(struct Queue *queue) {
	struct Node *cur_node = queue->front;
	struct Node *temp;
	struct Node *prev_node = NULL;

	while (cur_node->next != NULL) {
		if (cur_node->p->at > cur_node->next->p->at) {
			temp = cur_node;

			if (prev_node != NULL) {
				prev_node->next = cur_node->next;
			}
			
			cur_node->next = cur_node->next->next;
			prev_node->next->next = cur_node;
			cur_node = prev_node->next;
		}
		prev_node = cur_node;
		cur_node = cur_node->next;
	}
}

void enqueue(struct Queue *queue, struct Node *new_node) {
	if (queue->front == NULL || queue->rear == NULL) {
		queue->front = new_node;
		queue->rear = new_node;
	}
	else {
		queue->rear->next = new_node;
		queue->rear = new_node;
	}
}

struct Process *getNodeProcess(struct Queue *queue, int loc) {
	struct Node *cur_node = queue->front;
	for (int i = 0; i < loc; i++) {
		if (cur_node != NULL) {
			cur_node = cur_node->next;
		}
	}
	return cur_node->p;
}

void main() {
	int sum_bt = 0;
	char c;
	float avgwt = 0, avgtt = 0;
	int n = 5;
	
	struct Queue *hrrn_queue = malloc(sizeof(struct Queue));

	int arriv[] = { 0, 2, 4, 6, 8 };
	int burst[] = { 3, 6, 4, 5, 2 };

	for (int i = 0, c = 'A'; i < n; i++, c++) {
		struct Node *new_node = malloc(sizeof(struct Node));
		struct Process *new_process = malloc(sizeof(struct Process));
		new_node->p = new_process;

		new_node->p->name = c;
		new_node->p->at = arriv[i];
		new_node->p->bt = burst[i];
		new_node->p->completed = 0;
		sum_bt += new_node->p->bt;

		enqueue(hrrn_queue, new_node);
	}
	sortByArrival(hrrn_queue);
	printf("\nName\tArrival Time\tBurst Time\tWating Time");
	printf("\tTurnAround Time\t Normalized TT");
	for (int t = hrrn_queue->front->p->at; t < sum_bt;) {
		float hrr = -9999;
		float temp;
		int loc;
		int i = 0;
		struct Node *cur_node = hrrn_queue->front;
		while (cur_node != NULL) {
			if (cur_node->p->at <= t && cur_node->p->completed != 1) {
				temp = (cur_node->p->bt + (t - cur_node->p->at)) / cur_node->p->bt;

				if (hrr < temp) {
					hrr = temp;

					loc = i;
				}
			}
			cur_node = cur_node->next;
			i++;
		}
		printf("\nloc: %d", loc);	
		struct Process *target = getNodeProcess(hrrn_queue, loc);
		t += target->bt;
		target->wt = t - target->at - target->bt;
		target->tt = t - target->at;

		avgtt += target->tt;

		target->ntt = ((float)target->tt / target->bt);

		target->completed = 1;

		avgwt += target->wt;
		printf("\n%c\t\t%d\t\t", target->name, target->at);
		printf("%d\t\t%d\t\t", target->bt, target->wt);
		printf("%d\t\t%f", target->tt, target->ntt);
	}
	printf("\nAverage waiting time:%f\n", avgwt / n);
	printf("Average Turn Around time:%f\n", avgtt / n);
}

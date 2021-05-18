#include "scheduling_io.h"

int parseProcess(struct Queue *queue) {
	printf("%s", input_data_path);
	char *resource_path = input_data_path;
    	char buffer[MAX_LINE_LENGTH];
    	FILE * fp;

	if ((fp = fopen(resource_path, "rb")) == NULL) {
		return 1;
	}

	for(int i = 0; fgets(buffer, MAX_LINE_LENGTH, fp) != NULL; i++){

		struct Node *new_node = malloc(sizeof(struct Node));
		struct Process *new_process = malloc(sizeof(struct Process));
		char * temp = strtok(buffer, " ");

		new_node->p = new_process;
		new_node->p->p_num = atoi(temp);

        	temp = strtok(NULL, " ");

		new_node->p->at = atoi(temp);

        	temp = strtok(NULL, " ");

		new_node->p->bt = atoi(temp);
        	new_node->p->completed = 0;

		queue->sum_bt += new_node->p->bt;
		enqueue(queue, new_node);
	}
	fclose(fp);

	queue->avgtt = 0;
	queue->avgwt = 0;
	queue->hrr = -9999;
	queue->cursor = 0;
	
	return 0;
}

void printBursted(struct Queue *queue) {
	struct Process *target = getNode(queue, queue->cursor)->p;

	queue->cur_time += target->bt;
	target->wt = queue->cur_time - target->at - target->bt;
	target->tt = queue->cur_time - target->at;

	queue->avgtt += target->tt;
	target->ntt = ((float)target->tt / target->bt);

	target->completed = 1;
	queue->avgwt += target->wt;

	printf("\n%d\t\t%d\t\t", target->p_num, target->at);
	printf("%d\t\t%d\t\t", target->bt, target->wt);
	printf("%d\t\t%f", target->tt, target->ntt);
}

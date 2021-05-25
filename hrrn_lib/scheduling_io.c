#include "scheduling_io.h"

//Parsing process information from text file
int parseProcess(struct Queue *queue) {
	printf("Opening data file from: \"%s\"...\n", INPUT_DATA_PATH);
	char *resource_path = INPUT_DATA_PATH;
    	char buffer[MAX_LINE_LENGTH];
    	FILE * fp;

	if ((fp = fopen(resource_path, "rb")) == NULL) {
		return 1;
	}

	for(int i = 0; fgets(buffer, MAX_LINE_LENGTH, fp) != NULL; i++){

		struct Node *new_node = malloc(sizeof(struct Node));
		char *temp = strtok(buffer, " ");

		if (temp == NULL || temp[0] == 10 || temp[0] == 21) {continue;}
		int p_num = atoi(temp); //Process id

        	temp = strtok(NULL, " ");
		if (temp == NULL) {return 2;}
		int p_at = atoi(temp); //Process arrival time

        	temp = strtok(NULL, " ");
		if (temp == NULL) {return 2;}
		int p_bt = atoi(temp); //Process burst time
		
		new_node->p = createProcess(p_num, p_at, p_bt);

		queue->sum_bt += new_node->p->bt; //Adding total process burst time
		queue->max_size++;
		enqueue(queue, new_node);
	}

	fclose(fp);
	return 0;
}

//Printing information of completed process
void printBursted(struct Process *p) {
	
	printf("%d\t\t%d\t\t", p->p_num, p->at);
	printf("%d\t\t%d\t\t", p->bt, p->wt);
	printf("%d\t\t%f\t\t%d\n", p->tt, p->ntt, p->rt);
}

void printText(struct Queue *queue) {
	char *path = malloc(sizeof(char) * (strlen(OUTPUT_DATA_DIR) + strlen(OUTPUT_DATA_NAME)) + 2);
	strcpy(path, OUTPUT_DATA_DIR);
	strcat(path, "/");
	strcat(path, OUTPUT_DATA_NAME);

	if (mkdir(OUTPUT_DATA_DIR, 0755) == -1 && errno != EEXIST) {
		printf("Can't create directory in: \"%s\"\nError detected: %s\n", OUTPUT_DATA_DIR, strerror(errno));
		return;
	}
	FILE *fp = fopen(path, "w+");
	
	if (fp == NULL) {
		printf("Can't write gantt data in directory: \"%s\"\n", path);
		return;	
	}

	for (int i = 0; i < queue->max_size; i++) {
		fputs(queue->data[i], fp);
		free(queue->data[i]);
	}

	fclose(fp);
}

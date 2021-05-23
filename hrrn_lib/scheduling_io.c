#include "scheduling_io.h"

//Parsing process information from text file
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
        char *temp = strtok(buffer, " ");

        int p_num = atoi(temp); //Process id
            temp = strtok(NULL, " ");

        int p_at = atoi(temp); //Process arrival time
            temp = strtok(NULL, " ");

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

    printf("\n%d\t\t%d\t\t", p->p_num, p->at);
    printf("%d\t\t%d\t\t", p->bt, p->wt);
    printf("%d\t\t%f\t\t%d", p->tt, p->ntt, p->rt);
}

void printText(struct Queue *queue) {
    FILE *fp = fopen("./data/gantt/hrrn_gantt.txt", "w+");

    fputs(queue->data, fp);

    fclose(fp);
}
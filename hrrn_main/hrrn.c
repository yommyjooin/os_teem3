#include "hrrn_queue.h"
#include "scheduling_io.h"

int main() {

    struct Queue *hrrn_queue = malloc(sizeof(struct Queue)); //Creating queue contain processes

    //Return 1 and quit if failed to load text file
    if (parseProcess(hrrn_queue)) {
        return 1;
    }

    sortByArrival(hrrn_queue);

    printf("\nId\tArrival Time\tBurst Time\tWating Time");
    printf("\tTurnAround Time\t Normalized TT\t Response Time");

    while (hrrn_queue->size > 0) {

        struct Process *target = moveCursor(hrrn_queue); //Getting hightest response ratio process

        hrrn_queue->cur_time += target->bt; //Updating current time;

        updateProcessInfo(target, hrrn_queue->cur_time);

        hrrn_queue->avgtt += target->tt; //Adding turn around time of process to total turn around time
        hrrn_queue->avgwt += target->wt; //Adding waiting time of process to total waiting time
        hrrn_queue->avgrt += target->rt; //Adding response time of process to total response time

        printBursted(target);
        dequeue(hrrn_queue);
        parseGantt(hrrn_queue, target);
    }

    printf("\nAverage waiting time:%f\n", hrrn_queue->avgwt / hrrn_queue->max_size);
    printf("Average Turn Around time:%f\n", hrrn_queue->avgtt / hrrn_queue->max_size);
    printText(hrrn_queue);

    return 0;
}
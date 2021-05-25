#include "hrrn_queue.h"
#include "scheduling_io.h"

int main() {

	struct Queue *hrrn_queue = (Queue*)malloc(sizeof(struct Queue)); //Creating queue contain processes
	
	int error_code = parseProcess(hrrn_queue);
	//Return 1 and quit if failed to load text file
	if (error_code) {
		if (error_code == 1) {printf("Invalid input file: %s\n", INPUT_DATA_PATH);}
		else if (error_code == 2) {printf("Invalid form of contents in file: %s\n", INPUT_DATA_PATH);}
		return 1;
	}
	
	hrrn_queue = (Queue*)realloc(hrrn_queue, sizeof(struct Queue) + sizeof(char*) * hrrn_queue->max_size);	
	
	sortByArrival(hrrn_queue);

	printf("\nId\tArrival Time\tBurst Time\tWating Time");
	printf("\tTurnAround Time\t Normalized TT\n");

	while (hrrn_queue->size > 0) {
		int prev_ended_time = 0; //Store previous process' end time.
		
		struct Process *target = moveCursor(hrrn_queue); //Getting hightest response ratio process

		//If there are no terms with previous process
		if (target->at <= hrrn_queue->cur_time) {
			hrrn_queue->cur_time += target->bt; //Updating current time;
			prev_ended_time = hrrn_queue->cur_time; //Updating endtime of previous process
		}
		//If there are terms with previous process
		else {
			prev_ended_time = hrrn_queue->cur_time; //Updating endtime of previous process
			hrrn_queue->cur_time = target->bt + target->at; //Updating current time;
		}
		updateProcessInfo(target, hrrn_queue->cur_time);

		hrrn_queue->avgtt += target->tt; //Adding turn around time of process to total turn around time
		hrrn_queue->avgwt += target->wt; //Adding waiting time of process to total waiting time
		hrrn_queue->avgrt += target->rt; //Adding response time of process to total response time
		
		printBursted(target);
		dequeue(hrrn_queue);
		parseGantt(hrrn_queue, target, prev_ended_time);
	}

	printf("\nAverage Waiting Time:%f\n", hrrn_queue->avgwt / hrrn_queue->max_size);
	printf("Average Turn Around Time:%f\n", hrrn_queue->avgtt / hrrn_queue->max_size);
	printf("Average Response Time:%f\n", hrrn_queue->avgrt / hrrn_queue->max_size);
	printText(hrrn_queue);
	free(hrrn_queue);
	return 0;
}

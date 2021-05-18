#include "hrrn_queue.h"
#include "scheduling_io.h"

int main() {

	struct Queue *hrrn_queue = malloc(sizeof(struct Queue));

	if (parseProcess(hrrn_queue)) {
		return 1;
	}

	sortByArrival(hrrn_queue);

	printf("\nId\tArrival Time\tBurst Time\tWating Time");
	printf("\tTurnAround Time\t Normalized TT");

	for (hrrn_queue->cur_time = hrrn_queue->front->p->at; hrrn_queue->cur_time < hrrn_queue->sum_bt && hrrn_queue->size > 0;) {
		
		moveCursor(hrrn_queue);

		printBursted(hrrn_queue);
		dequeue(hrrn_queue);
	}

	printf("\nAverage waiting time:%f\n", hrrn_queue->avgwt / hrrn_queue->max_size);
	printf("Average Turn Around time:%f\n", hrrn_queue->avgtt / hrrn_queue->max_size);

	return 0;
}

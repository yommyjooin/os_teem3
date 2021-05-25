#include <stdio.h>
#include <stdlib.h>

typedef struct Process {

	int p_num; //process id
	int at, bt, wt, tt, rt; //arrival time, burst time, wating time, turn-around time, response time
	int completed;
	float ntt; //normalized turn-around time

} Process;

struct Process *createProcess(int id, int at, int bt);
void updateProcessInfo(struct Process *p, int cur_time);

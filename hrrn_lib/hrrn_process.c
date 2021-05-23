#include "hrrn_process.h"

//Create process
struct Process *createProcess(int id, int at, int bt) {

    struct Process *new_process = malloc(sizeof(struct Process));

    new_process->p_num = id;
    new_process->at = at;
    new_process->bt = bt;
    new_process->completed = 0;

    return new_process;
}

//Calculate information of processes
void updateProcessInfo(struct Process *p, int cur_time) {

    p->wt = cur_time - p->at - p->bt; //Calculating waiting time
    p->tt = cur_time - p->at; //Calculating turn around time
    p->ntt = ((float)p->tt / p->bt); //Calculating normalized turn around time
    p->rt = p->wt; //Calculate response time
    p->completed = 1; //Updating completion of process
}
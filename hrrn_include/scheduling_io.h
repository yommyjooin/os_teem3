#ifndef __SCHEDULING_IO_H
#define __SCHEDULING_IO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hrrn_queue.h"

#define MAX_CNT 5
#define MAX_LINE_LENGTH 20
#define input_data_path "./data/hrrn_task_data.txt" //Path of text file including processes information

int parseProcess(struct Queue *queue);
void printBursted(struct Process *p);

#endif



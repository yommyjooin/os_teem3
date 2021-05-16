#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_CNT 2
#define MAX_LINE_LENGTH 20
#define input_data_path "./edf_task_data.txt"

int timer = 0;
int idle_time = 0;

// ?�로?�스 ?�행?? ?�요?? ?�보�? ?�는 구조�? (arrival time, burst time, deadline)
typedef struct Task {
    int arrival_t;
    int burst_t;
    int deadline;
} Task;

typedef struct TaskStatus {
    int waiting_t;
    int turnaround_t;
    int response_t;
    int remain_burst_t;
    int remain_deadline;
    int cpu_flag;
    Task task;
} TaskStatus;

typedef struct Node
{
    TaskStatus data;
    struct Node *next;
}Node;
 
 
typedef struct Queue
{
    Node *front;
    Node *rear;
    int count;
}Queue;
 
void init_queue(Queue *queue);
int is_empty(Queue *queue);
void enqueue(Queue *queue, TaskStatus data);
void sorted_enqueue(Queue *queue, TaskStatus data);
TaskStatus dequeue(Queue *queue);

Queue ready_queue;

void init_queue(Queue *queue)
{
    queue->front = queue->rear = NULL;
    queue->count = 0;
}

int is_empty(Queue *queue)
{
    return queue->count == 0;
}

void enqueue(Queue *queue, TaskStatus data)
{

}

void sorted_enqueue(Queue *queue, TaskStatus data)
{
    Node *now = (Node *)malloc(sizeof(Node));
    now->data = data;
    now->next = NULL;
 
    if (is_empty(queue))
    {
        queue->front = now;
    }
    else
    {
        Node * curr = queue->front;
        Node * prev = NULL;
        for(int i = 0 ; i < queue->count; i++)
        {
            if (curr->data.remain_deadline > now->data.remain_deadline)
                break;
            prev = curr;
            curr = curr->next;
        }
        if (curr == queue->front)
        {
            now->next = queue->front;
            queue->front = now;
        }
        else if (curr == NULL)
        {
            now->next = NULL;
            queue->rear->next = now;
        }
        else
        {
            prev->next = now;
            now->next = curr;
        }
    }
    queue->count++;
}
 
TaskStatus dequeue(Queue *queue)
{
    TaskStatus re;
    Node *now;
    if (is_empty(queue))
    {
        re.cpu_flag = -1;
        return re;
    }
    now = queue->front;
    re = now->data;
    queue->front = now->next;
    free(now);
    queue->count--;
    return re;
}

Task tasks[MAX_CNT];

void input_read(){
    char *resource_path = input_data_path;
    char buffer[MAX_LINE_LENGTH];
    Task task;
    int file_size = 0;
    FILE * fp;
    
    if ((fp = fopen(resource_path, "rb")) == NULL) {
        return;
    }
    for(int i = 0; fgets(buffer, MAX_LINE_LENGTH, fp) != NULL; i++){
        char * temp = strtok(buffer, " ");
        task.arrival_t = atoi(temp);
        temp = strtok(NULL, " ");
        task.burst_t = atoi(temp);
        temp = strtok(NULL, " ");
        task.deadline = atoi(temp);
        // printf("arrival: %d burst: %d deadline: %d\n", task.arrival_t, task.burst_t, task.deadline);
        tasks[i] = task;
    }
    fclose(fp);
}

void ready_sorted_enqueue(){
    for (int i = 0; i < MAX_CNT; i++)
    {
        if (tasks[i].arrival_t == timer)
        {
            TaskStatus task_status;
            task_status.turnaround_t = 0;
            task_status.waiting_t = 0;
            task_status.response_t = 0;
            task_status.remain_burst_t = tasks[i].burst_t;
            task_status.cpu_flag = 0;
            task_status.task = tasks[i];
            task_status.remain_deadline = tasks[i].deadline + timer;
            sorted_enqueue(&ready_queue, task_status);
        }
        else if (timer > tasks[i].arrival_t && (timer - tasks[i].arrival_t) % tasks[i].deadline == 0)
        {
            TaskStatus task_status;
            task_status.turnaround_t = 0;
            task_status.waiting_t = 0;
            task_status.response_t = 0;
            task_status.remain_burst_t = tasks[i].burst_t;
            task_status.cpu_flag = 0;
            task_status.task = tasks[i];
            task_status.remain_deadline = tasks[i].deadline + timer;
            sorted_enqueue(&ready_queue, task_status);
        }
    }
}

void update_ready_queue(Queue *queue) // deadline, response time, turnaround time, waiting
{
    Node * cur = queue->front;
    if(queue->front != NULL)
    {
        for(int i = 0; i < queue->count; i++)
        {
            cur->data.turnaround_t += 1; // always increase
            if(cur != queue->front)
            {
                cur->data.waiting_t += 1; // increasse waiting time when not grabbing cpu
            }
            if(cur->data.cpu_flag == 1)
            {
                cur->data.response_t += 1;
            }
            if(cur->data.remain_deadline > timer)
            {
                // dequeue
            }
            
            cur = cur->next;
        }
    }
    return;
}

void update_running_task(){
    TaskStatus * running_task_status =  &ready_queue.front->data;
    if (running_task_status->cpu_flag == -1)
    {
        idle_time += 1;
        return;
    }
    else if (running_task_status->cpu_flag == 0)
    {
        running_task_status->cpu_flag = 1;
        running_task_status->response_t = running_task_status->waiting_t;
    }
    else if (running_task_status->cpu_flag == 1)
    {
        running_task_status->remain_deadline -= 1;
        running_task_status->remain_burst_t -= 1;
        running_task_status->turnaround_t += 1;
    }
    
    if (running_task_status->remain_burst_t <= 0)
    {
        dequeue(&ready_queue);
        return;
    }
}

void cal_performance(Queue* result)
{
    float tot_waiting_t = 0.0;
    float tot_turnaround_t = 0.0;
    float tot_response_t = 0.0;
    float avg_waiting_t = 0.0;
    float avg_turnaround_t = 0.0;
    float avg_response_t = 0.0; 
    int total_count = result->count;   
    
    while(result->count > 0){
        TaskStatus task = dequeue(result);
        tot_waiting_t += task.waiting_t;
        tot_turnaround_t += task.turnaround_t;
        tot_response_t += task.response_t;
    }

    avg_waiting_t = tot_waiting_t/total_count;
    avg_turnaround_t = tot_turnaround_t/total_count;
    avg_response_t = tot_response_t/total_count;

    printf("avg waiting time: %.3f\n",avg_waiting_t);
    printf("avg turnaround time: %.3f\n",avg_turnaround_t);
    printf("avg response time: %.3f\n",avg_response_t);
}

int main(void){
    init_queue(&ready_queue);
    input_read();
    while(timer < 160){
        ready_sorted_enqueue();
        update_ready_queue();
        update_running_task();
        timer++;
    }
    // cal_performance(result_queue);
    return 0;
}
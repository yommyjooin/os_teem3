//edf.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_CNT 2
#define MAX_LINE_LENGTH 20
#define input_data_path "./edf_task_data.txt"

int timer = 0;
int idle_time = 0;

// 프로세스 실행에 필요한 정보를 담는 구조체 (arrival time, burst time, deadline)
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
 
 
typedef struct Queue //Queue 구조체 정의
{
    Node *front; //맨 앞(꺼낼 위치)
    Node *rear; //맨 뒤(보관할 위치)
    int count;//보관 개수
}Queue;
 
void init_queue(Queue *queue);//큐 초기화
int is_empty(Queue *queue); //큐가 비었는지 확인
void enqueue(Queue *queue, TaskStatus data); //큐에 보관
void sorted_enqueue(Queue *queue, TaskStatus data); //큐에 보관
TaskStatus dequeue(Queue *queue); //큐에서 꺼냄
 
Queue ready_queue;
 
void init_queue(Queue *queue)
{
    queue->front = queue->rear = NULL; //front와 rear를 NULL로 설정
    queue->count = 0;//보관 개수를 0으로 설정
}
 
int is_empty(Queue *queue)
{
    return queue->count == 0;    //보관 개수가 0이면 빈 상태
}

void enqueue(Queue *queue, TaskStatus data)
{

}

void sorted_enqueue(Queue *queue, TaskStatus data)
{
    Node *now = (Node *)malloc(sizeof(Node)); //노드 생성
    now->data = data;//데이터 설정
    now->next = NULL;
 
    if (is_empty(queue))//큐가 비어있을 때
    {
        queue->front = now;//맨 앞을 now로 설정
    }
    else//비어있지 않을 때
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
        // enqueue 되는 값이 가장 작은 deadline을 가질 경우
        if (curr == queue->front)
        {
            now->next = queue->front;
            queue->front = now;
        }
        // enqueue 되는 값이 가장 클 deadline을 가질 경우
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
    queue->count++;//보관 개수를 1 증가
}
 
TaskStatus dequeue(Queue *queue)
{
    TaskStatus re;
    Node *now;
    if (is_empty(queue))//큐가 비었을 때
    {
        re.cpu_flag = -1;
        return re;
    }
    now = queue->front;//맨 앞의 노드를 now에 기억
    re = now->data;//반환할 값은 now의 data로 설정
    queue->front = now->next;//맨 앞은 now의 다음 노드로 설정
    free(now);//now 소멸
    queue->count--;//보관 개수를 1 감소
    return re;
}

// 프로세스 실행 정보들의 배열
Task tasks[MAX_CNT];

void input_read(){
    char *resource_path = input_data_path;
    char buffer[MAX_LINE_LENGTH];
    Task task;
    int file_size = 0;
    FILE * fp;
    
    /* 파일을 연다. */
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
            task_status.remain_deadline = tasks[i].deadline;
            sorted_enqueue(&ready_queue, task_status);
        }
    }
}

/*
    ready queue에 들어가 있는 task들의 속성들을 1초 업데이트 한다.
*/
void update_ready_queue(){

}

/*
    현재 CPU를 통해 실행중인 task의 속성들을 1초 업데이트 한다.
*/
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

/*
    스케줄링의 전반적인 수행결과를 계산한다.
*/
void cal_performance()
{

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
    cal_performance();
    return 0;
}
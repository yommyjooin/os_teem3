#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>

#define MAX_CNT 10
#define MAX_LINE_LENGTH 20
#define input_data_path "./edf_task_data.txt"

int timer = 0;
int idle_time = 0;
int task_num = 0; // this indicate how many task in txt file, initialized in input_read
int dequeued_task = 0;

typedef struct Task {
    int arrival_t;
    int burst_t;
    int deadline;
    int pid;
} Task;

typedef struct TaskStatus {
    int waiting_t;
    int turnaround_t;
    int response_t;
    int remain_burst_t;
    int deadline;
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
Queue result_queue;

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
        queue->rear = now;
    }
    else
    {
        Node * curr = queue->front;
        Node * prev = NULL;
        for(int i = 0 ; i < queue->count; i++)
        {
            if (curr->data.deadline > now->data.deadline)
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
		    queue->rear = now;
		    if(queue->count == 1)
		        queue->front->next = now;
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
    now = queue->front;
    re = now->data;
    queue->front = now->next;
    free(now);
    queue->count--;
    dequeued_task += 1; // count task when dequeue
    return re;
}

//void print_queue(Queue *queue)
//{
//	Node * cur = queue->front;
//	while(curr != NULL)
//	    {
//		TaskStatus s = cur->data;
//		printf("--TaskStatus Report--\n");
//		printf("id : %d, remain_burst : %d, 
//	            cur = cur->next;
//	        }
//	    }
//}

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
        task.pid = i + 1;
        // printf("arrival: %d burst: %d deadline: %d\n", task.arrival_t, task.burst_t, task.deadline);
        tasks[i] = task;
        task_num += 1;
    }
    fclose(fp);
}

void ready_sorted_enqueue(){
    for (int i = 0; i < task_num; i++) // not max_cnt, 
    {
        if ((tasks[i].arrival_t + 1) == timer)
        {
            TaskStatus task_status;
            task_status.turnaround_t = 0;
            task_status.waiting_t = 0;
            task_status.response_t = 0;
            task_status.remain_burst_t = tasks[i].burst_t;
            task_status.cpu_flag = 0;
            task_status.task = tasks[i];
            task_status.deadline = tasks[i].deadline;
            printf("%ds PID: %d INSERTED\n", timer - 1, tasks[i].pid);
            sorted_enqueue(&ready_queue, task_status);
        }
        // delete because this consider deadline = period
        // else if (timer > (tasks[i].arrival_t + 1) && ((timer - (tasks[i].arrival_t + 1)) % tasks[i].deadline) == 0)
        // {
		// printf("insert because of period\n");
        //     TaskStatus task_status;
        //     task_status.turnaround_t = 0;
        //     task_status.waiting_t = 0;
        //     task_status.response_t = 0;
        //     task_status.remain_burst_t = tasks[i].burst_t;
        //     task_status.cpu_flag = 0;
        //     task_status.task = tasks[i];
        //     task_status.deadline = tasks[i].deadline + timer;
        //     printf("%ds PID: %d INSERTED\n", timer - 1, tasks[i].pid);
        //     sorted_enqueue(&ready_queue, task_status);
        // }
    }
}

/*
    timer를 먼저 증가시키고 update
    deadline == timer -> dequeue
    remain_burst == 0
    deadline == timer
*/

void update(){
    // 실행중인 task
    TaskStatus * running_task_status = &ready_queue.front->data;
    Node * curr = ready_queue.front;
    if (is_empty(&ready_queue) == 1)
    {
        printf("%ds IDLE\n", timer);
        idle_time += 1;
        return;
    }
    else if (running_task_status->cpu_flag == 0)
    {
        running_task_status->cpu_flag = 1;
        printf("%ds PID: %d CPU FLAG ON\n", timer - 1, running_task_status->task.pid);
    }
    running_task_status->remain_burst_t -= 1; // --> 이미 0임 (실행은 됐다)
    
    running_task_status->turnaround_t += 1;

    if (running_task_status->remain_burst_t <= 0)
    {
        printf("%ds PID: %d CPU BURST\n", timer, running_task_status->task.pid);
        TaskStatus task = dequeue(&ready_queue);
        sorted_enqueue(&result_queue, task); // debug
        if (is_empty(&ready_queue)) return;
        curr = ready_queue.front;
    }
    while(curr != NULL)
    {
        if (&curr->data != running_task_status || (&curr->data == running_task_status && running_task_status->cpu_flag == 0)){
            curr->data.waiting_t += 1;
            curr->data.turnaround_t += 1;
        }
        if (curr->data.cpu_flag == 0)
            curr->data.response_t += 1;
        
        if (curr->data.deadline == timer)
        {
            printf("%ds PID: %d DEADLINE OVER\n", timer, curr->data.task.pid);
            TaskStatus task = dequeue(&ready_queue); //
            sorted_enqueue(&result_queue, task); // debug
            curr = ready_queue.front;
        }
        else
        {
            curr = curr->next;
        }
    }
}

// concatenate back to end of front string
void fwrite_gantt(FILE * fp, char * tmp_buffer, int pid)
{
    memset(tmp_buffer, 0, 1000);
    sprintf(tmp_buffer, "%d ", pid);
    int remain = pid / 10;
    if(remain == 0)
    {
        fwrite(tmp_buffer, 2, 1, fp); // 1~9 + space
    }
    else if(remain < 10)
    {
        fwrite(tmp_buffer, 3, 1, fp); // 10~99 + space
    }
    else
    {
        fwrite(tmp_buffer, 4, 1, fp); // 100~ + space
    }
}

void update_gantt(FILE * fp, char * tmp_buffer){
    // 실행중인 task
    TaskStatus * running_task_status = &ready_queue.front->data;
    Node * curr = ready_queue.front;
    if (is_empty(&ready_queue) == 1)
    {
        printf("%ds IDLE\n", timer);
        idle_time += 1;
        fwrite_gantt(fp, tmp_buffer, 0);
        return;
    }
    else if (running_task_status->cpu_flag == 0)
    {
        running_task_status->cpu_flag = 1;
        printf("%ds PID: %d CPU FLAG ON\n", timer - 1, running_task_status->task.pid);
    }
    running_task_status->remain_burst_t -= 1; // --> 이미 0임 (실행은 됐다)
    running_task_status->turnaround_t += 1;

    fwrite_gantt(fp, tmp_buffer, running_task_status->task.pid);

    if (running_task_status->remain_burst_t <= 0)
    {
        printf("%ds PID: %d CPU BURST\n", timer, running_task_status->task.pid);
        TaskStatus task = dequeue(&ready_queue);
        sorted_enqueue(&result_queue, task); // debug
        if (is_empty(&ready_queue)) return;
        curr = ready_queue.front;
    }
    while(curr != NULL)
    {
        if (&curr->data != running_task_status || (&curr->data == running_task_status && running_task_status->cpu_flag == 0)){
            curr->data.waiting_t += 1;
            curr->data.turnaround_t += 1;
        }
        if (curr->data.cpu_flag == 0)
            curr->data.response_t += 1;
        
        if (curr->data.deadline == timer)
        {
            printf("%ds PID: %d DEADLINE OVER\n", timer, curr->data.task.pid);
            TaskStatus task = dequeue(&ready_queue); //
            sorted_enqueue(&result_queue, task); // debug
            curr = ready_queue.front;
        }
        else
        {
            curr = curr->next;
        }
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
    float cpu_utilization = 0.0; 
    float throughput = 0.0; 
    int total_count = result->count;   
    
    printf("\n");
    while(result->count > 0){
        TaskStatus task = dequeue(result);
        tot_waiting_t += task.waiting_t;
        tot_turnaround_t += task.turnaround_t;
        tot_response_t += task.response_t;
        printf("PID : %3d response : %5d turnaround : %5d waiting : %5d remain_burst : %5d\n",
            task.task.pid, task.response_t, task.turnaround_t, task.waiting_t, task.remain_burst_t);
    }

    avg_waiting_t = tot_waiting_t/total_count;
    avg_turnaround_t = tot_turnaround_t/total_count;
    avg_response_t = tot_response_t/total_count;
    cpu_utilization = 1.0 - (float)idle_time/timer;
    throughput = (float)total_count/timer;

    printf("avg waiting time: %.3f sec\n",avg_waiting_t);
    printf("avg turnaround time: %.3f sec\n",avg_turnaround_t);
    printf("avg response time: %.3f sec\n",avg_response_t);
    printf("cpu utilization: %.3f\n",cpu_utilization);
    printf("throughput: %.3f (process/sec)\n",throughput);
}

void new_input_read(char* filename) // not using const string, using parameter to read file
{
    char *resource_path = filename;
    //char *resource_path =  "./edf_task_data.txt";
    char buffer[MAX_LINE_LENGTH];
    Task task;
    int file_size = 0;
    FILE * fp;
    
    if ((fp = fopen(resource_path, "rb")) == NULL) {
        printf("file read failed : %s\n", resource_path);
        perror("");
        return;
    }
    printf("\nCurrent txt file is : %s\n", filename); // debug option
    for(int i = 0; fgets(buffer, MAX_LINE_LENGTH, fp) != NULL; i++){
        char * temp = strtok(buffer, " ");
        task.arrival_t = atoi(temp);
        temp = strtok(NULL, " ");
        task.burst_t = atoi(temp);
        temp = strtok(NULL, " ");
        task.deadline = atoi(temp);
        task.pid = i + 1;
        // printf("arrival: %d burst: %d deadline: %d\n", task.arrival_t, task.burst_t, task.deadline);
        tasks[i] = task;
        task_num += 1;
    }
    fclose(fp);
}

int is_completed()
{
    if(task_num == dequeued_task) // break loop when the number of tasks in txt file and dequeued task is same
    {
        return 0;
    }
    return 1;
}

int sim(char* filename){ // fix this later
    task_num = 0; // trace the number of tasks in txt file
    dequeued_task = 0;
    init_queue(&ready_queue);
    init_queue(&result_queue);
    //input_read();
    new_input_read(filename);
    while(is_completed())
    {
        timer++;
	    //printf("current time is : %d\n", timer);
        ready_sorted_enqueue();
        update();
    }
    cal_performance(&result_queue);
    return 0;
}

int sim_gantt(char* filename){ // only works in ./edf_task_data.txt
    task_num = 0; // trace the number of tasks in txt file
    dequeued_task = 0;
    init_queue(&ready_queue);
    init_queue(&result_queue);
    //input_read();
    new_input_read(filename);

    FILE * fp;
    if((fp = fopen("./data/gantt/gantt.txt", "wb")) == -1)
    {
        perror("failed to open ./gantt.txt");
        return -1;
    }
    char * tmp_buffer = (char*) malloc(sizeof(char) * 1000); // size is fixed 1000, size must be same with fwrite_gantt() - memset
    while(is_completed())
    {
        timer++;
	    //printf("current time is : %d\n", timer);
        ready_sorted_enqueue();
        update_gantt(fp, tmp_buffer); // automatically writes gantt.txt data
    }
    cal_performance(&result_queue);
    free(tmp_buffer);
    return 0;
}

int parse_file_name(char* filename) // return 1 if file name contains .txt  new
{
	strtok(filename, ".");
	char *str = strtok(NULL, ".");
	if(str != NULL && !strcmp(str, "txt")) // same = 0, so put !
	{
		return 1;
	}
	return 0;
}

int main(void)
{
    char test_dir[] = "./edf_test";
    DIR *d;
    struct dirent *dir;
    d = opendir(test_dir);
    if (d)
    {
        char * filename = malloc(sizeof(char) * 100);
        while ((dir = readdir(d)) != NULL) // check all file in current directory
        {
            if(parse_file_name(dir->d_name)) // if the directory has a .txt file, then this is true
            {
                memset(filename, 0, 100);
                strcat(filename, "./edf_test/");
                strcat(filename, dir->d_name);
                strcat(filename, ".txt");
                if(strcmp(filename, "./edf_test/edf_task_data.txt") == 0)
                {
                    printf("--writing gantt--\n");
                    sim_gantt(filename); // write txt data for gantt chart
                }
                else
                {
                    sim(filename);
                }
                timer = 0; // should report total time before this line
            }
        }
        free(filename);
    }
    closedir(d);
    return(0);
}

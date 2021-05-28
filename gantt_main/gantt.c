#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>

int total_time;
char* pids;

// parsing input txt file, then set total time
void parse_input(char* resource_path){
    FILE * fp;
    
    // open file
    if ((fp = fopen(resource_path, "rb")) == NULL) {
        return;
    }
    // getting file size
    fseek(fp, 0, SEEK_END);
    int file_size = ftell(fp);
    char buffer[file_size];
    fseek(fp, 0, SEEK_SET);

    // compute total time
    total_time = (file_size - 1 )/ 2;
    printf("file size: %d, total time: %d\n",file_size, total_time);

    // read file then save in buffer.
    while(fread(buffer, file_size, 1, fp) != 0){
    }
    fclose(fp);

    // remove the white space then save to array 'pid'.
    pids = malloc(sizeof(char)*total_time);
    int j=0;
    for(int i=0; i<file_size; i=i+2){ 
        pids[j]=buffer[i];
        j++;
    }
    pids[j] = '\0';

    /*
    * why error?
    */
    // allocate memory except white space.
    // pids = malloc(sizeof(char)*total_time+1);
    
    // // tokenize by space then save in pids
    // char* ptr = strtok(buffer, " ");
    // sprintf(pids,"%s", ptr);

    // while(ptr != NULL){
    //     strcat(pids, ptr);
    //     ptr = strtok(NULL, " ");
    // }
    // pids[total_time] = '\0';
}

void print_gantt(){
    char l1[total_time*2]; // border of chart. 
    char l2[total_time*2]; // PID
    char l3[total_time*2]; // context switch time

    // flag to check whether PID is printed.
    short is_id_printed=0;

    for(int i = 0; i<total_time; i++){
        // print only at first.
        if(i==0){
            sprintf(l1,"+");
            sprintf(l2,"|");
            sprintf(l3,"0");
        }
        // no context switching
        if(pids[i] == pids[i+1]){        
            strcat(l1,"-");

            // run once per one task.
            if(is_id_printed == 0){
                strcat(l1,"-");

                // print PID & if idle -> X
                if(pids[i] == '0'){
                    strcat(l2,"X ");
                }
                else{
                    char pidToStr[10];
                    sprintf(pidToStr,"%c",pids[i]);
                    strcat(l2,"P");
                    strcat(l2,pidToStr);
                }
                // if time is 1 digit, add one white space more.
                if(i<10){
                    strcat(l3," ");
                }
                // set flag 1 to make it run once.
                is_id_printed = 1;
            }
            else{
                strcat(l2," ");
            }

            strcat(l3," ");
        }
        // if there's a context switching.
        else{// if(pids[i] != pids[i+1]){
            char iStr[10];
            sprintf(iStr,"%d",i+1);
            
            strcat(l1,"+");
            strcat(l2,"|");
            strcat(l3,iStr);
            is_id_printed = 0;
        }
    }
    // print all.
    printf("%s\n",l1); // +---------+------------+  ...
    printf("%s\n",l2); // |P1       |P2          |  ...
    printf("%s\n",l1); // +---------+------------+  ...
    printf("%s\n",l3); // 0         10           20 ...
    printf("\n");
}

int main(void){
    char gantt_dir[] = "./data/gantt";
    DIR *d;
    struct dirent *dir;
    d = opendir(gantt_dir);
    if (d){
        char * filename = malloc(sizeof(char) * 100);
        while ((dir = readdir(d)) != NULL) // check all file in current directory
        {
            // except '.' and '..'
            if (strcmp(dir->d_name,".") != 0 && strcmp(dir->d_name,"..") != 0)
            {   
                sprintf(filename, "%s/",gantt_dir);
                strcat(filename, dir->d_name);

                printf("Gantt Chart of \"%s\"\n", dir->d_name);
                // print gantt chart
                parse_input(filename);
                print_gantt();
            }
        }
        free(filename);
    }
    closedir(d);
    free(pids);
    
    return 0;
}

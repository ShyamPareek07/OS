#include <stdio.h>
#include <stdlib.h>

struct process {
    int arrival_time;
    int completion_time;
    int burst_time;
    int pid;
    int waiting_time;
    int turn_around_time;
};

int main() {

    int total_proc = 5;  // Number of processes
    struct process* schedule = (struct process*)malloc(sizeof(struct process) * total_proc);

    // Dynamically initializing each process using malloc
    if (schedule == NULL) {
        printf("Memory allocation failed\n");
        return 1;  // Exit if memory allocation fails
    }

    // Hardcoded process details (dynamically assigned)
    schedule[0].pid = 3; schedule[0].arrival_time = 0; schedule[0].burst_time = 4;
    schedule[1].pid = 4; schedule[1].arrival_time = 0; schedule[1].burst_time = 7;
    schedule[2].pid = 1; schedule[2].arrival_time = 2; schedule[2].burst_time = 2;
    schedule[3].pid = 2; schedule[3].arrival_time = 5; schedule[3].burst_time = 6;
    schedule[4].pid = 5; schedule[4].arrival_time = 7; schedule[4].burst_time = 4;

    // FCFS scheduling calculation
    for(int i = 0; i < total_proc; i++) {
        if(i > 0) {
            int time_pass_till_now = schedule[i-1].waiting_time + schedule[i-1].burst_time + schedule[i-1].arrival_time;
            schedule[i].waiting_time = time_pass_till_now - schedule[i].arrival_time;
            schedule[i].completion_time = time_pass_till_now + schedule[i].burst_time;
            schedule[i].turn_around_time = schedule[i].completion_time - schedule[i].arrival_time;
        }
        else {
            schedule[i].completion_time = schedule[i].burst_time;
            schedule[i].turn_around_time = schedule[i].completion_time - schedule[i].arrival_time;
            schedule[i].waiting_time = 0;  // First process has no waiting time
        }
    }

    printf("Execution sequence:\n\n");
    float avg_waiting_time = 0;  
    float avg_turn_around = 0;   

    for(int i = 0; i < total_proc; i++) {
        printf("PID:%d ,", schedule[i].pid);
        printf("arrival time:%d ,", schedule[i].arrival_time);
        printf("burst time:%d ,", schedule[i].burst_time);
        printf("waiting time:%d ,", schedule[i].waiting_time);
        printf("TAT:%d ,", schedule[i].turn_around_time);
        printf("completion time:%d\n\n", schedule[i].completion_time);

        avg_waiting_time += schedule[i].waiting_time;
        avg_turn_around += schedule[i].turn_around_time;
    }

    avg_turn_around = avg_turn_around / total_proc;
    avg_waiting_time = avg_waiting_time / total_proc;

    printf("AVERAGE WAITING TIME: %f\n", avg_waiting_time);
    printf("AVERAGE TAT: %f\n", avg_turn_around);

    free(schedule);  // Free dynamically allocated memory
    return 0;
}

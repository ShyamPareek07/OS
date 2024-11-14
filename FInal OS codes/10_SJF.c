#include <stdio.h>

#define MAX_PROCESSES 5

typedef struct {
    int process_id;
    int arrival_time;
    int burst_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
    int remaining_time;
} Process;

void calculate_sjf_non_preemptive(Process processes[], int n) {
    int completed = 0, current_time = 0, min_index;
    int min_burst;
    int is_completed[MAX_PROCESSES] = {0};

    while (completed != n) {
        min_index = -1;
        min_burst = 10000;
        
        for (int i = 0; i < n; i++) {
            if (!is_completed[i] && processes[i].arrival_time <= current_time) {
                if (processes[i].burst_time < min_burst) {
                    min_burst = processes[i].burst_time;
                    min_index = i;
                }
            }
        }
        
        if (min_index != -1) {
            current_time += processes[min_index].burst_time;
            processes[min_index].completion_time = current_time;
            processes[min_index].turnaround_time = processes[min_index].completion_time - processes[min_index].arrival_time;
            processes[min_index].waiting_time = processes[min_index].turnaround_time - processes[min_index].burst_time;
            is_completed[min_index] = 1;
            completed++;
        } else {
            current_time++;
        }
    }
}

void calculate_sjf_preemptive(Process processes[], int n) {
    int completed = 0, current_time = 0, min_index;
    int min_remaining;
    
    while (completed != n) {
        min_index = -1;
        min_remaining = 10000;
        
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= current_time && processes[i].remaining_time > 0) {
                if (processes[i].remaining_time < min_remaining) {
                    min_remaining = processes[i].remaining_time;
                    min_index = i;
                }
            }
        }
        
        if (min_index != -1) {
            current_time++;
            processes[min_index].remaining_time--;

            if (processes[min_index].remaining_time == 0) {
                completed++;
                processes[min_index].completion_time = current_time;
                processes[min_index].turnaround_time = processes[min_index].completion_time - processes[min_index].arrival_time;
                processes[min_index].waiting_time = processes[min_index].turnaround_time - processes[min_index].burst_time;
            }
        } else {
            current_time++;
        }
    }
}

void print_results(Process processes[], int n) {
    printf("PID\tAT\tBT\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\n", 
            processes[i].process_id, 
            processes[i].arrival_time, 
            processes[i].burst_time, 
            processes[i].completion_time, 
            processes[i].turnaround_time, 
            processes[i].waiting_time);
    }
}

int main() {
    Process processes_non_preemptive[MAX_PROCESSES] = {
        {1, 0, 7, 0, 0, 0, 7},
        {2, 2, 4, 0, 0, 0, 4},
        {3, 4, 1, 0, 0, 0, 1},
        {4, 5, 4, 0, 0, 0, 4},
        {5, 6, 3, 0, 0, 0, 3}
    };

    Process processes_preemptive[MAX_PROCESSES] = {
        {1, 0, 7, 0, 0, 0, 7},
        {2, 2, 4, 0, 0, 0, 4},
        {3, 4, 1, 0, 0, 0, 1},
        {4, 5, 4, 0, 0, 0, 4},
        {5, 6, 3, 0, 0, 0, 3}
    };

    int n = MAX_PROCESSES;

    printf("SJF Non-Preemptive:\n");
    calculate_sjf_non_preemptive(processes_non_preemptive, n);
    print_results(processes_non_preemptive, n);

    printf("\nSJF Preemptive:\n");
    calculate_sjf_preemptive(processes_preemptive, n);
    print_results(processes_preemptive, n);

    return 0;
}
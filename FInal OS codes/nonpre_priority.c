#include <stdlib.h>
#include <stdio.h>

struct process {
    int waiting_time;
    int pid;
    int burst_time;
    int completion_time;
    int turnaround_time;
    int arrival_time;
    int priority;
};

void sortprocs(struct process* processes, int total_proc) {
    for (int i = 0; i < total_proc; i++) {
        for (int j = 0; j < total_proc - 1 - i; j++) {
            if (processes[j].arrival_time > processes[j + 1].arrival_time) {
                struct process temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            } else if (processes[j].arrival_time == processes[j + 1].arrival_time) {
                if (processes[j].priority > processes[j + 1].priority) {
                    struct process temp = processes[j];
                    processes[j] = processes[j + 1];
                    processes[j + 1] = temp;
                }
            }
        }
    }
}

int findNextProcess(struct process* processes, int total_proc, int curr_time, int* completed) {
    int idx = -1;
    for (int i = 0; i < total_proc; i++) {
        if (processes[i].arrival_time <= curr_time && completed[i] == 0) {
            if (idx == -1 || processes[i].priority < processes[idx].priority) {
                idx = i;
            }
        }
    }
    return idx;
}

int main() {
    int total_proc = 5;  // Number of processes
    int curr_time = 0;

    // DMA to allocate memory for processes and completed array
    struct process* processes = (struct process*)malloc(total_proc * sizeof(struct process));
    int* completed = (int*)calloc(total_proc, sizeof(int));

    if (processes == NULL || completed == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    // Hardcoded process details using DMA
    processes[0].pid = 1; processes[0].arrival_time = 0; processes[0].burst_time = 10; processes[0].priority = 3;
    processes[1].pid = 2; processes[1].arrival_time = 0; processes[1].burst_time = 1;  processes[1].priority = 1;
    processes[2].pid = 3; processes[2].arrival_time = 0; processes[2].burst_time = 2;  processes[2].priority = 4;
    processes[3].pid = 4; processes[3].arrival_time = 0; processes[3].burst_time = 1;  processes[3].priority = 5;
    processes[4].pid = 5; processes[4].arrival_time = 0; processes[4].burst_time = 5;  processes[4].priority = 2;

    sortprocs(processes, total_proc);

    int completed_count = 0;
    while (completed_count < total_proc) {
        int next_pidx = findNextProcess(processes, total_proc, curr_time, completed);

        if (next_pidx != -1) {
            // Process found, execute it
            curr_time += processes[next_pidx].burst_time;
            processes[next_pidx].completion_time = curr_time;
            processes[next_pidx].turnaround_time = processes[next_pidx].completion_time - processes[next_pidx].arrival_time;
            processes[next_pidx].waiting_time = processes[next_pidx].turnaround_time - processes[next_pidx].burst_time;
            completed[next_pidx] = 1;
            completed_count++;
        } else {
            // No process is ready to execute, increment time
            curr_time++;
        }
    }

    printf("\nPID\tArrivalTime\tBurstTime\tCompletionTime\tTAT\tWaitingTime\n");
    for (int i = 0; i < total_proc; i++) {
        struct process p = processes[i];
        printf("%d\t%d\t\t%d\t\t%d\t\t%d\t%d\n", 
            p.pid, p.arrival_time, p.burst_time, p.completion_time, p.turnaround_time, p.waiting_time);
    }

    // Free dynamically allocated memory
    free(processes);
    free(completed);
    return 0;
}

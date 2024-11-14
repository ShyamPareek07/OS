#include <stdio.h>

#define MAX_PROCESSES 5
#define TIME_QUANTUM 2

typedef struct {
    int process_id;
    int arrival_time;
    int burst_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
    int remaining_time;
} Process;

void calculate_round_robin(Process processes[], int n, int time_quantum) {
    int current_time = 0;
    int completed = 0;
    int queue[MAX_PROCESSES];
    int front = 0, rear = 0;
    
    // Initialize the queue with processes that have arrived at time 0
    for (int i = 0; i < n; i++) {
        if (processes[i].arrival_time <= current_time) {
            queue[rear++] = i;
        }
    }

    while (completed < n) {
        if (front != rear) {
            int index = queue[front];
            front = (front + 1) % MAX_PROCESSES;

            if (processes[index].remaining_time > time_quantum) {
                current_time += time_quantum;
                processes[index].remaining_time -= time_quantum;
            } else {
                current_time += processes[index].remaining_time;
                processes[index].completion_time = current_time;
                processes[index].turnaround_time = processes[index].completion_time - processes[index].arrival_time;
                processes[index].waiting_time = processes[index].turnaround_time - processes[index].burst_time;
                processes[index].remaining_time = 0;
                completed++;
            }

            for (int i = 0; i < n; i++) {
                if (i != index && processes[i].arrival_time <= current_time && processes[i].remaining_time > 0) {
                    int found = 0;
                    for (int j = front; j != rear; j = (j + 1) % MAX_PROCESSES) {
                        if (queue[j] == i) {
                            found = 1;
                            break;
                        }
                    }
                    if (!found) {
                        queue[rear] = i;
                        rear = (rear + 1) % MAX_PROCESSES;
                    }
                }
            }

            if (processes[index].remaining_time > 0) {
                queue[rear] = index;
                rear = (rear + 1) % MAX_PROCESSES;
            }
        } else {
            current_time++;
            for (int i = 0; i < n; i++) {
                if (processes[i].arrival_time <= current_time && processes[i].remaining_time > 0) {
                    queue[rear++] = i;
                    break;
                }
            }
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
    Process processes[MAX_PROCESSES] = {
        {1, 0, 5, 0, 0, 0, 5},
        {2, 1, 4, 0, 0, 0, 4},
        {3, 2, 2, 0, 0, 0, 2},
        {4, 3, 1, 0, 0, 0, 1},
        {5, 4, 6, 0, 0, 0, 6}
    };

    int n = MAX_PROCESSES;

    calculate_round_robin(processes, n, TIME_QUANTUM);
    print_results(processes, n);

    return 0;
}
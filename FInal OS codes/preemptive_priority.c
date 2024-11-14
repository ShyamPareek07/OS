#include <stdio.h>
#include <stdlib.h>

struct process {
    int waiting_time;
    int pid;
    int burst_time;
    int completion_time;
    int turnaround_time;
    int remaining_time;
    int arrival_time;
    int priority;
};

struct queue {
    int* schedule;
    int size;
    int front;
    int rear;
};

void enqueue(int pidx, struct queue* q) {
    if (q->rear == q->size) {
        return;
    }
    if (q->front == -1) {
        q->front = 0;
        q->rear = 0;
    }
    q->schedule[q->rear++] = pidx;
}

int isEmpty(struct queue* q) {
    return (q->front == -1 || q->front == q->rear);
}

int dequeue(struct queue* q) {
    if (isEmpty(q)) {
        return -1;
    }
    int polled = q->schedule[q->front++];

    if (q->front == q->rear) {
        q->front = -1;
        q->rear = -1;
    }
    return polled;
}

struct queue* initReadyQueue() {
    struct queue* q = (struct queue*)malloc(sizeof(struct queue));
    q->schedule = (int*)calloc(100, sizeof(int));
    q->size = 100;
    q->front = -1;
    q->rear = -1;
    return q;
}

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

int getNextPidx(struct queue* q, int currtime, struct process* processes) {
    if (isEmpty(q)) return -1;

    int finalIdx = -1;
    int queue_idx = -1;
    for (int i = q->front; i < q->rear; i++) {
        int process_idx = q->schedule[i];
        if (processes[process_idx].arrival_time <= currtime && processes[process_idx].remaining_time != 0) {
            if (finalIdx == -1 || processes[process_idx].priority < processes[finalIdx].priority) {
                finalIdx = process_idx;
                queue_idx = i;
            }
        }
    }
    if (queue_idx != -1) {
        int temp = q->schedule[q->front];
        q->schedule[q->front] = q->schedule[queue_idx];
        q->schedule[queue_idx] = temp;
    }

    return finalIdx;
}

int main() {
    int total_proc = 3;  // Number of processes
    int curr_time = 0;

    // DMA for processes array
    struct process* processes = (struct process*)calloc(total_proc, sizeof(struct process));
    int* completed = (int*)calloc(total_proc, sizeof(int));

    // Check for memory allocation errors
    if (processes == NULL || completed == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    // Hardcoded process details
    processes[0] = (struct process){.pid = 1, .arrival_time = 0, .burst_time = 5, .priority = 3, .remaining_time = 5};
    processes[1] = (struct process){.pid = 2, .arrival_time = 1, .burst_time = 5, .priority = 2, .remaining_time = 5};
    processes[2] = (struct process){.pid = 3, .arrival_time = 2, .burst_time = 5, .priority = 1, .remaining_time = 5};

    // Sort processes by arrival time and priority
    sortprocs(processes, total_proc);

    int track = 0;
    int done = 0;
    struct queue* q = initReadyQueue();

    while (done < total_proc) {
        while (track < total_proc && curr_time >= processes[track].arrival_time) {
            enqueue(track++, q);
        }

        int nextpidx = getNextPidx(q, curr_time, processes);
        if (nextpidx == -1) {
            curr_time++;
            continue;
        } else {
            int currpidx = dequeue(q);
            struct process* currproc = &processes[currpidx];
            currproc->remaining_time--;
            curr_time++;
            if (currproc->remaining_time == 0) {
                currproc->completion_time = curr_time;
                currproc->turnaround_time = currproc->completion_time - currproc->arrival_time;
                currproc->waiting_time = currproc->turnaround_time - currproc->burst_time;
                done++;
            } else {
                enqueue(currpidx, q);
            }
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
    free(q->schedule);
    free(q);
    
    return 0;
}

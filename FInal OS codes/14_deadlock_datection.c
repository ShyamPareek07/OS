#include <stdio.h>

#define MAX_PROCESSES 5
#define MAX_RESOURCES 3

int available[MAX_RESOURCES];
int allocation[MAX_PROCESSES][MAX_RESOURCES];
int max[MAX_PROCESSES][MAX_RESOURCES];
int need[MAX_PROCESSES][MAX_RESOURCES];
int finish[MAX_PROCESSES];

void calculate_need(int n, int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }
}

int is_deadlocked(int n, int m) {
    int work[MAX_RESOURCES];
    for (int i = 0; i < m; i++) {
        work[i] = available[i];
    }

    for (int i = 0; i < n; i++) {
        finish[i] = 0;
    }

    int deadlock = 0;
    while (1) {
        int found_process = 0;
        for (int i = 0; i < n; i++) {
            if (finish[i] == 0) {
                int can_allocate = 1;
                for (int j = 0; j < m; j++) {
                    if (need[i][j] > work[j]) {
                        can_allocate = 0;
                        break;
                    }
                }
                if (can_allocate) {
                    for (int k = 0; k < m; k++) {
                        work[k] += allocation[i][k];
                    }
                    finish[i] = 1;
                    found_process = 1;
                }
            }
        }
        if (!found_process) {
            break;
        }
    }

    for (int i = 0; i < n; i++) {
        if (finish[i] == 0) {
            deadlock = 1;
            printf("Process %d is in deadlock.\n", i);
        }
    }

    return deadlock;
}

int main() {
    int n = MAX_PROCESSES;
    int m = MAX_RESOURCES;

    available[0] = 3;
    available[1] = 3;
    available[2] = 2;

    int max_matrix[MAX_PROCESSES][MAX_RESOURCES] = {
        {7, 5, 3},
        {3, 2, 2},
        {9, 0, 2},
        {2, 2, 2},
        {4, 3, 3}
    };

    int allocation_matrix[MAX_PROCESSES][MAX_RESOURCES] = {
        {0, 1, 0},
        {2, 0, 0},
        {3, 0, 2},
        {2, 1, 1},
        {0, 0, 2}
    };

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            max[i][j] = max_matrix[i][j];
            allocation[i][j] = allocation_matrix[i][j];
        }
    }

    calculate_need(n, m);

    if (is_deadlocked(n, m)) {
        printf("System is in a deadlock state.\n");
    } else {
        printf("System is not in a deadlock state.\n");
    }

    return 0;
}
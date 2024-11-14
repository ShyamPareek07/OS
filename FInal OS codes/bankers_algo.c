#include <stdio.h>
#include <stdbool.h>

bool is_safe(int n_proc, int n_res, int available[], int allocation[][n_res], int need[][n_res]) {
    int work[n_res];
    bool finish[n_proc];
    int safe_sequence[n_proc];
    int count = 0; // tracks number of finished processes

    for (int i = 0; i < n_res; i++) {
        work[i] = available[i];
    }
    for (int i = 0; i < n_proc; i++) {
        finish[i] = false;
    }

    while (count < n_proc) {
        bool safe = false;

        for (int i = 0; i < n_proc; i++) {
            if (!finish[i]) {
                bool can_allocate = true;

                for (int j = 0; j < n_res; j++) {
                    if (need[i][j] > work[j]) {
                        can_allocate = false;
                        break;
                    }
                }

                if (can_allocate) {
                    // simulate process completion
                    for (int j = 0; j < n_res; j++) {
                        work[j] += allocation[i][j];
                    }
                    finish[i] = true;
                    safe_sequence[count++] = i;
                    safe = true;
                }
            }
        }

        // no process could execute, system is not in a safe state
        if (!safe) {
            return false;
        }
    }

    // Print the safe sequence
    printf("\nSystem is in a safe state.\nSafe sequence is: ");
    for (int i = 0; i < n_proc; i++) {
        printf("P%d ", safe_sequence[i]);
    }
    printf("\n");

    return true;
}

int main() {
    int n_proc = 5;  // Number of processes
    int n_res = 3;   // Number of resource types

    // Safe state example
    int available_safe[] = {3, 3, 2};
    int max_need_safe[5][3] = {
        {7, 5, 3},
        {3, 2, 2},
        {9, 0, 2},
        {2, 2, 2},
        {4, 3, 3}
    };
    int allocation_safe[5][3] = {
        {0, 1, 0},
        {2, 0, 0},
        {3, 0, 2},
        {2, 1, 1},
        {0, 0, 2}
    };
    int need_safe[5][3];

    printf("Running Safe State Example:\n");

    // Calculate the need matrix for safe example
    for (int i = 0; i < n_proc; i++) {
        for (int j = 0; j < n_res; j++) {
            need_safe[i][j] = max_need_safe[i][j] - allocation_safe[i][j];
        }
    }

    if (!is_safe(n_proc, n_res, available_safe, allocation_safe, need_safe)) {
        printf("\nSystem is not in a safe state.\n");
    }
    // Unsafe state example
    int available_unsafe[] = {1, 0, 0};
    int max_need_unsafe[5][3] = {
        {0, 0, 1},
        {2, 7, 5},
        {3, 6, 5},
        {2, 3, 5},
        {2, 3, 5}
    };
    int allocation_unsafe[5][3] = {
        {0, 0, 1},
        {2, 0, 0},
        {3, 0, 3},
        {2, 1, 1},
        {0, 0, 2}
    };
    int need_unsafe[5][3];


    for (int i = 0; i < n_proc; i++) {
        for (int j = 0; j < n_res; j++) {
            need_unsafe[i][j] = max_need_unsafe[i][j] - allocation_unsafe[i][j];
        }
    }

    if (!is_safe(n_proc, n_res, available_unsafe, allocation_unsafe, need_unsafe)) {
        printf("\nSystem is not in a safe state.\n");
    }

    return 0;
}

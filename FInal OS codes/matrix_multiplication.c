#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

struct arg_struct
{
    int **matrix1;
    int **matrix2;
    int rowIndex;
    int m;
    int n;
    int p;
    int **answer;
};

void *calculateFinalRow(void *arg)
{
    struct arg_struct *a = (struct arg_struct *)arg; // typecasting the void pointer
    for (int i = 0; i < a->p; i++)
    { // Loop should run for columns in result matrix
        a->answer[a->rowIndex][i] = 0;
        for (int j = 0; j < a->n; j++)
        { // Corrected to use a->n
            a->answer[a->rowIndex][i] += a->matrix1[a->rowIndex][j] * a->matrix2[j][i];
        }
    }
    return NULL;
}

int main()
{
    int m = 3; // rows of m1
    int n = 3; // cols of m1, rows of m2
    int p = 3; // cols of m2

    // Allocate and initialize matrix1
    int **matrix1 = (int **)malloc(sizeof(int *) * m);
    for (int i = 0; i < m; i++)
    {
        matrix1[i] = (int *)malloc(sizeof(int) * n);
    }
    matrix1[0][0] = 2;
    matrix1[0][1] = 3;
    matrix1[0][2] = 4;
    matrix1[1][0] = 3;
    matrix1[1][1] = 5;
    matrix1[1][2] = 6;
    matrix1[2][0] = 4;
    matrix1[2][1] = 5;
    matrix1[2][2] = 3;

    // Allocate and initialize matrix2
    int **matrix2 = (int **)malloc(sizeof(int *) * n);
    for (int i = 0; i < n; i++)
    {
        matrix2[i] = (int *)malloc(sizeof(int) * p);
    }
    matrix2[0][0] = 1;
    matrix2[0][1] = 2;
    matrix2[0][2] = 1;
    matrix2[1][0] = -1;
    matrix2[1][1] = 2;
    matrix2[1][2] = 1;
    matrix2[2][0] = 3;
    matrix2[2][1] = 2;
    matrix2[2][2] = 1;

    // Allocate the result matrix
    int **ans = (int **)malloc(sizeof(int *) * m);
    for (int i = 0; i < m; i++)
    {
        ans[i] = (int *)malloc(sizeof(int) * p);
    }

    // Create threads to calculate each row
    for (int i = 0; i < m; i++)
    {
        pthread_t thread;
        struct arg_struct *a = (struct arg_struct *)malloc(sizeof(struct arg_struct));
        a->matrix1 = matrix1;
        a->matrix2 = matrix2;
        a->answer = ans;
        a->m = m;
        a->n = n;
        a->p = p;
        a->rowIndex = i;

        pthread_create(&thread, NULL, calculateFinalRow, a);
        pthread_join(thread, NULL);

        free(a); // Free the argument struct after the thread is done
    }


    printf("Matrix1 :\n");
    for(int i = 0 ; i < m; i++) {
        for(int j = 0 ; j < n; j++) {
            printf("%d ", matrix1[i][j]);
        }
        printf("\n");
    }

    printf("Matrix2 :\n");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < p; j++)
        {
            printf("%d ", matrix2[i][j]);
        }
        printf("\n");
    }

    // Print the result matrix
    printf(":\n");
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < p; j++)
        {
            printf("%d ", ans[i][j]);
        }
        printf("\n");
    }

    // Free allocated memory
    for (int i = 0; i < m; i++)
        free(matrix1[i]);
    free(matrix1);

    for (int i = 0; i < n; i++)
        free(matrix2[i]);
    free(matrix2);

    for (int i = 0; i < m; i++)
        free(ans[i]);
    free(ans);

    return 0;
}

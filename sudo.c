#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 9

int sudoku[SIZE][SIZE] = {
    {5,3,4,6,7,8,9,1,2},
    {6,7,2,1,9,5,3,4,8},
    {1,9,8,3,4,2,5,6,7},
    {8,5,9,7,6,1,4,2,3},
    {4,2,6,8,5,3,7,9,1},
    {7,1,3,9,2,4,8,5,6},
    {9,6,1,5,3,7,2,8,4},
    {2,8,7,4,1,9,6,3,5},
    {3,4,5,2,8,6,1,7,9}
};


int result[3] = {0, 0, 0};

void* check_rows(void* arg) {
    for (int i = 0; i < SIZE; i++) {
        int seen[SIZE + 1] = {0};
        for (int j = 0; j < SIZE; j++) {
            int num = sudoku[i][j];
            if (num < 1 || num > 9 || seen[num]) {
                pthread_exit(NULL);
            }
            seen[num] = 1;
        }
    }
    result[0] = 1;
    pthread_exit(NULL);
}

void* check_cols(void* arg) {
    for (int j = 0; j < SIZE; j++) {
        int seen[SIZE + 1] = {0};
        for (int i = 0; i < SIZE; i++) {
            int num = sudoku[i][j];
            if (num < 1 || num > 9 || seen[num]) {
                pthread_exit(NULL);
            }
            seen[num] = 1;
        }
    }
    result[1] = 1;
    pthread_exit(NULL);
}

void* check_boxes(void* arg) {
    for (int row = 0; row < SIZE; row += 3) {
        for (int col = 0; col < SIZE; col += 3) {
            int seen[SIZE + 1] = {0};
            for (int i = row; i < row + 3; i++) {
                for (int j = col; j < col + 3; j++) {
                    int num = sudoku[i][j];
                    if (num < 1 || num > 9 || seen[num]) {
                        pthread_exit(NULL);
                    }
                    seen[num] = 1;
                }
            }
        }
    }
    result[2] = 1;
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[3];


    pthread_create(&threads[0], NULL, check_rows, NULL);
    pthread_create(&threads[1], NULL, check_cols, NULL);
    pthread_create(&threads[2], NULL, check_boxes, NULL);

    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    if (result[0] && result[1] && result[2]) {
        printf("it's ok\n");
    } else {
        printf("it's not OK\n");
    }

    return 0;
}

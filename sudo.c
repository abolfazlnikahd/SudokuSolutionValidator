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

int row_valid = 0, col_valid = 0;
int box_valid[9] = {0};

void* check_row(void* arg) {
    for (int i = 0; i < SIZE; i++) {
        int seen[10] = {0};
        for (int j = 0; j < SIZE; j++) {
            int num = sudoku[i][j];
            if (num < 1 || num > 9 || seen[num]) pthread_exit(NULL);
            seen[num] = 1;
        }
    }
    row_valid = 1;
    pthread_exit(NULL);
}

void* check_col(void* arg) {
    for (int j = 0; j < SIZE; j++) {
        int seen[10] = {0};
        for (int i = 0; i < SIZE; i++) {
            int num = sudoku[i][j];
            if (num < 1 || num > 9 || seen[num]) pthread_exit(NULL);
            seen[num] = 1;
        }
    }
    col_valid = 1;
    pthread_exit(NULL);
}

typedef struct {
    int row;
    int col;
    int index;
} BoxPos;

void* check_box(void* arg) {
    BoxPos* pos = (BoxPos*) arg;
    int seen[10] = {0};
    for (int i = pos->row; i < pos->row + 3; i++) {
        for (int j = pos->col; j < pos->col + 3; j++) {
            int num = sudoku[i][j];
            if (num < 1 || num > 9 || seen[num]) pthread_exit(NULL);
            seen[num] = 1;
        }
    }
    box_valid[pos->index] = 1;
    free(pos);
    pthread_exit(NULL);
}

int main() {
    pthread_t row_thread, col_thread, box_threads[9];

    pthread_create(&row_thread, NULL, check_row, NULL);
    pthread_create(&col_thread, NULL, check_col, NULL);

    int index = 0;
    for (int i = 0; i < SIZE; i += 3) {
        for (int j = 0; j < SIZE; j += 3) {
            BoxPos* pos = (BoxPos*) malloc(sizeof(BoxPos));
            pos->row = i;
            pos->col = j;
            pos->index = index;
            pthread_create(&box_threads[index], NULL, check_box, (void*) pos);
            index++;
        }
    }

    pthread_join(row_thread, NULL);
    pthread_join(col_thread, NULL);
    for (int i = 0; i < 9; i++) {
        pthread_join(box_threads[i], NULL);
    }

    int all_boxes_valid = 1;
    for (int i = 0; i < 9; i++) {
        if (box_valid[i] == 0) {
            all_boxes_valid = 0;
            break;
        }
    }

    if (row_valid && col_valid && all_boxes_valid) {
        printf("OK\n");
    } else {
        printf("NOT OK\n");
    }

    return 0;
}

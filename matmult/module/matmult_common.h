#ifndef KOREATECH_MATMULT_COMMON_H
#define KOREATECH_MATMULT_COMMON_H

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define MAX 2048

bool log_enabled = false;

typedef struct {
    int row_start;
    int row_end;
    int col_start;
    int col_end;
} Block;

void print_thread_info(Block *block) {
    if (log_enabled) {
        if (block != NULL) {
            printf("Thread [%ld] 가 연산을 수행합니다. | rows [%d : %d] columns [%d : %d]\n",
                   pthread_self(), block->row_start, block->row_end, block->col_start, block->col_end);
        } else {
            printf("Thread [%ld] 가 연산을 수행합니다. | rows [%d : %d] columns [%d : %d]\n", pthread_self());
        }
    }
}

#endif

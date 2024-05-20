#ifndef KOREATECH_MATMULT_N_THREAD_H
#define KOREATECH_MATMULT_N_THREAD_H

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define MAX 2048

int A[MAX][MAX];    /* multiplier matrix */
int B[MAX][MAX];    /* multiplicand matrix */
int C[MAX][MAX];    /* product matrix */
int N;  // 행렬 크기

bool log_enabled = false; // 로깅여부

typedef struct {
    int start_row;
    int end_row;
} ThreadData;

void *matmult_multi_threaded(void *arg);

void print_thread_info(ThreadData *threadData);

int execute_matmult_by_thread_count(int threadCount, bool is_log) {
    log_enabled = is_log;
    int sizes[4] = {1024, 1500, 2000, 2048};
    struct timespec start, end;
    double threaded_time;
    pthread_t thr[threadCount];
    ThreadData thread_data[threadCount];
    int error;

    for (int size_idx = 0; size_idx < 4; size_idx++) {
        N = sizes[size_idx];

        // 행렬 초기화
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                A[i][j] = i + j;
                B[i][j] = i + j;
                C[i][j] = 0; // 결과 행렬 초기화
            }
        }

        // 멀티 쓰레드 실행 시간 측정
        clock_gettime(CLOCK_MONOTONIC, &start);

        // 각 쓰레드가 처리할 행의 범위 설정
        int rows_per_thread = N / threadCount;
        for (int i = 0; i < threadCount; i++) {
            thread_data[i].start_row = i * rows_per_thread;
            thread_data[i].end_row = (i == threadCount - 1) ? N : (i + 1) * rows_per_thread;
        }

        // 작업 쓰레드 생성
        for (int i = 0; i < threadCount; i++) {
            if ((error = pthread_create(
                    &thr[i],
                    NULL,
                    matmult_multi_threaded,
                    (void *) &thread_data[i]))) {
                fprintf(stderr, "pthread_create: %s\n", strerror(error));
                exit(1);
            }
        }

        // 작업 완료 대기
        for (int i = 0; i < threadCount; i++) {
            pthread_join(thr[i], NULL);
        }

        // 종료시간 책정
        clock_gettime(CLOCK_MONOTONIC, &end);
        threaded_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
        printf("[Thread %d개] 행렬곱 수행 시간 (%dx%d): %f 초\n", threadCount, N, N, threaded_time);
    }

    return 0;
}

void *matmult_multi_threaded(void *arg) {
    ThreadData *data = (ThreadData *) arg;
    int start_row = data->start_row;
    int end_row = data->end_row;

    if (log_enabled) {
        print_thread_info(data);
    }

    for (int row = start_row; row < end_row; row++) {
        for (int col = 0; col < N; col++) {
            int t = 0;
            for (int k = 0; k < N; k++) {
                t += A[row][k] * B[k][col];
            }
            C[row][col] = t;
        }
    }
    return NULL;
}

void print_thread_info(ThreadData *threadData) {
    if (log_enabled) {
        if (threadData != NULL) {
            printf("Thread [%ld] 가 연산을 수행합니다. | rows [%d : %d]\n",
                   pthread_self(), threadData->start_row, threadData->end_row);
        } else {
            printf("Thread [%ld] 가 연산을 수행합니다.\n", pthread_self());
        }
    }
}

#endif //KOREATECH_MATMULT_N_THREAD_H
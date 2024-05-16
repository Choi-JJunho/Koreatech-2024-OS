#include "module/matmult_common.h"

int A[MAX][MAX];
int B[MAX][MAX];
int C[MAX][MAX];

int N;

void *matmult(void *arg);

int execute_matmult_single_thread(bool is_log) {
    log_enabled = is_log;
    printf("[Single Thread 행렬곱 수행]\n");
    int i, j;
    pthread_t thr;
    int error;

    // 행렬 크기
    int sizes[4] = {1024, 1500, 2000, 2048};
    struct timespec start, end;
    double threaded_time;

    for (int size_idx = 0; size_idx < 4; size_idx++) {
        N = sizes[size_idx];

        // 행렬 초기화
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                A[i][j] = i + j;
                B[i][j] = i + j;
            }
        }

        // 멀티 쓰레드 실행 시간 측정
        clock_gettime(CLOCK_MONOTONIC, &start);

        // 블록 크기
        int block_size = N / 4;
        Block blocks[16];
        int block_index = 0;

        for (int row_block = 0; row_block < 4; row_block++) {
            for (int col_block = 0; col_block < 4; col_block++) {
                blocks[block_index].row_start = row_block * block_size;
                blocks[block_index].row_end = (row_block + 1) * block_size;
                blocks[block_index].col_start = col_block * block_size;
                blocks[block_index].col_end = (col_block + 1) * block_size;
                block_index++;
            }
        }

        // 작업 쓰레드 생성 | single thread
        if ((error = pthread_create(
                &thr,
                NULL,
                matmult,
                (void *) blocks))) {
            fprintf(stderr, "pthread_create: %s\n", strerror(error));
            exit(1);
        }

        // 작업 완료 대기
        pthread_join(thr, NULL);

        // 종료시간 책정
        clock_gettime(CLOCK_MONOTONIC, &end);
        threaded_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
        printf("[Single Thread] 행렬곱 수행 시간 (%dx%d): %f 초\n", N, N, threaded_time);
    }

    return 0;
}

void *matmult(void *arg) {
    Block *blocks = (Block *) arg;

    for (int i = 0; i < 16; i++) {
        int row_start = blocks[i].row_start;
        int row_end = blocks[i].row_end;
        int col_start = blocks[i].col_start;
        int col_end = blocks[i].col_end;

        print_thread_info(NULL);
        for (int row = row_start; row < row_end; row++) {
            for (int col = col_start; col < col_end; col++) {
                int t = 0;
                for (int k = 0; k < N; k++) {
                    t += A[row][k] * B[k][col];
                }
                C[row][col] = t;
            }
        }
    }
    return NULL;
}

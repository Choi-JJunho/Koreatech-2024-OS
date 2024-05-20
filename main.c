#include "matmult_n_thread.h"

int main() {

    /*TODO
     * 1. 쓰레드 16개(4x4블록)를 사용해서 A, B, C 세 개의 행렬이 모두 NxN 행렬이라고 가정하고
     * 1024x1024, 1500x1500, 2000x2000, 2048x2048의 행렬 곱셉을 수행하는 프로그램을 작성하시오.
     */
    execute_matmult_by_thread_count(16, false);

    /*TODO
     * 2. 붙임 2의 알고리즘을 약간 변형하여 쓰레드의 수를 16개로 고정하고 쓰레드당 여러 개의 행을 처리하도록 프로그램을 수정할 수 있다.
     * 예로 1024x1024행렬의 경우 하나의 쓰레드 당 64개의 행을 처리하면 된다.
     * 이 프로그램을 이용하여 1024x1024, 1500x1500, 2000x2000, 2048x2048의 행렬 곱셉을 수행하는 프로그램을 작성하시오.
     */
    // Multi Thread
    execute_matmult_by_thread_count(16, false);

    // Single Thread
    execute_matmult_by_thread_count(1, false);
}
#include "fmm.h"
#include <malloc.h>

void fmm(int n, int* m1, int* m2, int* result) {

    int* A11 = (int *)malloc(n * n / 4 * sizeof(int));

    for (int i = 0; i < n/2; ++i) {
        for (int j = 0; j < n/2; ++j) {
            A11[i*(n/2) + j] = m1[i*n + j];
        }
    }

    int* A12 = (int *)malloc(n * n / 4 * sizeof(int));

    for (int i = 0; i < n/2; ++i) {
        for (int j = 0; j < n/2; ++j) {
            A12[i*(n/2) + j] = m1[i*n+ (n/2) + j];
        }
    }

    int* A21 = (int *)malloc(n * n / 4 * sizeof(int));

    int sqN = n*n;

    for (int i = 0; i < n/2; ++i) {
        for (int j = 0; j < n/2; ++j) {
            A21[i*(n/2) + j] = m1[i*n+ sqN/2 + j];
        }
    }

    int* A22 = (int *)malloc(n * n / 4 * sizeof(int));

    for (int i = 0; i < n/2; ++i) {
        for (int j = 0; j < n/2; ++j) {
            A22[i*(n/2) + j] = m1[i*n+ sqN/2 + n/2 + j];
        }
    }

    ////////////////////////////////////////////////////////

    int* B11 = (int *)malloc(n * n / 4 * sizeof(int));

    for (int i = 0; i < n/2; ++i) {
        for (int j = 0; j < n/2; ++j) {
            B11[i*(n/2) + j] = m2[i*n + j];
        }
    }

    int* B12 = (int *)malloc(n * n / 4 * sizeof(int));

    for (int i = 0; i < n/2; ++i) {
        for (int j = 0; j < n/2; ++j) {
            B12[i*(n/2) + j] = m2[i*n+ (n/2) + j];
        }
    }

    int* B21 = (int *)malloc(n * n / 4 * sizeof(int));

    for (int i = 0; i < n/2; ++i) {
        for (int j = 0; j < n/2; ++j) {
            B21[i*(n/2) + j] = m2[i*n+ sqN/2 + j];
        }
    }

    int* B22 = (int *)malloc(n * n / 4 * sizeof(int));

    for (int i = 0; i < n/2; ++i) {
        for (int j = 0; j < n/2; ++j) {
            B22[i*(n/2) + j] = m2[i*n+ sqN/2 + n/2 + j];
        }
    }

    //M1 = (A11 + A22) X (B11 + B22)


    int* resLeft = (int *)malloc(n * n / 4 * sizeof(int));;
    int* resRight = (int *)malloc(n * n / 4 * sizeof(int));;

    int* M1 = (int *)malloc(n * n / 4 * sizeof(int));;

    for (int i = 0; i < n/2; ++i) {
        for (int j = 0; j < n/2; ++j) {
            resLeft[i * n/2 + j] = A11[i * n/2 + j] + A22[i * n/2 + j];
        }
    }


    for (int i = 0; i < n/2; ++i) {
        for (int j = 0; j < n/2; ++j) {
            resRight[i * n/2 + j] = B11[i * n/2 + j] + B22[i * n/2 + j];
        }
    }

    for (int i = 0; i < n/2; i++) {
        for (int k = 0; k < n/2; k++) {
            M1[i * n/2 + k] = 0;  // Initialize M1[i][k] to zero
        }
        for (int k = 0; k < n/2; k++) {
            for (int j = 0; j < n/2; j++) {
                M1[i * n/2 + j] += resLeft[i * n/2 + k] * resRight[k * n/2 + j];
                // Multiply and accumulate the result
            }
        }
    }

    //M2 = (A21 + A22) X (B11)

    int* M2 = (int *)malloc(n * n / 4 * sizeof(int));;

    for (int i = 0; i < n/2; ++i) {
        for (int j = 0; j < n/2; ++j) {
            resLeft[i * n/2 + j] = A21[i * n/2 + j] + A22[i * n/2 + j];
        }
    }

    for (int i = 0; i < n/2; i++) {
        for (int j = 0; j < n/2; j++) {
            M2[i * n/2 + j] = 0;  // Initialize result[i][j] to zero
        }
        for (int k = 0; k < n/2; k++) {
            for (int j = 0; j < n/2; j++) {
                M2[i * n/2 + j] += resLeft[i * n/2 + k] * B11[k * n/2 + j];
                // Multiply and accumulate the result
            }
        }
    }


    //M3 = (A11) X (B12 - B22)

    int* M3 = (int *)malloc(n * n / 4 * sizeof(int));;

    for (int i = 0; i < n/2; ++i) {
        for (int j = 0; j < n/2; ++j) {
            resRight[i * n/2 + j] = B12[i * n/2 + j] - B22[i * n/2 + j];
        }
    }

    for (int i = 0; i < n/2; i++) {
        for (int j = 0; j < n/2; j++) {
            M3[i * n/2 + j] = 0;  // Initialize result[i][j] to zero
        }
        for (int k = 0; k < n/2; k++) {
            for (int j = 0; j < n/2; j++) {
                M3[i * n/2 + j] += A11[i * n/2 + k] * resRight[k * n/2 + j];
                // Multiply and accumulate the result
            }
        }
    }


    //M4 = (A22) X (B21 - B11)

    int* M4 = (int *)malloc(n * n / 4 * sizeof(int));;

    for (int i = 0; i < n/2; ++i) {
        for (int j = 0; j < n/2; ++j) {
            resRight[i * n/2 + j] = B21[i * n/2 + j] - B11[i * n/2 + j];
        }
    }

    for (int i = 0; i < n/2; i++) {
        for (int j = 0; j < n/2; j++) {
            M4[i * n/2 + j] = 0;  // Initialize result[i][j] to zero
        }
        for (int k = 0; k < n/2; k++) {
            for (int j = 0; j < n/2; j++) {
                M4[i * n/2 + j] += A22[i * n/2 + k] * resRight[k * n/2 + j];
                // Multiply and accumulate the result
            }
        }
    }


    //M5 = (A11 + A12) X (B22)

    int* M5 = (int *)malloc(n * n / 4 * sizeof(int));;

    for (int i = 0; i < n/2; ++i) {
        for (int j = 0; j < n/2; ++j) {
            resLeft[i * n/2 + j] = A11[i * n/2 + j] + A12[i * n/2 + j];
        }
    }

    for (int i = 0; i < n/2; i++) {
        for (int j = 0; j < n/2; j++) {
            M5[i * n/2 + j] = 0;  // Initialize result[i][j] to zero
        }
        for (int k = 0; k < n/2; k++) {
            for (int j = 0; j < n/2; j++) {
                M5[i * n/2 + j] += resLeft[i * n/2 + k] * B22[k * n/2 + j];
                // Multiply and accumulate the result
            }
        }
    }


    //M6 = (A21 - A11) X (B11 + B12)

    int* M6 = (int *)malloc(n * n / 4 * sizeof(int));;


    for (int i = 0; i < n/2; ++i) {
        for (int j = 0; j < n/2; ++j) {
            resLeft[i * n/2 + j] = A21[i * n/2 + j] - A11[i * n/2 + j];
        }
    }


    for (int i = 0; i < n/2; ++i) {
        for (int j = 0; j < n/2; ++j) {
            resRight[i * n/2 + j] = B11[i * n/2 + j] + B12[i * n/2 + j];
        }
    }

    for (int i = 0; i < n/2; i++) {
        for (int j = 0; j < n/2; j++) {
            M6[i * n/2 + j] = 0;  // Initialize result[i][j] to zero
        }
        for (int k = 0; k < n/2; k++) {
            for (int j = 0; j < n/2; j++) {
                M6[i * n/2 + j] += resLeft[i * n/2 + k] * resRight[k * n/2 + j];
                // Multiply and accumulate the result
            }
        }
    }



    //M7 = (A12 - A22) X (B21 + B22)

    int* M7 = (int *)malloc(n * n / 4 * sizeof(int));;


    for (int i = 0; i < n/2; ++i) {
        for (int j = 0; j < n/2; ++j) {
            resLeft[i * n/2 + j] = A12[i * n/2 + j] - A22[i * n/2 + j];
        }
    }


    for (int i = 0; i < n/2; ++i) {
        for (int j = 0; j < n/2; ++j) {
            resRight[i * n/2 + j] = B21[i * n/2 + j] + B22[i * n/2 + j];
        }
    }

    for (int i = 0; i < n/2; i++) {
        for (int j = 0; j < n/2; j++) {
            M7[i * n/2 + j] = 0;  // Initialize result[i][j] to zero
        }
        for (int k = 0; k < n/2; k++) {
            for (int j = 0; j < n/2; j++) {
                M7[i * n/2 + j] += resLeft[i * n/2 + k] * resRight[k * n/2 + j];
                // Multiply and accumulate the result
            }
        }
    }



    //C11 = M1+ M4 - M5 + M7

    int* C11 = (int *)malloc(n * n / 4 * sizeof(int));;


    for (int i = 0; i < n/2; ++i) {
        for (int j = 0; j < n/2; ++j) {
            C11[i * n/2 + j] = M1[i * n/2 + j] + M4[i * n/2 + j] - M5[i
                                                                      *
                                                                      n/2 + j] + M7[i * n/2 + j];
        }
    }


    //C12 = M3+M5

    int* C12 = (int *)malloc(n * n / 4 * sizeof(int));;


    for (int i = 0; i < n/2; ++i) {
        for (int j = 0; j < n/2; ++j) {
            C12[i * n/2 + j] = M3[i * n/2 + j] + M5[i * n/2 + j];
        }
    }

    //C21 = M2+ M4

    int* C21 = (int *)malloc(n * n / 4 * sizeof(int));;


    for (int i = 0; i < n/2; ++i) {
        for (int j = 0; j < n/2; ++j) {
            C21[i * n/2 + j] = M2[i * n/2 + j] + M4[i * n/2 + j];
        }
    }

    //C22 = M1-M2+M3+M6

    int* C22 = (int *)malloc(n * n / 4 * sizeof(int));;


    for (int i = 0; i < n/2; ++i) {
        for (int j = 0; j < n/2; ++j) {
            C22[i * n/2 + j] = M1[i * n/2 + j] - M2[i * n/2 + j] + M3[i
                                                                      *
                                                                      n/2 + j] + M6[i * n/2 + j];
        }
    }

    //func that rebuild result from the Cij blocks

    for (int i = 0; i < n/2; ++i) {
        for (int j = 0; j < n/2; ++j) {
            result[i * n + j] = C11[i * n/2 + j];
        }
    }

    for (int i = 0; i < n/2; ++i) {
        for (int j = 0; j < n/2; ++j) {
            result[i * n + n/2 + j] = C12[i * n/2 + j];
        }
    }

    for (int i = 0; i < n/2; ++i) {
        for (int j = 0; j < n/2; ++j) {
            result[i * n + n*n/2 + j] = C21[i * n/2 + j];
        }
    }

    for (int i = 0; i < n/2; ++i) {
        for (int j = 0; j < n/2; ++j) {
            result[i * n + n*n/2 + n/2+ j] = C22[i * n/2 + j];
        }
    }

    free(A11);
    free(A12);
    free(A21);
    free(A22);
    free(B11);
    free(B12);
    free(B21);
    free(B22);
    free(C11);
    free(C12);
    free(C21);
    free(C22);
    free(resLeft);
    free(resRight);
    free(M1);
    free(M2);
    free(M3);
    free(M4);
    free(M5);
    free(M6);
    free(M7);
    return;
}


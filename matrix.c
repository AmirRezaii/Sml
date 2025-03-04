#include "matrix.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


static int randomize_helper(FILE *in)
{
     unsigned int  seed;

    if (!in)
         return -1;

    if (fread(&seed, sizeof seed, 1, in) == 1) {
        fclose(in);
        srand(seed);
    return 0;
    }

    fclose(in);
    return -1;
}
static int randomize(void)
{
    if (!randomize_helper(fopen("/dev/urandom", "r")))
         return 0;
    if (!randomize_helper(fopen("/dev/arandom", "r")))
         return 0;
    if (!randomize_helper(fopen("/dev/random", "r")))
         return 0;

/* Other randomness sources (binary format)? */

/* No randomness sources found. */
    return -1;
}

static double randm() {
    randomize();
    return (double)rand() / (double)RAND_MAX;
}

void initMatrix(Matrix *matrix, size_t rows, size_t cols) {
    int num_of_cells = rows * cols;
    if (num_of_cells == 0) {
        fprintf(stderr, "ERROR: Cannot initialize 0 dimensional matrix.");
        exit(1);
    }

    matrix->cells = (Value *)malloc(sizeof(Value) * num_of_cells);
    memset(matrix->cells, 0, sizeof(Value) * num_of_cells);
    matrix->rows = rows;
    matrix->cols = cols;
}

void assignMatrix(Matrix matrix, Value *values) {
    for (size_t i = 0; i < matrix.rows; ++i) {
        for (size_t j = 0; j < matrix.cols; ++j) {
            size_t loc = i*matrix.cols + j;
            MATRIX_AT(matrix, i+1, j+1) = values[loc];
        }
    }
}

Value *matrixAt(Matrix matrix, size_t row, size_t col) {
    if (row > matrix.rows || col > matrix.cols || row < 1 || col < 1) {
        fprintf(stderr, "ERROR: Out of bound access at (%ld, %ld)", row, col);
        exit(1);
    }

    size_t dest = (row - 1) * matrix.cols + col - 1;

    return &matrix.cells[dest];
}

void copyMatrix(Matrix *dest, Matrix source) {
    initMatrix(dest, source.rows, source.cols);

    for (size_t i = 0; i < source.rows; ++i) {
        for (size_t j = 0; j < source.cols; ++j) {
            MATRIX_AT(*dest, i+1, j+1) = MATRIX_AT(source, i+1, j+1);
        }
    }
}

void dotMatrix(Matrix *dest, Matrix first, Matrix second) {
    if (first.cols != second.rows) {
        fprintf(stderr, "ERROR: invalid dimensions for matrix multiplication.");
        exit(1);
    }

    initMatrix(dest, first.rows, second.cols);

    for (size_t i = 0; i < first.rows; ++i) {
        for (size_t j = 0; j < second.cols; ++j) {
            int sum = 0;
            for (size_t k = 0; k < first.cols; ++k) {
                sum += MATRIX_AT(first, i+1, k+1) * MATRIX_AT(second, k+1, j+1);
            }

            MATRIX_AT(*dest, i+1, j+1) = sum;
        }
    }
}

void multMatrix(Matrix matrix, Value scalar) {
    for (size_t i = 1; i <= matrix.rows; ++i) {
        for (size_t j = 1; j <= matrix.cols; ++j) {
            MATRIX_AT(matrix, i, j) *= scalar;
        }
    }
}

void addMatrix(Matrix first, Matrix second) {
    if (first.rows == second.rows && first.cols == second.cols) {
        for (size_t i = 1; i <= first.rows; ++i) {
            for (size_t j = 1; j <= first.cols; ++j) {
                MATRIX_AT(first, i, j) += MATRIX_AT(second, i, j);
            }
        }
    } else {
        fprintf(stderr, "ERROR: Dimensions not equal for addition.");
        exit(1);
    }
}
void subMatrix(Matrix first, Matrix second) {
    if (first.rows == second.rows && first.cols == second.cols) {
        for (size_t i = 1; i <= first.rows; ++i) {
            for (size_t j = 1; j <= first.cols; ++j) {
                MATRIX_AT(first, i, j) -= MATRIX_AT(second, i, j);
            }
        }
    } else {
        fprintf(stderr, "ERROR: Dimensions not equal for subtraction.");
        exit(1);
    }

}

void randMatrix(Matrix *dest, size_t rows, size_t cols) {
    srand(time(NULL));
    initMatrix(dest, rows, cols);

    for (size_t i = 1; i <= dest->rows; ++i) {
        for (size_t j = 1; j <= dest->cols; ++j) {
            MATRIX_AT(*dest, i, j) = randm();
        }
    }
}

void printMatrix(Matrix matrix, const char *name) {
    printf("%s ", name);
    printf("[ \n");
    for (size_t i = 1; i <= matrix.rows; ++i) {
        for (size_t j = 1; j <= matrix.cols; ++j) {
            printf("  %f  ", (double)MATRIX_AT(matrix, i, j));
        }
        printf("\n");
    }
    printf("] \n");
}
void freeMatrix(Matrix matrix) {
    free(matrix.cells);
}

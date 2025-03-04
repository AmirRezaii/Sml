#ifndef MATRIX_H_
#define MATRIX_H_

#include <stddef.h>

typedef double Value;

typedef struct {
    Value *cells;
    size_t rows;
    size_t cols;
} Matrix;

#define MATRIX_AT(matrix, row, col) *matrixAt((matrix), (row), (col))

void initMatrix(Matrix *matrix, size_t rows, size_t cols);
Value *matrixAt(Matrix matrix, size_t row, size_t col);
void assignMatrix(Matrix matrix, Value *values);
void copyMatrix(Matrix *dest, Matrix source);
void dotMatrix(Matrix *dest, Matrix first, Matrix second);
void multMatrix(Matrix matrix, Value scalar);
void addMatrix(Matrix first, Matrix second);
void subMatrix(Matrix first, Matrix second);
void randMatrix(Matrix *dest, size_t rows, size_t cols);
void printMatrix(Matrix matrix, const char *name);
void freeMatrix(Matrix matrix);

#endif // MATRIX_H_

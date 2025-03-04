#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include "matrix.h"

#define TRAIN_INFO
#define EPOCH 50000

typedef struct {
    size_t num_of_data;

    size_t input_size;
    Value *inputs;

    size_t num_of_outputs;
    size_t output_size;
    Value *outputs;
} Data;

typedef struct {
    Matrix weights;
    Matrix biases;
} Neuron;

Data makeData(Value *inputs, Value *outputs, size_t num_of_data, size_t input_size, size_t output_size) {
    Data data;
    data.num_of_data = num_of_data;
    data.input_size = input_size;
    data.output_size = output_size;
    data.inputs = inputs;
    data.outputs = outputs;

    return data;
}


double sigmoid(double input) {
    // Sigmoid activation function.
    double res = 1 / (1 + exp(-input));
    return res;
}

void activate(Matrix matrix) {
    for (size_t i = 1; i <= matrix.rows; ++i) {
        for (size_t j = 1; j <= matrix.cols; ++j) {
            MATRIX_AT(matrix, i, j) = sigmoid(MATRIX_AT(matrix, i, j));
        }
    }
}

void evalNeuron(Matrix *dest, Neuron neuron, Matrix input) {
    dotMatrix(dest, neuron.weights, input);
    addMatrix(*dest, neuron.biases);
    activate(*dest);
}

double loss(Matrix weights, Matrix biases, Data data) {
    double res = 0;

    Matrix input;
    initMatrix(&input, data.input_size, 1);

    for (size_t i = 0; i < data.num_of_data; ++i) {
        assignMatrix(input, data.inputs + (i * data.input_size));

        Neuron neuron = { weights, biases };

        Matrix mid;
        evalNeuron(&mid, neuron, input);

        int N = data.output_size;
        double loss = 0;

        for (int j = 1; j <= N; ++j) {
            double out = MATRIX_AT(mid, j, 1) - *(data.outputs + (i*N) + (j-1));
            loss += out*out;
        }

        res += loss/N;

        freeMatrix(mid);
    }

    freeMatrix(input);
    return res;
}

void gradient(Matrix *weights_gradient, Matrix *biases_gradient, Matrix weights, Matrix biases, Data data, Value eps) {

    initMatrix(weights_gradient, weights.rows, weights.cols);
    for (size_t i = 1; i <= weights.rows; ++i) {
        for (size_t j = 1; j <= weights.cols; ++j) {
            Matrix mid;
            copyMatrix(&mid, weights);
            MATRIX_AT(mid, i, j) += eps;
            MATRIX_AT(*weights_gradient, i, j) = (loss(mid, biases, data) - loss(weights, biases, data)) / eps;
            freeMatrix(mid);
        }
    }

    initMatrix(biases_gradient, biases.rows, biases.cols);
    for (size_t i = 1; i <= biases.rows; ++i) {
        for (size_t j = 1; j <= biases.cols; ++j) {
            Matrix mid;
            copyMatrix(&mid, biases);
            MATRIX_AT(mid, i, j) += eps;
            MATRIX_AT(*biases_gradient, i, j) = (loss(weights, mid, data) - loss(weights, biases, data)) / eps;
            freeMatrix(mid);
        }
    }

}

void printResult(Matrix weights, Matrix biases, Data data) {
    for (size_t i = 0; i < data.num_of_data; ++i) {
        Matrix input;
        initMatrix(&input, data.input_size, 1);
        assignMatrix(input, data.inputs + (i * data.input_size));

        printf("[ ");
        for (size_t j = 0; j < data.input_size; ++j) {
            printf(" %f ", MATRIX_AT(input, j+1, 1));
        }
        printf(" ]    =>    ");

        Neuron neuron = { weights, biases };

        Matrix mid;
        evalNeuron(&mid, neuron, input);

        printf("[ ");
        for (size_t j = 0; j < data.output_size; ++j) {
            printf(" %f ", MATRIX_AT(mid, j+1, 1));
        }
        printf(" ] \n");

        freeMatrix(input);
        freeMatrix(mid);
    }
}

void train(Matrix weights, Matrix biases, Data data) {
    double eps = 1.0e-0;
    double rate = 1.0e-0;

    for (int i = 1; i <= EPOCH; ++i) {
        Matrix weights_gradient;
        Matrix biases_gradient;
        gradient(&weights_gradient, &biases_gradient, weights, biases, data, eps);
        multMatrix(weights_gradient, rate);
        multMatrix(biases_gradient, rate);

        subMatrix(weights, weights_gradient);
        subMatrix(biases, biases_gradient);

        freeMatrix(weights_gradient);
        freeMatrix(biases_gradient);

        #ifdef TRAIN_INFO
        if (i % (EPOCH / 1) == 0) {
            printResult(weights, biases, data);
            printf("loss: %f\n", loss(weights, biases, data));
            printf("epoch: %d\n", i);
        }
        #endif
    }
}


int main() {
    // ML Dataset
    double inputsCells[][2] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
    double desiredCells[][1] = {{0}, {1}, {1}, {1}};
    Data data = makeData(*inputsCells, *desiredCells, 4, 2, 1);

    Matrix weights;
    randMatrix(&weights, 1, 2);

    Matrix biases;
    randMatrix(&biases, 1, 1);

    train(weights, biases, data);

    printResult(weights, biases, data);

    printMatrix(weights, "weights");
    printMatrix(biases, "biases");

    freeMatrix(weights);
    freeMatrix(biases);
    return 0;
}

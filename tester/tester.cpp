#include "tester.h"

void Tester::execute() {
    Mocker mocker;

    for (int i = 0; i < NUMBER_OF_TESTS; ++i) {
        unsigned int rows = mocker.generateRandomInt(10);
        unsigned int columns = mocker.generateRandomInt(10);
        testMatrix<int>(rows, columns);
    }
}

void Tester::customMatrixMult() {
    //matrix mult test
    auto matrixA = new Matrix<int>(3, 4);
    matrixA->set(0,0,6);
    matrixA->set(0,1,4);
    matrixA->set(0,3,5);

    matrixA->set(1,0,7);
    matrixA->set(1,1,2);
    matrixA->set(1,2,8);
    matrixA->set(1,3,-4);

    matrixA->set(2,0,1);
    matrixA->set(2,1,3);
    matrixA->set(2,2,-3);
    matrixA->set(2,3,9);

    auto matrixB = new Matrix<int>(4, 2);
    matrixB->set(0,0,-1);
    matrixB->set(0,1,6);
    matrixB->set(1,0,4);
    matrixB->set(1,1,2);
    matrixB->set(2,0,7);
    matrixB->set(2,1,3);
    matrixB->set(3,0,-2);
    matrixB->set(3,1,10);

    cout << "Matrix A"<<endl;
    matrixA->print();

    cout << "Matrix B"<<endl;
    matrixB->print();

    auto matrixAB = *matrixA * *matrixB;
    cout << "Matrix AxB"<<endl;
    matrixAB.print();

    delete matrixA;
    delete matrixB;
}
template <typename T>
void Tester::testMatrix(unsigned int rows, unsigned int columns) {
    Mocker mocker;

    T **matrix1 = buildMatrix<T>(rows, columns);
    Matrix<T> test1 = setMatrix<T>(matrix1, rows, columns);

    unsigned int scalar = mocker.generateRandomInt(10);
    Matrix<T> result = test1 * scalar;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            ASSERT(result(i, j) == matrix1[i][j] * scalar, "There is a problem with the scalar multiplication");
        }
    }

    int **matrix2 = buildMatrix<T>(rows, columns);
    Matrix<T> test2 = setMatrix<T>(matrix2, rows, columns);
    //result = test1 + test2; //call copy constructor
    Matrix<T> result2 = test1 + test2; //calling copy constructor. Avoid override = operator
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            ASSERT(result2(i, j) == matrix1[i][j] + matrix2[i][j], "There is a problem with the addition");
        }
    }

    //result = test1 - test2
    Matrix<T> result3 = test1 - test2; //calling copy constructor. Avoid override = operator
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            ASSERT(result3(i, j) == matrix1[i][j] - matrix2[i][j], "There is a problem with the subtraction");
        }
    }
}

template <typename T>
T** Tester::buildMatrix(unsigned int rows, unsigned int columns) {
    Mocker mocker;

    T **matrix = new int*[rows];
    for (int i = 0; i < rows; ++i) {
        matrix[i] = mocker.generateRandomArray<T>(columns);
    }

    return matrix;
}

template <typename T>
Matrix<T> Tester::setMatrix(T **&matrix, unsigned int rows, unsigned int columns) {
    Matrix<T> result(rows, columns);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            result.set(i, j, matrix[i][j]);
            //cout << i << " " << j << " " << matrix[i][j] << endl;
            ASSERT(result(i, j) == matrix[i][j], "There is a problem with the set or operator()");
        }
    }

    return result;
}
#ifndef SPARSE_MATRIX_MATRIX_H
#define SPARSE_MATRIX_MATRIX_H

#include "node.h"
#include <vector>
#include <stdexcept>
#include <iomanip>

using namespace std;

template<typename T>
class Matrix {
public:
    static const int cout_length=3;
private:
    unsigned rows, columns;

    vector<Node<T> *> row_link; //x
    vector<Node<T> *> col_link; //y

    bool whichPosInCol(unsigned x, unsigned y, Node<T> **&pointer) {
        bool resultBool = false;

        for (pointer = &row_link[x]; *pointer != NULL; pointer = &(*pointer)->down) {
            if (y == (*pointer)->posY) {
                *pointer = (*pointer)->down;
                resultBool = true;
                break;
            } else if (y < (*pointer)->posY) {
                *pointer = (*pointer)->down;
                resultBool = false;
            }
        }

        return resultBool;
    }

    bool whichPosInRow(unsigned x, unsigned y, Node<T> **&pointer) {
        bool resultBool = false;

        for (pointer = &col_link[y]; *pointer != NULL; pointer = &(*pointer)->next) {
            //if((*lpp)->data == data) {
            if (x == (*pointer)->posX) {
                *pointer = (*pointer)->next;
                resultBool = true;
                break;
            } else if (x < (*pointer)->posX) {
                *pointer = (*pointer)->next;
                resultBool = false;
            }
        }

        return resultBool;
    }

    void initializeRowsandCols(unsigned rows, unsigned columns) {
        for (int i = 0; i < rows; ++i)
            row_link.push_back(nullptr);

        for (int i = 0; i < columns; ++i)
            col_link.push_back(nullptr);
    }
public:
    Matrix(unsigned rows, unsigned columns) {
        this->rows = rows;
        this->columns = columns;

        initializeRowsandCols(this->rows, this->columns);
    }

    Matrix(const Matrix &oldMatrix) { //Copy constructor
        this->rows = oldMatrix.rows;
        this->columns = oldMatrix.columns;

        initializeRowsandCols(this->rows, this->columns);

        for(int i=0 ; i < this->rows ; ++i) {
            for(int j=0 ; j < this->columns ; ++j) {
                set(i, j, oldMatrix(i, j) );
            }
        }
    }

    void set(unsigned x, unsigned y, T data) {
        auto newNode = new Node<T>();

        newNode->data = data;
        newNode->posX = x;
        newNode->posY = y;

        Node<T> **doublePointerX = nullptr;
        Node<T> **doublePointerY = nullptr;

        whichPosInCol(x, y, doublePointerY);
        whichPosInRow(x, y, doublePointerX);

        newNode->down = *doublePointerY;
        *doublePointerY = newNode;

        newNode->next = *doublePointerX;
        *doublePointerX = newNode;
    }

    T operator()(unsigned x, unsigned y) const {

        return getElementByRowCol(x, y);
        /*auto auxCol = row_link[x];

        if (!y && auxCol)
            return auxCol->data;

        if(!auxCol)
            return 0;

        auto downAux = auxCol->down;

        while (downAux) {
            if (downAux->posY == y)
                return downAux->data;

            downAux = downAux->down;
        }

        return 0;*/
    }

    T getElementByRowCol(unsigned x, unsigned y) const {
        auto auxCol = row_link[x];

        if (!y && auxCol)
            return auxCol->data;

        if(!auxCol)
            return 0;

        auto downAux = auxCol->down;

        while (downAux) {
            if (downAux->posY == y)
                return downAux->data;

            downAux = downAux->down;
        }

        return 0;
    }
    Matrix<T> operator*(T scalar) const {
        Matrix<T> result(this->rows, this->columns);

        for (auto auxCol: row_link) {
            if (!auxCol)
                continue;

            while (auxCol) {
                result.set(auxCol->posX, auxCol->posY, auxCol->data * scalar);
                auxCol = auxCol->down;
            }
        }

        return result;
    }

    Matrix<T> operator+(Matrix<T> other) const {
        Matrix<T> result(this->rows, this->columns);

        if(this->rows != other.rows || this->columns != other.columns) {
            auto msg = "Operation not allowed: Matrix aren't the same size";
            throw runtime_error(msg);
        }

        for (auto auxRow: row_link) {
            if (!auxRow)
                continue;

            while (auxRow) {
                result.set(
                        auxRow->posX,
                        auxRow->posY,
                        auxRow->data + other(auxRow->posX, auxRow->posY) );
                auxRow = auxRow->down;
            }
        }

        return result;
    }

    Matrix<T> operator-(Matrix<T> other) const {
        Matrix<T> result(this->rows, this->columns);

        if(this->rows != other.rows || this->columns != other.columns) {
            auto msg = "Operation not allowed: Matrix aren't the same size";
            throw runtime_error(msg);
        }

        return result;
    }

    Matrix<T> operator*(Matrix<T> other) const {
        Matrix<T> result(this->rows, other.columns);

        if(this->columns != other.rows) {
            auto msg = "Matrix multiplication not allowed: Matrix aren't the same size";
            throw runtime_error(msg);
        }

        for (int i=0;i<this->rows;++i) {
            for (int j=0;j<this->columns;++j) {
                T aux = 0;
                for (int k=0;k<this->columns;k++) {
                    auto fact1 = getElementByRowCol(i, k);
                    auto fact2 = other(k, j);
                    aux = aux +  fact1 * fact2;
                }
                result.set(i, j, aux);
            }
        }


        return result;
    }

    Matrix<T> transpose() const {
        Matrix<T> result(this->columns, this->rows);

        for( int i=0 ; i < this->rows ; ++i) {
            if( this->row_link[i] ) {
                auto colAux = row_link[i];

                while(colAux) {
                    result.set(colAux->posY, colAux->posX, colAux->data);
                    colAux = colAux->down;
                }

            }
        }

        return result;
    }

    void print() const {

        for( int i=0 ; i < this->rows ; ++i) {
            if( !this->row_link[i] ) {  //empty row
                printZeros(this->columns);
                cout << endl;
            }  else {
                auto colAux = row_link[i];
                int colNum = colAux->posY;

                printZeros(colNum);

                while(colAux) {
                    printZeros(colAux->posY - colNum - 1);
                    cout << setw(cout_length) << colAux->data << " ";
                    colNum = colAux->posY;
                    colAux = colAux->down;
                }
                printZeros(columns - colNum - 1);
                cout << endl;
            }
        }
    }

    void printZeros(int cant) const {
        int dummy=0;
        for(int j=0 ; j < cant ; ++j) {
            cout<<setw(cout_length)<<dummy<<" ";
        }
    }

    ~Matrix() {
        for(auto colAux : col_link ) {
            if(colAux) {
                colAux->killSelf(); break;
            }
        }

        row_link.clear();
        col_link.clear();
    }
};

#endif //SPARSE_MATRIX_MATRIX_H
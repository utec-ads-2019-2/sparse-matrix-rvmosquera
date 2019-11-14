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
                //*pointer = (*pointer)->down;
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
            if (x == (*pointer)->posX) {
                //*pointer = (*pointer)->next;
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
        if (row_link.size() > 0)
            row_link.clear();

        for (int i = 0; i < rows; ++i)
            row_link.push_back(nullptr);

        if(col_link.size() > 0)
            col_link.clear();

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
                auto value = oldMatrix(i, j);
                if (!value)
                    continue;

                set(i, j, value );
            }
        }
    }

    void set(unsigned x, unsigned y, T data) {
        Node<T> **doublePointerX = nullptr;
        Node<T> **doublePointerY = nullptr;

        auto resultY = whichPosInCol(x, y, doublePointerY);
        auto resultX = whichPosInRow(x, y, doublePointerX);

        if(!resultY) {

            if(!data) { //No node creation for 0 value
                return;
            }

            auto newNode = new Node<T>();

            newNode->data = data;
            newNode->posX = x;
            newNode->posY = y;

            newNode->down = *doublePointerY;
            *doublePointerY = newNode;

            newNode->next = *doublePointerX;
            *doublePointerX = newNode;
        } else {
            if(data) {
                /*auto newNode = new Node<T>();

                newNode->data = data;
                newNode->posX = x;
                newNode->posY = y;

                newNode->down = *doublePointerY;
                *doublePointerY = newNode;

                newNode->next = *doublePointerX;
                *doublePointerX = newNode;*/
                (*doublePointerY)->data = data;
                (*doublePointerX)->data = data;
            } else { //instead of set the node with 0, delete it
                *doublePointerY = (*doublePointerY)->down;
                *doublePointerX = (*doublePointerX)->next;
            }
        }
    }

    T operator()(unsigned x, unsigned y) const {

        return getElementByRowCol(x, y);
    }

    T getElementByRowCol(unsigned x, unsigned y) const {
        auto auxCol = row_link[x];

        if(!auxCol)
            return 0;

        auto downAux = auxCol;

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

    Matrix<T> operator+(Matrix<T> other)  const{
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

        return result; //not calling copy constructor
    }

    Matrix<T> operator-(Matrix<T> other) const {
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
                        auxRow->data - other(auxRow->posX, auxRow->posY) );
                auxRow = auxRow->down;
            }
        }

        return result;
    }

    Matrix<T> operator*(const Matrix<T> other) const {
        Matrix<T> result(this->rows, other.columns);

        if(this->columns != other.rows) {
            auto msg = "Matrix multiplication not allowed: Matrix aren't the same size";
            throw runtime_error(msg);
        }

        for (int i=0;i< result.rows;++i) {
            for (int j=0;j< result.columns;++j) {
                T aux = 0;
                for (int k=0;k<this->columns;k++)
                    aux = aux +  getElementByRowCol(i, k) * other(k, j);

                result.set(i, j, aux);
            }
        }

        return result;
    }

    Matrix<T>& operator=(const Matrix<T> &rhs) {
        if (this != &rhs) {                  // do not copy to yourself
            if (rhs.rows > 0) {            // copy string from rhs if it exists

                this->rows = rhs.rows;
                this->columns = rhs.columns;

                initializeRowsandCols(this->rows, this->columns);

                for(int i=0 ; i < this->rows ; ++i) {
                    for(int j=0 ; j < this->columns ; ++j) {
                        auto value = rhs(i, j);
                        if (!value)
                            continue;
                        set(i, j, value );
                    }
                }
            }
        }

        return *this;                        // always end with this line
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
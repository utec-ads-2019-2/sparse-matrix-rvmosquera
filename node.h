#ifndef SPARSE_MATRIX_NODE_H
#define SPARSE_MATRIX_NODE_H

template <typename T>
class Matrix;

template <typename T>
class Node {
//protected:
public:
    T data;
    int posX;
    int posY;
    Node<T> *next, *down;

    explicit Node() {
        next = nullptr;
        down = nullptr;
        posX = 0;
        posY = 0;
    }

    friend class Matrix<T>;

    void killSelf() {
        if (this->next)
            this->next->killSelf();
        else if (this->down)
            this->down->killSelf();

        delete this;
    }

    ~Node() {

    }
};

#endif //SPARSE_MATRIX_NODE_H

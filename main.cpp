#include <iostream>

#include "tester/tester.h"

int main() {
    //Tester::execute();
    Tester::customMatrixMult();
    /*auto test = new Matrix<int>(7, 7);

    test->set(1, 0, 7);
    test->set(2, 0, 10);
    test->set(2, 3, 11);
    test->set(3, 6, 12);
    test->set(4, 3, 17);
    test->set(5, 1, 9);
    test->set(5, 2, 1);
    test->set(5, 5, 21);
    test->set(6, 0, 3);
    test->set(6, 1, 2);
    test->set(6, 2, 1);
    test->set(6, 4, 1);
    test->set(6, 5, 2);
    test->set(6, 6, 4);

    cout << "Original Matrix"<<endl;
    test->print();
    cout << "after update node 5,5 with 189"<<endl;
    test->set(5, 5, 189);
    test->print();
    cout << "after set node 5,5 with 0"<<endl;
    test->set(5, 5, 0);
    test->print();

    cout << "Transpose Matrix"<<endl;
    auto resMatrix = test->transpose();
    resMatrix.print();

    auto resMult = *test * resMatrix;
    cout << "Mult result"<<endl;
    resMult.print();

    delete test;*/

    return EXIT_SUCCESS;
}


#include <iostream>
#include <map>
#include <vector>
#include "mathpack.h"

using namespace std;

map<char, double> emptyMap;
map<char, double> XYZ;

//cos i cbf installing boost for check equal
template <class T>
void check_equal(T lhs, T rhs) {
    if (lhs == rhs) {
        std::cout << "Test Passed; " << lhs << " == " << rhs << std::endl;
    } else {
        std::cout << "!Test Failed; " << lhs << " != " << rhs << std::endl;
    }
}

void testNode() {
//Node superclass tests
    std::cout << "Testing Node Class\n";
    Node* myNode = new Node();
    check_equal<bool>(myNode->compute(emptyMap), false);
    check_equal<bool>(myNode->compute(XYZ), false);
    check_equal<bool>(myNode->simplify(), false);
    delete myNode;
    std::cout << "End Node Class Test\n\n";
    //End Node tests
}

void testConst() {
//Constant Value tests
    std::cout << "Testing Constant Class\n";
    Constant* myConst = new Constant();
    Constant* myConstTwo = new Constant(100);
    check_equal<bool>(myConst->compute(emptyMap), true);
    check_equal<bool>(myConst->compute(XYZ), true);
    check_equal<int>(myConst->get_last_result(),0);
    myConstTwo->compute(emptyMap);
    check_equal<int>(myConstTwo->get_last_result(),100);
    check_equal<bool>(myConst->simplify(), false);
    delete myConst;
    delete myConstTwo;
    std::cout << "End Constant Class Test\n\n";
    //End Constant tests
}

void testVar() {
//Variable Value tests
    std::cout << "Testing Variable Class\n";
    VariableResolve* myVar = new VariableResolve();
    VariableResolve* myVarTwo = new VariableResolve('x');
    check_equal<bool>(myVar->compute(emptyMap), false);
    check_equal<bool>(myVar->compute(XYZ), false);

    check_equal<bool>(myVarTwo->compute(XYZ), true);
    check_equal<int>(myVarTwo->get_last_result(),1);
    check_equal<bool>(myVarTwo->simplify(), false);
    delete myVar;
    delete myVarTwo;
    std::cout << "End Variable Class Test\n\n";
    //End Variable tests

}

void testAdd() {
   //Add tests
    std::cout << "Testing Add Class\n";
    Constant* C1 = new Constant(100);
    Constant* C2 = new Constant(200);
    Constant* C3 = new Constant(-50.50);
    Constant* C4 = new Constant(100);
    Constant* C5 = new Constant(100);
    Constant* C6 = new Constant(100);
    Constant* C7 = new Constant(200);
    Constant* C8 = new Constant(100);
    Constant* C9 = new Constant(200);
    VariableResolve* V1 = new VariableResolve('x');
    VariableResolve* V2 = new VariableResolve('y');
    VariableResolve* V3 = new VariableResolve('x');

    Add* A1 = new Add();
    Add* A2 = new Add(C1, C2);
    Add* A3 = new Add(C4, C3);

    Add* A4 = new Add(C5, V1);

    Add* A5 = new Add(C6, C7);
    Add* A6 = new Add(C8, C9);
    Add* A7 = new Add(A5, A6);

    Add* A8 = new Add(V3, V2);

    check_equal<bool>(A1->compute(emptyMap), true);
    check_equal<bool>(A2->compute(emptyMap), true);
    check_equal<bool>(A3->compute(emptyMap), true);
    check_equal<bool>(A4->compute(emptyMap), false);

    check_equal<bool>(A1->compute(XYZ), true);
    check_equal<bool>(A2->compute(XYZ), true);
    check_equal<bool>(A3->compute(XYZ), true);
    check_equal<bool>(A4->compute(XYZ), true);
    check_equal<bool>(A8->compute(XYZ), true);

    check_equal<int>(A1->get_last_result(),0);
    check_equal<int>(A2->get_last_result(),300);
    check_equal<double>(A3->get_last_result(),49.50);
    check_equal<int>(A4->get_last_result(),101);
    check_equal<int>(A8->get_last_result(),3);

    check_equal<bool>(A7->compute(emptyMap), true);
    check_equal<int>(A7->get_last_result(),600);
    check_equal<int>(A7->get_num_recursive_children(), 6);
    check_equal<bool>(A7->simplify(), true);
    check_equal<int>(A7->get_num_recursive_children(), 2);
    check_equal<int>(A7->get_last_result(),600);



    delete A1;
    delete A2;
    delete A3;
    delete A4;
    delete A7;
    delete A8;
    std::cout << "End Add Class Test\n\n";
    //End Add tests
}

int main()
{
    //write some unit testing functions
    XYZ['x'] = 1;
    XYZ['y'] = 2;
    XYZ['z'] = 3;

    testNode();
    testConst();
    testVar();
    testAdd();

    return 0;
}

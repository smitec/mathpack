#include <math.h>
#include <vector>
#include <string>
#include <map>


//!Superclass.
/**
The Node class forms the basis of all operations within the function class. 
It is names to represent its use as the node of a tree. 
A node can have any number of children representing the variable number of operands within a function.

for example:
    sin would have 1 child
    + would have 2 children
user functions could have any number of children. i.e f(x1,x2,x3...xn)
*/
class Node {
protected:
    //!Collection of children belonging to the Node
    std::vector<Node*> children;

    //!Result of last calculation
    double lastResult;
    std::string opCode;
public:
    //!Constructor (Empty for Superclass)
    Node(){};

    //!destructor (Empty for Superclass)
    ~Node(){};

    //!Compute function
    /**
    *Function to compute the value of the node. Computes all its children then uses them to perform an operation.
    *Terminating Nodes are needed such as constant and variable resolvers.
    *
    *Function is virtual as all subcleasses must have their own version of this function.
    *
    * @param variables a map of the variable values and the chars representing them
    * @return bool represents wheather the Node can compute given the variable information
    */
    virtual bool compute(std::map<char,double> variables){return false;}

    //!Simplify Function
    /**
    *Node calls compute on its children with no variable info. If any return true that sub tree is
    *removed and replaced by a constant Node.
    *
    * @return true if any children have been simplified
    */
    virtual bool simplify(){return false;}

    //! Simple Getter Returning the last computed result
    double get_last_result(){ return this->lastResult;}

    //!Gets the number of children
    int get_num_children(){return (int)this->children.size();}

    //!gets the number of clildren + grandchildren etc etc.
    int get_num_recursive_children();
    
    virtual std::string to_string(){return "";}

    /*TODO
        Operator <<
        probably more as I make other classes
    */
};

//=========================================================
//Divides Node Into Temrinal and Non Terminal Operations
//=========================================================
//!Outer node of the formula tree.
/**
Classes used to indicate that a node is the outermost node on a branch.
These nodes do not take arguments and will be the stop case for all recursive computations.
*/
class Terminal : public Node {
public:
    //!Basic constructor
    Terminal(){};

    //!Basic destructor
    ~Terminal(){};

    //!Stop case for recursive simplify calls
    bool simplify(){return false;}
};

//!Internal node of the formula tree.
/**
Classes which take operands. All NonTerminal derivitives will have at least one argument.
*/
class NonTerminal : public Node {
public:

    //! Basic Constructor
    NonTerminal(){};

    //! Basic Destructor
    /**
        Will call delete on all children of the node.
    */
    ~NonTerminal();

    //! Constructor From Vector
    NonTerminal(std::vector<Node*> children);

    //!Simplyfy Tree
    /**
    simplify uses tree traversal to try and simplify the function
    the basic methedology is if a branch can be calculated with no variables being passed to it then
    it can be replaced by a single constant node. This does not yet apply commutative properties but
    this will hopefully happen in the future.
    */
    bool simplify();
    
    void remove_children();
};

//=========================================================
//Divides NonTerminal Based on Number of Operands
//=========================================================
//!Operations with strictly 2 operands
/**
This covers most basic operations such as + - / and *.
These functions only need to be able to perform their actions on a lhs and rhs at any time.
*/
class TwoOp : public NonTerminal {
public:
    TwoOp();
    ~TwoOp() {};
    TwoOp(Node* lhs, Node* rhs);
    TwoOp(std::vector<Node*> children) : NonTerminal(children) {};
    virtual double operation(double lhs, double rhs){ return 0;}
    bool compute(std::map<char,double> variables);
    std::string to_string(){return "";}
};

//=========================================================
//Terminal Operations
//=========================================================
//TODOC
class Constant : public Terminal {
private:
    double constValue;
public:
    Constant();
    Constant(double constVal);
    ~Constant();

    bool compute(std::map<char,double> variables);

    //Already a terminal node has no children
    bool simplify() {return false;}
    
    std::string to_string();
};

//TODOC
class VariableResolve : public Terminal {
private:
    char variableIndicator;
public:
    VariableResolve();
    VariableResolve(char var);
    ~VariableResolve();

    bool compute(std::map<char,double> variables);

    //Already a terminal node has no children
    bool simplify() {return false;}
    
    std::string to_string(){return "";}
};

//=========================================================
//Operations Taking Two Arguments
//=========================================================
class Add : public TwoOp {
public:
    Add();
    Add(Node* lhs, Node* rhs);
    Add(std::vector<Node*> children);
    ~Add();

    double operation(double lhs, double rhs);
};

class Subtract : public TwoOp {
public:
    Subtract();
    Subtract(Node* lhs, Node* rhs);
    Subtract(std::vector<Node*> children);
    ~Subtract();

    double operation(double lhs, double rhs);
};

class Multiply : public TwoOp {
public:
    Multiply();
    Multiply(Node* lhs, Node* rhs);
    Multiply(std::vector<Node*> children);
    ~Multiply();

    double operation(double lhs, double rhs);
};

class Divide : public TwoOp {
public:
    Divide() : TwoOp() {};
    Divide(Node* lhs, Node* rhs) : TwoOp(lhs,rhs) {};
    Divide(std::vector<Node*> children) : TwoOp(children) {};
    ~Divide() {};

    double operation(double lhs, double rhs);
};

class Power : public TwoOp {
public:
    Power() : TwoOp() {};
    Power(Node* lhs, Node* rhs) : TwoOp(lhs,rhs) {};
    Power(std::vector<Node*> children) : TwoOp(children) {};
    ~Power() {};

    double operation(double lhs, double rhs);
};
//=========================================================
//Operations Taking a Single Argument
//=========================================================

/*to add:
    sin cos tan
    sinh cosh tanh
    abs
    ceil
    floor
*/

class Factorial : public NonTerminal {
public:
    Factorial();
    Factorial(Node* child);
    Factorial(std::vector<Node*> children);
    ~Factorial();
    
    bool compute(std::map<char,double> variables);
};

class SquareRoot : public NonTerminal {
public:
    SquareRoot();
    SquareRoot(Node* child);
    SquareRoot(std::vector<Node*> children);
    ~SquareRoot();
    
    bool compute(std::map<char,double> variables);
};

//=========================================================
//Operations Taking a and # of Arguments
//=========================================================
/*
 other functions to add:
 min
 max
 */

//=========================================================
//Actual formula Class
//=========================================================

//!Formula.
/**
 this class is the translation point between strings and classes
 it uses pretty standard formual parsign methods to split the string
 into its parts. eventually it will be able to handle RPN as well but
 for now normal input is okay. input in infix should be converted to RPN then parsed
 things are much much easier that way things will go like this
 
    opCode(arg1 arg2 arg3....)
 
 nested functions:
    opCode1(opCode2(arg arg) arg opCode1(ar arg))
 
 fucntions should be input as name(x1,x1...var) = some function of the operands
 other lines will be computed on the spot.
 */
/*
TODO: to make this work i need a custom node type that evaluates its child when evaluated
 then for the function factory i can just build based on the opCode parameter of the function
 to update this i need to change the var and const ones to c and v or something along those lines.
*/
class Formula {
private:
    //!head of tree
    /**
     uses NonTerminal super class as all it has is children
     this allows simplify to be called on the head which should
     then create the most basic simply combineable funciton
    */
    Node* head;
    
    //!original string used to generate the function class
    std::string original;
    
    //!name given to the function in the string
    std::string functionName;
public:
    Formula();
    Formula(std::string str); //the money-maker most of the hard work goes on here
    ~Formula();
    
};

//can yet but will be able to handel user defined funcs
std::vector<Node*> avaliableFunctions;

Node* function_factory(std::string opCode, std::vector<Node*> args);


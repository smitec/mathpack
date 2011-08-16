#include "mathpack.h"

//=========================================================
//Node Class
//=========================================================
int Node::get_num_recursive_children(){
    int result = this->get_num_children();
    for (int i = 0; i < this->get_num_children(); i++) {
        result += this->children[i]->get_num_recursive_children();
    }
    return result;
}

//=========================================================
//NonTerminal Class
//=========================================================
bool NonTerminal::simplify(){
    std::map<char,double> emptyMap;
    bool result = true;
    Constant* temp;
    for (int i = 0; i < this->get_num_children(); i++) {
        //can the node be computed with no variables and is it a non terminal node
        if (this->children[i]->compute(emptyMap)) {
            //dont need to redo terminal nodes
            if (this->children[i]->simplify()) {
                temp = new Constant(this->children[i]->get_last_result());
                delete this->children[i];
                this->children[i] = (Node*)temp;
            }
        } else {
            result = false;
        }
    }
    return result;
}

NonTerminal::NonTerminal(std::vector<Node*> children){
    this->children = children;
}

NonTerminal::~NonTerminal(){
    //free the children
    for (int i = 0; i < this->get_num_children(); i++) {
        delete this->children[i];
    }
}

//=========================================================
//TwoOp Class
//=========================================================
bool TwoOp::compute(std::map<char,double> variables){
    bool result;
    double finalVal = 0;
    for (int i = 0; i < this->get_num_children(); i++) {
        result = this->children[i]->compute(variables);
        if (result) {
            finalVal = this->operation(finalVal, this->children[i]->get_last_result());
        } else {
            return false;
        }
    }
    this->lastResult = finalVal;
    return true;
}

TwoOp::TwoOp(){
    //by default set lhs and rhs to 0
    Constant* lhs = new Constant(0);
    Constant* rhs = new Constant(0);
    this->children.push_back((Node*)lhs);
    this->children.push_back((Node*)rhs);
}

TwoOp::TwoOp(Node* lhs, Node* rhs){
    this->children.push_back(lhs);
    this->children.push_back(rhs);
}


//=========================================================
//Constant Class
//=========================================================
Constant::Constant(){
    this->constValue = 0;
}

Constant::Constant(double constVal){
    this->constValue = constVal;
}

Constant::~Constant(){
    //nothing to do
}

bool Constant::compute(std::map<char,double> variables){
    this->lastResult = this->constValue;
    return true;
}

//=========================================================
//Variable Class
//=========================================================
VariableResolve::VariableResolve(){
    this->variableIndicator = '\0';
}

VariableResolve::VariableResolve(char var){
    this->variableIndicator = var;
}

VariableResolve::~VariableResolve(){
    //nothing to do
}

bool VariableResolve::compute(std::map<char,double> variables){
    if (variables.count(this->variableIndicator) == 1) {
        this->lastResult = variables[this->variableIndicator];
        return true;
    } else {
        return false;
    }
}

//=========================================================
//Add Class
//=========================================================
double Add::operation(double lhs, double rhs){
    return lhs + rhs;
}

//=========================================================
//Subtract Class
//=========================================================
double Subtract::operation(double lhs, double rhs){
    return lhs - rhs;
}

//=========================================================
//Multiply Class
//=========================================================
double Multiply::operation(double lhs, double rhs){
    return lhs * rhs;
}

//=========================================================
//Divide Class
//=========================================================
double Divide::operation(double lhs, double rhs){
    return lhs / rhs; //should add div 0 checking
}

//=========================================================
//Power Class
//=========================================================
double Power::operation(double lhs, double rhs){
    return pow(lhs,rhs);
}

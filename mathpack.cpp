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
    //change to copy not assign?
    this->children = children;
}

NonTerminal::~NonTerminal(){
    //free the children
    this->remove_children();
}

void NonTerminal::remove_children() {
    for (int i = 0; i < this->get_num_children(); i++) {
        if (this->children[i] != NULL) {
            delete this->children[i];
        }
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

std::string Constant::to_string() {
    //TODO use << and sstream
    return "";
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
Add::Add() : TwoOp() {
    this->opCode = "+";
}

Add::Add(Node* lhs, Node* rhs) : TwoOp(lhs,rhs) {
    this->opCode = "+";
}

Add::Add(std::vector<Node*> children) : TwoOp(children) {
    this->opCode = "+";
}

Add::~Add() {

}

double Add::operation(double lhs, double rhs){
    return lhs + rhs;
}

//=========================================================
//Subtract Class
//=========================================================
Subtract::Subtract() : TwoOp() {
    this->opCode = "-";
}

Subtract::Subtract(Node* lhs, Node* rhs) : TwoOp(lhs,rhs) {
    this->opCode = "-";
}

Subtract::Subtract(std::vector<Node*> children) : TwoOp(children) {
    this->opCode = "-";
}

Subtract::~Subtract() {

}

double Subtract::operation(double lhs, double rhs){
    return lhs - rhs;
}

//=========================================================
//Multiply Class
//=========================================================
Multiply::Multiply() : TwoOp() {
    this->opCode = "-";
}

Multiply::Multiply(Node* lhs, Node* rhs) : TwoOp(lhs, rhs) {
    this->opCode = "-";
    
}

Multiply::Multiply::Multiply(std::vector<Node*> children) : TwoOp(children) {
    this->opCode = "-";
    
}

Multiply::~Multiply(){
    
    
}

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

//=========================================================
//Single Oerator Classes
//=========================================================

//=========================================================
//Factorial Class
//=========================================================
Factorial::Factorial(){
    this->opCode = "!";
    this->lastResult = -1;
}

Factorial::Factorial(Node* child) {
    this->opCode = "!";
    this->children = std::vector<Node*>();
    this->children.push_back(child);
    this->lastResult = -1;
    
}

Factorial::Factorial(std::vector<Node*> children) {
    this->opCode = "!";
    this->children = children;
    this->lastResult = -1;
}

Factorial::~Factorial(){
    
}

bool Factorial::compute(std::map<char,double> variables) {
    double result = 0;
    int temp = 0;
    
    if (this->children.size() != 1) {
        return false;
    }
    if (this->children[0]->compute(variables) != true) {
        return false;
    }
    result = this->children[0]->get_last_result();
    if (result != round(result)) {
        return false;
    }
    if (result < 0) {
        return false;
    }
    //all the tests passed lets compute this factorial
    temp = int(result);
    this->lastResult = 1;
    while (temp--) {
        this->lastResult *= temp;
    }
    return true;
}

//=========================================================
//Square Root Class
//=========================================================

SquareRoot::SquareRoot() {
    this->opCode = "sqrt";
    this->lastResult = -1;
}

SquareRoot::SquareRoot(Node* child) {
    this->opCode = "sqrt";
    this->lastResult = -1;
    this->children = std::vector<Node*>();
    this->children.push_back(child);
}

SquareRoot::SquareRoot(std::vector<Node*> children) {
    this->opCode = "sqrt";
    this->lastResult = -1;
    this->children = children;
}

SquareRoot::~SquareRoot() {
    
}

bool SquareRoot::compute(std::map<char,double> variables){
    double result = 0;
    
    if (this->children.size() != 1) {
        return false;
    }
    if (this->children[0]->compute(variables) != true) {
        return false;
    }
    result = this->children[0]->get_last_result();
    if (result < 0) { //positive only for now
        return false;
        
    }
    this->lastResult = sqrt(result);
    return true;
}

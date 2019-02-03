/*
    Tori Windrich
    11/15/2017
    Project 4: Derivative Calculator
*/

#include "Node.h"
#include <iostream>
#include <string>

//overloaded constructor - each value is initialized to zero/false/null by default
Node::Node(int coeff, int exp, std::string trig, int trigC, bool neg, int num, int den, int tnum, int tden)
{
    //assigns each value
    coefficient = coeff;
    exponent = exp;
    trigIdentifier = trig;
    trigCoefficient = trigC;
    isNeg = neg;
    numerator = num;
    denominator = den;
    trigNumerator = tnum;
    trigDenominator = tden;
    nextPtr = nullptr;
}//constructor

//sets the next value for the linked list to changeNext
void Node::snext(Node* changeNext)
{
    nextPtr = changeNext;
}//set next

//sets the coefficient to coeff
void Node::setCoeff(int coeff)
{
    coefficient = coeff;
}//setCoefficient

//sets the exponent to exp
void Node::setExp(int exp)
{
    exponent = exp;
}//setExponent

//sets the trig identifier to trig
void Node::setTrigId(std::string trig)
{
    trigIdentifier = trig;
}//setTrigIdentifier

//sets the trig coefficient to trigCoeff
void Node::setTrigCoeff(int trigCoeff)
{
    trigCoefficient = trigCoeff;
}//setTrigCoefficient

//sets if the term is negative to neg
void Node::setIsNeg(bool neg)
{
    isNeg = neg;
}//setIsNeg

//sets the numerator to num
void Node::setNum(int num)
{
    numerator = num;
}//setNumerator

//sets the denominator to den
void Node::setDen(int den)
{
    denominator = den;
}//setDenominator

//sets trig numerator to tnum
void Node::setTrigNum(int tnum)
{
    trigNumerator = tnum;
}//setTrigNumerator

//sets trig numerator to tden
void Node::setTrigDen(int tden)
{
    trigDenominator = tden;
}//setTrigDenominator

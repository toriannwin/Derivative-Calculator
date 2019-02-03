/*
    Tori Windrich
    11/15/2017
    Project 4: Derivative Calculator
*/

#ifndef NODE_H
#define NODE_H
#include <iostream>
#include <string>

class Node
{
    private:
        int coefficient; //term coefficient
        int numerator; //term numerator
        int denominator; //term denominator
        int exponent; //term exponent
        std::string trigIdentifier; //term trig identifier
        int trigCoefficient; //trig coefficient
        int trigNumerator; //trig numerator
        int trigDenominator; //trig denominator
        bool isNeg; //if the term is negative
        Node* nextPtr; //next value (for the linked list)
    public:
        //overloaded constructor (every value initialized to zero/false/null by default
        Node(int coeff = 0, int exp = 0, std::string trig = "", int trigC = 0, bool neg = false, int num = 0, int den = 0, int tnum = 0, int tden = 0);
        //returns the term coefficient
        int getCoeff() const { return coefficient; }
        //returns the term exponent
        int getExp() const { return exponent; }
        //returns the trig identifier
        std::string getTrigId() const { return trigIdentifier; }
        //returns the trig coefficient
        int getTrigCoeff() const { return trigCoefficient; }
        //returns if the term is negative
        bool getIsNeg() const { return isNeg; }
        //returns the term's numerator
        int getNumerator() const { return numerator; }
        //returns the term's denominator
        int getDenominator() const { return denominator; }
        //returns the trig's numerator
        int getTrigNum() const { return trigNumerator; }
        //returns the trig's denominator
        int getTrigDen() const { return trigDenominator; }
        //returns the next pointer for the linked list
        Node* next() const { return nextPtr; }
        void snext(Node* changeNext); //sets the next value
        void setCoeff(int coeff); //sets the coefficient
        void setExp(int exp); //sets the exponent
        void setTrigId(std::string trig); //sets the trig identifier
        void setTrigCoeff(int trigCoeff); //sets the trig coefficient
        void setIsNeg(bool neg); //sets if the term is negative
        void setNum(int num); //sets the numerator
        void setDen(int den); //sets the denominator
        void setTrigNum(int tnum); //sets the trig numerator
        void setTrigDen(int tden); //sets the trig denominator
};

#endif // NODE_H

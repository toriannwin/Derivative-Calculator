/*
    Tori Windrich
    11/15/2017
    Project 4: Derivative Calculator
*/

#include <iostream>
#include "Node.h"
#include "LinkedList.h"
#include <string>
#include <cctype>
#include <fstream>

using namespace std;

//prototypes
int getNumFunctions(ifstream&);
int GCF(int,int);
void createNewList(string, LinkedList*&);
void takeDerivative(LinkedList*&);
void simpleExpression(Node*);
void trigExpression(Node*);
void trigFractionExpression(Node*);
void fractionExpression(Node*);
void writeToFile(LinkedList*&, ofstream&);

//main function
int main()
{
    //creates and opens the functions.txt file, error message if it doesn't exist
    ifstream functions;
    functions.open("functions.txt");
    if (functions.fail())
    {
        cout << "The file functions.txt does not exist in this path.\n";
        return 1;
    }//functions.txt doesn't exist in this path
    //finds the number of functions
    int numFuncs = getNumFunctions(functions);
    //creates and opens the derive.txt file (this is where output will go)
    ofstream derivatives;
    derivatives.open("derive.txt");
    //creates a new LinkedList in termList
    LinkedList *termList = new LinkedList();
    string functionExp; //string to hold the function from the file

    //goes through each function in the file one at a time
    for(int i = 0; i < numFuncs; i++)
    {
        //gets the next function
        getline(functions,functionExp);
        //calls create new list function
        createNewList(functionExp,termList);
        //takes the derivative of the function
        takeDerivative(termList);
        //writes the derived function to derive.txt
        writeToFile(termList,derivatives);
        //deletes the list and then recreates it to save memory
        termList->~LinkedList();
        delete termList;
        termList = new LinkedList();
    }//going through every function
    //deletes the list
    termList->~LinkedList();
    delete termList;
    functions.close();
    derivatives.close();
    return 0;
}//main

//finds the greatest common factor between the numerator and denominator values
//to reduce the fractions
int GCF(int num, int den)
{
    int gcf = -1; //returns negative one if there is no GCF

    //for all the numbers less than (or equal to) both num and den
    for (int i = 1; i <= num && i <= den; i++)
    {
        //if both are divisible by i, set gcf equal to i
        if(num%i == 0 && den%i == 0)
            gcf = i;
    }//going through all the options for the greatest common factor
    return gcf;
}//GCD

//goes through the file and counts how many lines there are
int getNumFunctions(ifstream& file)
{
    string toss; //just for calling getline
    int num = 0; //the count
    while(!file.eof())
    {
        getline(file,toss); //gets the next line
        num++; //increments the count
    }//while there are still functions
    //seeks back to beginning of the file
    file.seekg(0,ios::beg);
    return num;
}//getNumFunctions

//creates a new list by parsing through the string space by space and erasing parts of it that are assigned to variables
//after each term in the string, creates a new node
void createNewList(string expression, LinkedList *&funcs)
{
    //values to be sent into node constructor
    int coefficient = 0, exponent = 0, trigCoeff = 0, numerator = 0, denominator = 0, trigNumerator = 0, trigDenominator = 0;
    string trigID = "";
    bool neg = false;

    //values to be used during the do while loop
    int findAlpha = 0;
    const char *alphas = "xcst+- ";
    const char *nonTerms = "+- ";
    bool foundX = false;
    int slashPos = 0, brackPos = 0, nonTerm = 0;
    Node *newNode = nullptr;

    //do while the expression string is not empty
    do
    {
        //sets all of the values back to zero/false/null
        coefficient = 0; exponent = 0; trigCoeff = 0; numerator = 0; denominator = 0; trigNumerator = 0; trigDenominator = 0;
        trigID = ""; neg = false; foundX = false; slashPos = 0; brackPos = 0; newNode = nullptr; findAlpha = 0; nonTerm = 0;

        //while there is a space, delete it
        while(expression[0] == ' ')
            expression.erase(0,1);
        //if the first character is a negative...
        if(expression[0] == '-')
        {
            //set the term to negative and erase the negative
            neg = true;
            expression.erase(0,1);
        }//negative
        //if the first character is a plus sign, delete it
        else if (expression[0] == '+')
            expression.erase(0,1);
        //while there is a space, delete it
        while(expression[0] == ' ')
            expression.erase(0,1);
        //if the first character is any alpha character, coefficient is 1
        if(isalpha(expression[0]))
        {
            coefficient = 1;
        }//coefficient is 1
        //if the coefficient isn't a fraction (it doesn't start with the bracket)
        else if(expression[0]!='(')
        {
            //find the first alpha character position
            findAlpha = expression.find_first_of(alphas);
            if (findAlpha != -1)
            {
                //if the first alpha character exists somewhere, set the coefficient equal to up to that position
                coefficient = atoi((expression.substr(0,findAlpha)).c_str());
                //erase it
                expression.erase(0,findAlpha);
            }//not at end of expression
            else
            {
                //if the first alpha character doesn't exist anywhere, set the coefficient equal to the rest of the expression
                coefficient = atoi((expression.substr(0)).c_str());
                //erase it
                expression.erase(0);
            }//at end of expression
        }//integer coefficient
        //if the coefficient is a fraction
        else if(expression[0]=='(')
        {
            //find where the / and the ) are in the fraction
            slashPos = expression.find_first_of("/");
            brackPos = expression.find_first_of(")");
            //numerator is everything from after ( to start of /
            numerator = atoi((expression.substr(1,slashPos)).c_str());
            //denominator is everything from after / to start of )
            denominator = atoi((expression.substr(slashPos+1,brackPos)).c_str());

            findAlpha = expression.find_first_of(alphas);
            //if an alpha value still exists somewhere, just delete up to the ), otherwise, delete the rest of the expression
            if (findAlpha != -1)
                expression.erase(0,brackPos+1);
            else
                expression.erase(0);
        }//fraction coefficient
        //if the expression isn't already empty
        if(expression!="")
        {
            //if the start of the expression is x
            if(expression[0] == 'x')
            {
                //set foundX equal to true and erase x
                foundX = true;
                expression.erase(0,1);
                //we know the exponent is at least one, so set it here in case this is the end
                exponent = 1;
            }//if the first was x
            //if the expression is still not empty
            if(expression!="")
            {
                if(expression[0] == '^')
                {
                    //if the next thing is a ^ (meaning that an exponent is next) erase the ^
                    expression.erase(0,1);
                    if (expression[0] == '-')
                    {
                        //if the exponent is negative, erase it
                        expression.erase(0,1);
                        findAlpha = expression.find_first_of(alphas);
                        //find out if an alpha character exists anywhere else
                        if (findAlpha!=-1)
                        {
                            //if so, the exponent is from the start of expression to the next non term (+,- or space)
                            nonTerm = expression.find_first_of(nonTerms);
                            exponent = atoi((expression.substr(0,nonTerm+1)).c_str());
                            //erase it!
                            expression.erase(0,nonTerm);
                        }//this isn't the last term
                        else
                        {
                            //if not, exponent is the rest of the expression
                            exponent = atoi((expression.substr(0)).c_str());
                            //erase the exponent
                            expression.erase(0);
                        }//this is the last term

                        //make the exponent negative
                        exponent *= -1;
                    }//exponent is negative
                    else
                    {
                        //if the exponent is positive
                        //find out if it's the last term or not by finding the next instance of alphas
                        findAlpha = expression.find_first_of(alphas);
                        if (findAlpha!=-1)
                        {
                            //if it isn't the last term, the exponent is from start of expression to the next non term
                            nonTerm = expression.find_first_of(nonTerms);
                            exponent = atoi((expression.substr(0,nonTerm+1)).c_str());
                            //erase the exponent
                            expression.erase(0,nonTerm);
                        }//this isn't the last one
                        else
                        {
                            //if it is the last term, exponent is the rest of the expression
                            exponent = atoi((expression.substr(0)).c_str());
                            //erase the exponent
                            expression.erase(0);
                        }//this is the last one
                    }//exponent is positive
                }//there is an exponent
                //whatever is next in the expression isn't an alphabetical character (and it wasn't a number of a fraction)
                else if(!isalpha(expression[0]))
                {
                    //if there was an x, the exponent is one, otherwise it was a constant so the exponent is 0
                    if(foundX)
                        exponent = 1;
                    else
                        exponent = 0;
                }//end of term
                //if the start of the expression is an alphabetical character (aka it's a trig identifier)
                else if(isalpha(expression[0]))
                {
                    //get the trig ID from start of expression to the next space
                    trigID = expression.substr(0,expression.find_first_of(" ")+1);
                    //delete the trig ID, including the space
                    expression.erase(0,expression.find_first_of(" ")+1);

                    //if the trig coefficient is an integer
                    if (expression[0] == '-' || isdigit(expression[0]))
                    {
                        //set the trig coefficient to beginning of expression to the x
                        trigCoeff = atoi((expression.substr(0,expression.find_first_of("x"))).c_str());
                    }//trigCoeff listed as integer
                    //if the trig coefficient is a fraction
                    else if (ispunct(expression[0]))
                    {
                        //find positions of the / and ) in the fraction
                        slashPos = expression.find_first_of("/");
                        brackPos = expression.find_first_of(")");
                        //trig numerator is from after ( to before /
                        trigNumerator = atoi((expression.substr(1,slashPos)).c_str());
                        //trig denominator is from after / to before )
                        trigDenominator = atoi((expression.substr(slashPos+1,brackPos)).c_str());
                    }//trigCoeff is fraction
                    //the next character is just x
                    else
                    {
                        //trig coefficient is 1
                        trigCoeff = 1;
                    }//trigCoeff not listed
                    //erase whatever was inside the trig function
                    expression.erase(0,expression.find_first_of("x")+1);
                }//trig expression
            }//expression is STILL not empty
        }//expression is still not empty

        //create a new node with the determined values
        newNode = new Node(coefficient, exponent, trigID, trigCoeff, neg, numerator, denominator, trigNumerator, trigDenominator);
        //add the node to the list
        funcs->addNodeInOrder(newNode);
        delete newNode;
    }while(expression.length() != 0);
}//createNewList

//takes the derivative of the entire function as a list
void takeDerivative(LinkedList *&funcs)
{
    int position = 0; //to track the position in the list
    //get the current node (basically the first one)
    Node *currentNode = funcs->getNode(position);

    //do while the current node exists
    do
    {
        //increment position for later
        position++;
        //if the current node is a trig term
        if (currentNode->getTrigId() != "")
        {
            //if the trig term has a fraction coefficient
            if (currentNode->getNumerator() != 0)
            {
                trigFractionExpression(currentNode);
            }//there is a fraction coefficient
            else
            {
                //if the trig term has an integer coefficient
                trigExpression(currentNode);
            }//integer coefficient
        }//this is a trig term
        else if (currentNode->getNumerator() == 0)
        {
            //if the coefficient is an integer
            simpleExpression(currentNode);
        }//integer coefficient
        else
        {
            //if the coefficient is a fraction
            fractionExpression(currentNode);
        }//fraction coefficient
        //get the next node
        currentNode = funcs->getNode(position);
    }while(currentNode);
}//takeDerivative

//gets the derivative for a simple expression (no trig and no fraction)
void simpleExpression(Node* term)
{
    //set the coefficient equal to the coefficient times the exponent
    term->setCoeff((term->getCoeff())*(term->getExp()));
    //decrease the exponent minus one
    term->setExp((term->getExp())-1);
    if (term->getCoeff() < 0 && !term->getIsNeg())
    {
        term->setCoeff(term->getCoeff()*-1);
        term->setIsNeg(true);
    }//fixing the negative to true
    else if (term->getCoeff() < 0 && term->getIsNeg())
    {
        term->setCoeff(term->getCoeff()*-1);
        term->setIsNeg(false);
    }//fixing the negative to false
}//simpleExpression

//gets the derivative for a trig term
void trigExpression(Node* term)
{
    //if the trig term doesn't have a fraction inside it, set the coefficient equal to the current coefficient times the trig coefficient
    if(term->getTrigNum() == 0)
        term->setCoeff((term->getCoeff())*(term->getTrigCoeff()));
    else //the trig term has a fraction inside it
    {
        //set the term's numerator to the coefficient times the trig numerator
        term->setNum((term->getCoeff())*(term->getTrigNum()));
        //set the term's denominator to the trig denominator
        term->setDen((term->getTrigDen()));

    //*****************************FIXING THE NEGATIVE************************
        //if the numerator is now negative and the term was negative, make the numerator positive and set is neg to false
        if (((term->getNumerator()<0 && term->getDenominator()>0) || (term->getNumerator()>0 && term->getDenominator()<0))  && term->getIsNeg())
        {
            if(term->getNumerator()<0)
                term->setNum(term->getNumerator()*-1);
            else
                term->setDen(term->getDenominator()*-1);
            term->setIsNeg(false);
        }//fix is negative to false
        //if the numerator is negative and the term was positive, make the numerator positive and set is neg to true
        else if (((term->getNumerator()<0 && term->getDenominator()>0) || (term->getNumerator()>0 && term->getDenominator()<0)) && !term->getIsNeg())
        {
            if(term->getNumerator()<0)
                term->setNum(term->getNumerator()*-1);
            else
                term->setDen(term->getDenominator()*-1);
            term->setIsNeg(true);
        }//fix is negative to true
        //if the fraction is written as -/- it needs to be made into a positive fraction
        else if (term->getNumerator()<0 && term->getDenominator()<0)
        {
            term->setNum(term->getNumerator()*-1);
            term->setDen(term->getDenominator()*-1);
        }//fix fraction to be positive
    //*****************************FIXING THE NEGATIVE************************
        //set the coefficient to zero
        term->setCoeff(0);

        //if the fraction can be reduced to an integer, set the coefficient to that integer and nullify numerator and denominator
        if((term->getNumerator())%(term->getDenominator()) == 0)
        {
            term->setCoeff((term->getNumerator())/(term->getDenominator()));
            term->setNum(0);
            term->setDen(0);
        }//we have integer as coefficient
        else
        {
            //find out if the fraction numerator and denominator has a GCF, if so, reduce it by that factor
            int gcf = GCF(term->getNumerator(),term->getDenominator());
            if (gcf != -1)
            {
                //sets each number to itself divided by the GCF
                term->setNum((term->getNumerator())/gcf);
                term->setDen((term->getDenominator())/gcf);
            }//we can reduce
        }//maybe we can reduce the fraction
    }//there is a fraction as the trig coefficient

    //setting the trig IDs to the proper derivation form
    if(term->getTrigId()=="tan ")
        term->setTrigId("sec^2 "); //tan to sec^2
    else if (term->getTrigId()=="sin ")
        term->setTrigId("cos "); //sin to cos
    else
    {
        //if the trig id is cos, set it to sin and then flip the sign by changing whether the term is negative or not
        term->setTrigId("sin ");
        if(term->getIsNeg())
            term->setIsNeg(false);
        else
            term->setIsNeg(true);
    }//cosine function

}//trigExpression

//takes the derivative of a trig term that has a fraction coefficient
void trigFractionExpression(Node* term)
{
    //if there is an integer as the trig coefficient
    if (term->getTrigCoeff() != 0)
    {
        //set the numerator of the term to itself times the trig coefficient
        term->setNum((term->getNumerator())*(term->getTrigCoeff()));

    //*****************************FIXING THE NEGATIVE************************
        //if the numerator is now negative and the term was negative, make the numerator positive and set is neg to false
        if (((term->getNumerator()<0 && term->getDenominator()>0) || (term->getNumerator()>0 && term->getDenominator()<0))  && term->getIsNeg())
        {
            if(term->getNumerator()<0)
                term->setNum(term->getNumerator()*-1);
            else
                term->setDen(term->getDenominator()*-1);
            term->setIsNeg(false);
        }//fix is negative to false
        //if the numerator is negative and the term was positive, make the numerator positive and set is neg to true
        else if (((term->getNumerator()<0 && term->getDenominator()>0) || (term->getNumerator()>0 && term->getDenominator()<0)) && !term->getIsNeg())
        {
            if(term->getNumerator()<0)
                term->setNum(term->getNumerator()*-1);
            else
                term->setDen(term->getDenominator()*-1);
            term->setIsNeg(true);
        }//fix is negative to true
        //if the fraction is written as -/- it needs to be made into a positive fraction
        else if (term->getNumerator()<0 && term->getDenominator()<0)
        {
            term->setNum(term->getNumerator()*-1);
            term->setDen(term->getDenominator()*-1);
        }//fix fraction to be positive
    //*****************************FIXING THE NEGATIVE************************

        //if the fraction can be reduced to an integer, set the coefficient to that integer and nullify the numerator and denominator
        if((term->getNumerator())%(term->getDenominator()) == 0)
        {
            term->setCoeff((term->getNumerator())/(term->getDenominator()));
            term->setNum(0);
            term->setDen(0);
        }//we have integer as coefficient
        else
        {
            //see if the numerator and denominator have a GCF, if so, reduce it by that factor
            int gcf = GCF(term->getNumerator(),term->getDenominator());
            if (gcf != -1)
            {
                //sets numerator and denominator to themselves divided by the gcf
                term->setNum((term->getNumerator())/gcf);
                term->setDen((term->getDenominator())/gcf);
            }//we can reduce
        }//maybe we can reduce the fraction
    }//trig coefficient is an integer
    //we have a fraction as the trig coefficient
    else
    {
        //set the numerator to itself times the trig numerator, and set the denominator to itself times the trig denominator
        term->setNum((term->getNumerator())*(term->getTrigNum()));
        term->setDen((term->getDenominator())*(term->getTrigDen()));

    //*****************************FIXING THE NEGATIVE************************
        //if the numerator is now negative and the term was negative, make the numerator positive and set is neg to false
        if (((term->getNumerator()<0 && term->getDenominator()>0) || (term->getNumerator()>0 && term->getDenominator()<0))  && term->getIsNeg())
        {
            if(term->getNumerator()<0)
                term->setNum(term->getNumerator()*-1);
            else
                term->setDen(term->getDenominator()*-1);
            term->setIsNeg(false);
        }//fix is negative to false
        //if the numerator is negative and the term was positive, make the numerator positive and set is neg to true
        else if (((term->getNumerator()<0 && term->getDenominator()>0) || (term->getNumerator()>0 && term->getDenominator()<0)) && !term->getIsNeg())
        {
            if(term->getNumerator()<0)
                term->setNum(term->getNumerator()*-1);
            else
                term->setDen(term->getDenominator()*-1);
            term->setIsNeg(true);
        }//fix is negative to true
        //if the fraction is written as -/- it needs to be made into a positive fraction
        else if (term->getNumerator()<0 && term->getDenominator()<0)
        {
            term->setNum(term->getNumerator()*-1);
            term->setDen(term->getDenominator()*-1);
        }//fix fraction to be positive
    //*****************************FIXING THE NEGATIVE************************

        //if the fraction reduces to an integer, set the coefficient to the integer and nullify numerator and denominator
        if((term->getNumerator())%(term->getDenominator()) == 0)
        {
            term->setCoeff((term->getNumerator())/(term->getDenominator()));
            term->setNum(0);
            term->setDen(0);
        }//we have integer as coefficient
        else
        {
            //if the fraction has a gcf, reduce it by that factor
            int gcf = GCF(term->getNumerator(),term->getDenominator());
            if (gcf != -1)
            {
                //reducing numerator and denominator by the gcf
                term->setNum((term->getNumerator())/gcf);
                term->setDen((term->getDenominator())/gcf);
            }//we can reduce
        }//maybe we can reduce the fraction
    }//trig coefficient is a fraction

    //setting the trig id to the derived trig id
    if(term->getTrigId()=="tan ")
        term->setTrigId("sec^2 "); //tan is sec^2
    else if (term->getTrigId()=="sin ")
        term->setTrigId("cos "); //sin is cos
    else
    {
        //if the term is cos, change it to sin and flip the sign by flipping isNeg
        term->setTrigId("sin ");
        if(term->getIsNeg())
            term->setIsNeg(false);
        else
            term->setIsNeg(true);
    }//cosine

}//trigFractionExpression

//takes the derivative of a fraction expression with no trig identities
void fractionExpression(Node* term)
{
    //sets the numerator to the numerator times the exponent
    term->setNum((term->getNumerator())*(term->getExp()));
    if(term->getNumerator() == 0)
    {
        term->setDen(0);
        term->setCoeff(0);
        return;
    }//the exponent was zero initially

    //*****************************FIXING THE NEGATIVE************************
        //if the numerator is now negative and the term was negative, make the numerator positive and set is neg to false
        if (((term->getNumerator()<0 && term->getDenominator()>0) || (term->getNumerator()>0 && term->getDenominator()<0))  && term->getIsNeg())
        {
            if(term->getNumerator()<0)
                term->setNum(term->getNumerator()*-1);
            else
                term->setDen(term->getDenominator()*-1);
            term->setIsNeg(false);
        }//fix is negative to false
        //if the numerator is negative and the term was positive, make the numerator positive and set is neg to true
        else if (((term->getNumerator()<0 && term->getDenominator()>0) || (term->getNumerator()>0 && term->getDenominator()<0)) && !term->getIsNeg())
        {
            if(term->getNumerator()<0)
                term->setNum(term->getNumerator()*-1);
            else
                term->setDen(term->getDenominator()*-1);
            term->setIsNeg(true);
        }//fix is negative to true
        //if the fraction is written as -/- it needs to be made into a positive fraction
        else if (term->getNumerator()<0 && term->getDenominator()<0)
        {
            term->setNum(term->getNumerator()*-1);
            term->setDen(term->getDenominator()*-1);
        }//fix fraction to be positive
    //*****************************FIXING THE NEGATIVE************************

    //sets the exponent to itself minus one
    term->setExp((term->getExp())-1);

    //if the fraction can be reduced to an integer, set the coefficient equal to that integer and nullify the numerator and denominator
    if((term->getNumerator())%(term->getDenominator()) == 0)
    {
        term->setCoeff((term->getNumerator())/(term->getDenominator()));
        term->setNum(0);
        term->setDen(0);
    }//we have integer as coefficient
    else
    {
        //see if there is a greatest common factor between the numerator and denominator, if so, reduce by that factor
        int gcf = GCF(term->getNumerator(),term->getDenominator());
        if (gcf != -1)
        {
            //reduce the fraction by the gcf
            term->setNum((term->getNumerator())/gcf);
            term->setDen((term->getDenominator())/gcf);
        }//we can reduce
    }//maybe we can reduce the fraction
}//fractionExpression

//writes the current list to the file
void writeToFile(LinkedList*& funcs, ofstream& derivatives)
{
    Node *currentNode = nullptr; //node to go through the list

    //goes through the list until the length of the list
    for(int i = 0; i < funcs->getLength(); i++)
    {
        //gets the next term
        currentNode = funcs->getNode(i);
        //if the term is negative, write - to file, otherwise if there is a coefficient to write and
        //this isn't the first term in the expression, write + to file
        if (currentNode->getIsNeg() && (currentNode->getCoeff() != 0 || currentNode->getNumerator() != 0))
            derivatives << "- ";
        else if (i!=0 && (currentNode->getCoeff() > 0 || currentNode->getNumerator() > 0))
                derivatives << "+ ";
        //if there isn't a fraction as a coefficient
        if (currentNode->getNumerator() == 0)
        {
            //if there is a nonzero coefficient
            if (currentNode->getCoeff() != 0)
            {
                //write the coefficient if it isn't one OR it is one and x shouldn't appear
                if (currentNode->getCoeff()!=1)
                    derivatives << currentNode->getCoeff();
                else if (currentNode->getCoeff()==1 && currentNode->getExp()==0 && currentNode->getTrigId()=="")
                    derivatives << currentNode->getCoeff();
                //if there is a trig identity
                if (currentNode->getTrigId()!="")
                {
                    //write the trig id to the file
                    derivatives << currentNode->getTrigId();
                    //if there is a trig fraction
                    if(currentNode->getTrigNum() != 0)
                    {
                        //write the fraction to the file
                        derivatives << "(" << currentNode->getTrigNum() << "/" << currentNode->getTrigDen() << ")";
                    }//the trig coeff is a fraction
                    //there is just a trig coefficient
                    else
                    {
                        //write the trig coefficient to the file if it isn't 1
                        if (currentNode->getTrigCoeff()!=1)
                            derivatives << currentNode->getTrigCoeff();
                    }//the trig coeff is an integer
                    //write x to file
                    derivatives << "x ";
                }//there is a trig id
                //if there not a trig id and there is a nonzero exponent (if it's zero, x doesn't show up)
                else if(currentNode->getExp()!=0)
                {
                    //if the exponent isn't one, write x^ and the exponent to file, else just write x
                    if (currentNode->getExp()!=1)
                        derivatives << "x^" << currentNode->getExp() << " ";
                    else
                        derivatives << "x ";
                }//there is a nonzero exponent
            }//there is a value to write
        }//integer coefficient
        //if there is a fraction as a coefficient
        else
        {
            //write the fraction coefficent to the file
            derivatives << "(" << currentNode->getNumerator() << "/" << currentNode->getDenominator() << ")";
            //if there is a trig identity
            if (currentNode->getTrigId()!="")
                {
                    //write the trig id to the file
                    derivatives << currentNode->getTrigId();
                    //if there is a trig fraction
                    if(currentNode->getTrigNum() != 0)
                    {
                        //write the trig fraction to the file
                        derivatives << "(" << currentNode->getTrigNum() << "/" << currentNode->getTrigDen() << ")";
                    }//the trig coeff is a fraction
                    //if there is just a trig coefficient
                    else
                    {
                        //write the trig coefficient to the file if it's not equal to one
                        if (currentNode->getTrigCoeff()!=1)
                            derivatives << currentNode->getTrigCoeff();
                    }//the trig coeff is an integer
                    //write x to file
                    derivatives << "x ";
                }//there is a trig id
                //if there is a nonzero exponent (if it is zero, x will not display)
                else if(currentNode->getExp()!=0)
                {
                    //if exponent isn't 1 write x^ and the exponent to the file, else just write x
                    if (currentNode->getExp()!=1)
                        derivatives << "x^" << currentNode->getExp() << " ";
                    else
                        derivatives << "x ";
                }//there is a nonzero exponent
        }//fraction coefficient
    }//one node at a time
    //write an endl to the file
    derivatives << endl;
}//writeToFile

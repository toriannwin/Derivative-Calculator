/*
    Tori Windrich
    11/15/2017
    Project 4: Derivative Calculator
*/

#include "LinkedList.h"
#include "Node.h"
#include <iostream>

//constructor (default)
LinkedList::LinkedList()
{
    head = nullptr;
    ptr = nullptr;
}//default constructor

//constructor
LinkedList::LinkedList(Node *h)
{
    //assign passed in node to head, adds the
    //next nodes in the list to the new list,
    //points ptr to head and initializes
    //length to length of h by calculating the length
    head = new Node;
    *head = *h;
    //adds new nodes while they still exist
    while (h->next())
    {
        h = h->next();
        addNode(h);
    }//while the next node exists
    ptr = head;
    calcLength();
}//constructor

//destructor, calls the deleteList function
LinkedList::~LinkedList()
{
    deleteList();
}//destroys entire list

//deletes the entire list (recursive)
void LinkedList::deleteList(Node *nodePtr)
{
    //if head still exists
    if (head)
    {
        //store the next value in nodePtr, delete the head value
        //move head to nodePtr and call deleteList again
        nodePtr = head->next();
        delete head;
        head = nodePtr;
        deleteList(nullptr);
        length--;
    }//more to delete
}//deleteList

//get node at specific position (calls a recursive function to find the node and sets the class ptr value back to head)
Node* LinkedList::getNode(int position)
{
    //call the getNodeFull function to actually find the correct node
    Node *toReturn = getNodeFull(position);
    //set the pointer back to head
    setPtr();
    return toReturn;
}//gets node at desired position

//gets the node at the current position, using a counter that is 0 by default (recursive)
Node* LinkedList::getNodeFull(int position, int counter)
{
    //if the counter isn't yet equal to the position
    if(position != counter && ptr)
    {
        //advance ptr and call function again
        ptr = ptr->next();
        return getNodeFull(position,++counter);
    }//if position doesn't match the counter
    else
    {
        //return the node at the current pointer
        return ptr;
    }//if position matches
}//getNodeFull

//calculates the length of the list (recursive)
void LinkedList::calcLength(int counter)
{
    //if the current node isn't null, advance the counter and calc length again
    if (getNode(counter))
    {
        counter++;
        calcLength(counter);
    }//if current exists
    else
    {
        //once the current node is null, assign length to the counter
        length = counter;
    }//if current doesn't exist
}//calculates Length of list

//changes the head value to the passed in pointer
void LinkedList::changeHead(Node *newNode)
{
    //create pointer to the node (copying the node over) - create new private data
    Node *newNodePtr = new Node;
    *newNodePtr = *newNode;
    //set new pointer's next value equal to head, and set head equal to new pointer
    newNodePtr->snext(head);
    head = newNodePtr;
}//changes front of the list

//adds a node to the list based on the exponent (note: all trig functions will basically just be appended) (recursive)
void LinkedList::addNodeInOrder(Node *newNode, Node *nodePtr, Node *prevPtr, int counter)
{
    //if head doesn't yet exist, assigns newNode to the head
    if(!head)
    {
        //create pointer to the node (copying the node over) - create new private data
        Node *newNodePtr = new Node;
        *newNodePtr = *newNode;
        head = newNodePtr;
    }//not head
    //if the newNode is a trig function and therefore needs to be appended, call the normal Add function
    else if (newNode->getTrigId() != "")
    {
        addNode(newNode);
    }//trig function
    //if the newNode needs to go in front of the head because the exponent is bigger...
    else if (newNode->getExp() > head->getExp())
    {
        //calls changeHead method
        changeHead(newNode);
    }//new head
    //if this isn't the first pass through, and newNode needs to be inserted between two nodes...
    //exponent = 0 will all end up either at the end or between the normal terms and the trig terms
    else if (nodePtr && (newNode->getExp() > nodePtr->getExp() || nodePtr->getExp() == 0))
    {
        //create pointer to the node (copying the node over) - create new private data
        Node *newNodePtr = new Node;
        *newNodePtr = *newNode;
        //set the previous pointer's next value to new node and set new node's next value to nodePtr
        prevPtr->snext(newNodePtr);
        newNodePtr->snext(nodePtr);
    }//somewhere in the middle
    //if this is the first pass through...
    else if (counter == 0)
    {
        //nodePtr is set to head's next value, and prevPtr is set to head, and addNode is called again with incremented counter
        nodePtr = head->next();
        prevPtr = head;
        addNodeInOrder(newNode, nodePtr, prevPtr,1);
    }//nodePtr needs to be declared
    //if node pointer exists but nothing needs to be added yet
    else if (nodePtr)
    {
        //advance prevPtr and nodePtr, call addNode again with counter > 1 (we don't really care what the value is)
        prevPtr = nodePtr;
        nodePtr = nodePtr->next();
        addNodeInOrder(newNode, nodePtr, prevPtr,counter);
    }//nodePtr was already declared
    //if the new node needs to be added to the end of the list
    else
    {
        //create pointer to the node (copying the node over) - create new private data
        Node *newNodePtr = new Node;
        *newNodePtr = *newNode;
        //prevPtr's next value is set to the new node
        prevPtr->snext(newNodePtr);
    }//at the end
    //updates the length
    calcLength();
}//adds a node to the proper spot based on exponent

//adds the node at the end of the list (recursive)
void LinkedList::addNode(Node *newNode, Node *nodePtr, int counter)
{
    if (!head)
    {
        //copying the node to another pointer - create new private data
        Node *newNodePtr = new Node;
        *newNodePtr = *newNode;
        //assigning to head
        head = newNodePtr;
        length++;
        return;
    }//the list is empty
    if(counter == 0)
    {
        //increment the counter and set nodePtr equal to head
        counter++;
        nodePtr = head;
    }//if this is the first time through the addNode function
    if(nodePtr->next())
    {
        //note: we don't care what counter is after the first time through, because we just need
        //notePtr to be set to head, and we can't do that in the default declaration because head
        //isn't static, so on the first iteration it assigns nodePtr to head and increments the counter to keep
        //it from equaling zero later, we really only care if the nodePtr->next() value exists

        //increment nodePtr and call addNode again
        nodePtr = nodePtr->next();
        addNode(newNode,nodePtr,counter);
    }//the next value isn't null
    else
    {
        //create pointer to the node (copying the node over) - create new private data
        Node *newNodePtr = new Node;
        *newNodePtr = *newNode;
        //append node to the list
        nodePtr->snext(newNodePtr);
        length++;
    }//at the end of the list, time to add
    calcLength();
} //addNode

//deletes a node from indicated position in the list (recursive)
void LinkedList::deleteNode(int position, Node *prevPtr, int counter)
{
    calcLength();
    //if the position goes out of bounds
    if(position >= length)
    {
        std::cout << "The position is out of bounds and thus no data was deleted.\n";
        return;
    }//position too high
    //if we need to delete the head node
    if(position == 0)
    {
        prevPtr = head;
        head = head->next();
        delete prevPtr;
        return;
    }//we need to delete head
    if(counter==0)
    {
        prevPtr = head;
    }//prevPtr hasn't been set yet
    if(position!=counter+1)
    {
        counter++;
        prevPtr = prevPtr->next();
        deleteNode(position,prevPtr,counter);
    }//if we haven't found the position yet
    else
    {
        //get the node to be deleted
        Node *toDelete = prevPtr->next();
        //connect previous pointer to the pointer after the one to be deleted
        prevPtr->snext((prevPtr->next())->next());
        delete toDelete;
        calcLength();
    }//we are at the desired position
}//deletesNode

//prints the nodes of the list (the way it would be printed to the file, but to the console) (recursive)
void LinkedList::printList(int i)
{
    calcLength();
    if (i < length && head)
    {
        //gets the next term
        Node *currentNode = getNode(i);
        //if the term is negative, write - to file, otherwise if there is a coefficient to write and
        //this isn't the first term in the expression, write + to console
        if (currentNode->getIsNeg() && (currentNode->getCoeff() != 0 || currentNode->getNumerator() != 0))
            std::cout << "- ";
        else if (i!=0 && (currentNode->getCoeff() > 0 || currentNode->getNumerator() > 0))
                std::cout << "+ ";
        //if there isn't a fraction as a coefficient
        if (currentNode->getNumerator() == 0)
        {
            //if there is a nonzero coefficient
            if (currentNode->getCoeff() != 0)
            {
                //write the coefficient if it isn't one OR it is one and x shouldn't appear
                if (currentNode->getCoeff()!=1)
                    std::cout << currentNode->getCoeff();
                else if (currentNode->getCoeff()==1 && currentNode->getExp()==0)
                    std::cout << currentNode->getCoeff();
                //if there is a trig identity
                if (currentNode->getTrigId()!="")
                {
                    //write the trig id to the console
                    std::cout << currentNode->getTrigId();
                    //if there is a trig fraction
                    if(currentNode->getTrigNum() != 0)
                    {
                        //write the fraction to the console
                        std::cout << "(" << currentNode->getTrigNum() << "/" << currentNode->getTrigDen() << ")";
                    }//the trig coeff is a fraction
                    //there is just a trig coefficient
                    else
                    {
                        //write the trig coefficient to the file if it isn't 1
                        if (currentNode->getTrigCoeff()!=1)
                            std::cout << currentNode->getTrigCoeff();
                    }//the trig coeff is an integer
                    //write x to console
                    std::cout << "x ";
                }//there is a trig id
                //if there not a trig id and there is a nonzero exponent (if it's zero, x doesn't show up)
                else if(currentNode->getExp()!=0)
                {
                    //if the exponent isn't one, write x^ and the exponent to console, else just write x
                    if (currentNode->getExp()!=1)
                        std::cout << "x^" << currentNode->getExp() << " ";
                    else
                        std::cout << "x ";
                }//there is a nonzero exponent
            }//there is a value to write
        }//integer coefficient
        //if there is a fraction as a coefficient
        else
        {
            //write the fraction coefficent to the console
            std::cout << "(" << currentNode->getNumerator() << "/" << currentNode->getDenominator() << ")";
            //if there is a trig identity
            if (currentNode->getTrigId()!="")
                {
                    //write the trig id to the console
                    std::cout << currentNode->getTrigId();
                    //if there is a trig fraction
                    if(currentNode->getTrigNum() != 0)
                    {
                        //write the trig fraction to the console
                        std::cout << "(" << currentNode->getTrigNum() << "/" << currentNode->getTrigDen() << ")";
                    }//the trig coeff is a fraction
                    //if there is just a trig coefficient
                    else
                    {
                        //write the trig coefficient to the console if it's not equal to one
                        if (currentNode->getTrigCoeff()!=1)
                            std::cout << currentNode->getTrigCoeff();
                    }//the trig coeff is an integer
                    //write x to console
                    std::cout << "x ";
                }//there is a trig id
                //if there is a nonzero exponent (if it is zero, x will not display)
                else if(currentNode->getExp()!=0)
                {
                    //if exponent isn't 1 write x^ and the exponent to the console, else just write x
                    if (currentNode->getExp()!=1)
                        std::cout << "x^" << currentNode->getExp() << " ";
                    else
                        std::cout << "x ";
                }//there is a nonzero exponent
        }//fraction coefficient
        printList(++i);
    }//one node at a time
    //write an endl to the console
    std::cout << std::endl;
}//printList












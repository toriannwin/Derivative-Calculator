/*
    Tori Windrich
    11/15/2017
    Project 4: Derivative Calculator
*/

#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include "Node.h"
#include <iostream>

class LinkedList
{
    private:
        Node *head; //the lead Node for the list
        int length; //the length of the list
        Node *ptr; //pointer used for getting the next node
        //private function used within getNode to reset the pointer (recursive)
        Node* getNodeFull(int position, int counter = 0);
        //moves the pointer back to the beginning
        void setPtr() { ptr = head; }
    public:
        LinkedList(); //default constructor
        LinkedList(Node *h); //overloaded constructor
        virtual ~LinkedList(); //destructor
        //adds node at the end of the list (recursive)
        void addNode(Node *newNode, Node *nodePtr = nullptr, int counter = 0);
        //deletes a node from indicated position in the list (recursive)
        void deleteNode(int position, Node *prevPtr = nullptr, int counter = 0);
        //prints the nodes of the list (the way it would be printed to the file, but to the console) (recursive)
        void printList(int i = 0);
        //returns the head node
        const Node* getHead() { return head; }
        //calculates the new length (recursive)
        void calcLength(int counter = 0);
        //calculates and returns the length
        int getLength() { calcLength(); return length; }
        //changes the head pointer
        void changeHead(Node *newptr);
        //deletes the whole list (called by the destructor) (recursive)
        void deleteList(Node *nodePtr = nullptr);
        //adds a node to the list (in order of exponent) (recursive)
        void addNodeInOrder(Node *newNode, Node *nodePtr = nullptr, Node *prevPtr = nullptr, int counter = 0);
        //returns the node indicated by the position (calls getNodeFull which is recursive)
        Node* getNode(int position);

};

#endif // LINKEDLIST_H

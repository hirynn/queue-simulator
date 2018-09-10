#pragma once
#include <iostream>
#include "Service.h"
using namespace std;

class Queue
{
private:
	int endOfQueue;
	int startOfQueue;
	int size;
	Service *queueList; 
public:
	//initializes data
	void initialization(int); //note: used after default constructor to initialize data

	Queue();
	~Queue();

	//getters
	int getNumberOfItems();
	double getTotalServiceTime();

	//validators
	bool isEmpty();
	bool isFull();

	//queue functions
	bool enqueue(Service); //returns true if the operation succeeded
	Service dequeue(); //returns the service and decrements the queue position
	Service peek(); //peeks at the next service without dequeueing 
};


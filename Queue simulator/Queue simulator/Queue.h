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
	Service currentService;
	Service *queueList; 
public:
	void initialization(int);

	Queue();
	~Queue();

	//getters and setters
	Service getCurrentService();
	void setCurrentService(Service);
	int getNumberOfItems();
	double getTotalServiceTime();

	bool isEmpty();
	bool isFull();

	bool enqueue(Service); //returns true if the operation succeeded
	Service dequeue(); //returns the service and decrements the queue position
	Service peek(); //peeks at the next service without dequeueing 
};


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
	string currentService;
	Service *queueList; 
public:
	Queue(int);

	Queue();
	~Queue();

	//getters and setters
	string getCurrentService();
	void setCurrentService(string);

	bool isEmpty();
	bool isFull();

	bool enqueue(Service); //returns true if the operation succeeded
	Service dequeue(); //returns the service and decrements the queue position
};


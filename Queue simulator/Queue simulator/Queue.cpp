#include "stdafx.h"
#include "Queue.h"
#include "Service.h"

Queue::Queue()
{
	//not used
}

void Queue::initialization(int size)
{
	queueList = new Service[size];
	this->size = size;
	startOfQueue = 0;
	endOfQueue = 0;
}

Queue::~Queue()
{
	delete[] queueList;
}

//adds the job into the queue if the queue is not full
bool Queue::enqueue(Service service) 
{
	if (!isFull())
	{
		queueList[endOfQueue] = service;
		endOfQueue++;
		return true;
	}
	else
		return false;
}

//returns the service and decrements the queue position
Service Queue::dequeue()
{
	if (!isEmpty())
	{
		startOfQueue++; //oldest item in queue is now [current item] + 1
		return queueList[startOfQueue - 1]; //return the [current item]
	}
}

//peeks at the next service without dequeueing
Service Queue::peek()
{
	if (!isEmpty())
		return queueList[startOfQueue]; //return [next item] or [current item] + 1
}

int Queue::getNumberOfItems()
{
	return (endOfQueue - startOfQueue);
}

double Queue::getTotalServiceTime()
{
	double temp = 0;

	for (int i = 0; i < getNumberOfItems(); i++)
	{
		temp += queueList[i].getServiceTime();
	}

	return temp;
}

bool Queue::isEmpty()
{
	if (startOfQueue == endOfQueue)
		return true;
	else
		return false;
}

bool Queue::isFull()
{
	if (endOfQueue >= size)
		return true;
	else
		return false;
}
#include "stdafx.h"
#include "Queue.h"
#include "Service.h"

Queue::Queue()
{
	
}

Queue::Queue(int size)
{
	queueList = new Service[size];
	this->size = size;
	startOfQueue = 0;
	endOfQueue = 0;
}

Queue::~Queue()
{
	delete queueList;
	delete this;
}

bool Queue::enqueue(Service service) 
{
	if (!isFull())
	{
		this->queueList[endOfQueue] = service;
		endOfQueue++;
		return true;
	}
	else
		return false;
}

Service Queue::dequeue()
{
	if (!isEmpty())
	{
		startOfQueue++;
		return queueList[startOfQueue - 1];
	}
}

string Queue::getCurrentService()
{
	return currentService;
}

//maybe I'll need it idk
void Queue::setCurrentService(string currentService) 
{
	this->currentService = currentService;
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
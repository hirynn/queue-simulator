#include "stdafx.h"
#include "Server.h"
#include "Service.h"
#include "Queue.h"
#include <iostream>
using namespace std;

Server::Server()
{
	currentJobID = "";
	time = 0;
}

Server::~Server()
{
	delete this;
}

string Server::getCurrentJobID()
{
	return currentJobID;
}

void Server::setCurrentJobID(string currentJobID)
{
	this->currentJobID = currentJobID;
}

void Server::adjustTime(Service job, Queue queue)
{
	//TODO: add conditions for time, set job to "" if there is a window in arrival time and service time  
	if (isEmpty()) //if there is no job now 
	{
		//we wait until the job arrives
		time += job.getArrivalTime();
	}
	else if (!isEmpty())//if there is a job now 
	{
		//we add the time needed to service them 
		time += job.getServiceTime();
	}
}

bool Server::isEmpty()
{
	if (currentJobID == "")
		return true;
	else
		return false;
}

//set when there is no job 
void Server::setEmpty()
{
	currentJobID = "";
}
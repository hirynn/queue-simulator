#include "stdafx.h"
#include "Server.h"
#include "Service.h"
#include "Queue.h"
#include <iostream>
using namespace std;

Server::Server()
{
	time = 0;
	idleTime = 0;
	isBusy = false;
}

Server::~Server()
{

}

double Server::getTime()
{
	return time;
}

double Server::getIdleTime()
{
	return idleTime;
}

Service Server::getCurrentJob()
{
	return currentJob;
}

void Server::setCurrentJob(Service service)
{
	this->currentJob = service;
}

void Server::adjustTime(double extraTime)
{
	//if there is job wait time
	if (extraTime < 0)
		extraTime = 0;

	//this->time = (currentJob.getArrivalTime() + currentJob.getServiceTime() + extraTime);
	this->time = extraTime;
}

void Server::incrementIdleTime(double idleTime)
{
	this->idleTime += idleTime;
}

//set when there is no job
void Server::setFree()
{
	this->isBusy = false;
}

//set when there is a job 
void Server::setBusy()
{
	this->isBusy = true;
}
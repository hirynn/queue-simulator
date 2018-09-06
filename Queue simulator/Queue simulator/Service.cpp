#include "stdafx.h"
#include "Service.h"
#include <iostream>
using namespace std;

Service::Service()
{
	jobID = "";
	arrivalTime = 0;
	serviceTime = 0;
}

Service::Service(string jobID, double arrivalTime, double serviceTime)
{
	this->jobID = jobID;
	this->arrivalTime = arrivalTime;
	this->serviceTime = serviceTime;
}

Service::~Service()
{
	delete this;
}

string Service::getJobID()
{
	return jobID;
}

double Service::getArrivalTime()
{
	return arrivalTime;
}

double Service::getServiceTime()
{
	return serviceTime;
}

void Service::setJobID(string jobID)
{
	this->jobID = jobID;
}

void Service::setArrivalTime(double arrivalTime)
{
	this->arrivalTime = arrivalTime;
}

void Service::setServiceTime(double serviceTime)
{
	this->serviceTime = serviceTime;
}
#include "stdafx.h"
#include "Service.h"
#include <iostream>
using namespace std;

Service::Service()
{
	JobID = 0;
	arrivalTime = 0;
	serviceTime = 0;
}

Service::Service(double arrivalTime, double serviceTime)
{
	this->arrivalTime = arrivalTime;
	this->serviceTime = serviceTime;
}

Service::~Service()
{

}

int Service::getJobID()
{
	return JobID;
}

double Service::getArrivalTime()
{
	return arrivalTime;
}

double Service::getServiceTime()
{
	return serviceTime;
}

void Service::setJobID(int JobID)
{
	this->JobID = JobID;
}

void Service::setArrivalTime(double arrivalTime)
{
	this->arrivalTime = arrivalTime;
}

void Service::setServiceTime(double serviceTime)
{
	this->serviceTime = serviceTime;
}
#include "stdafx.h"
#include "Service.h"
#include <iostream>
using namespace std;

Service::Service()
{
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

double Service::getArrivalTime()
{
	return arrivalTime;
}

double Service::getServiceTime()
{
	return serviceTime;
}

void Service::setArrivalTime(double arrivalTime)
{
	this->arrivalTime = arrivalTime;
}

void Service::setServiceTime(double serviceTime)
{
	this->serviceTime = serviceTime;
}
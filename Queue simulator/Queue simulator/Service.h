#pragma once
#include <iostream>
using namespace std;

class Service
{
private:
	double arrivalTime;
	double serviceTime;

public:
	//non-default constructor
	Service(double, double); //not used

	//getters
	double getArrivalTime();
	double getServiceTime();

	//setters
	void setArrivalTime(double);
	void setServiceTime(double);

	//constructor 
	Service(); //using default constructor here

	//destructor
	~Service();
};


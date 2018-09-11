#pragma once
#include <iostream>
using namespace std;

class Service
{
private:
	int JobID;
	double arrivalTime;
	double serviceTime;

public:
	//non-default constructor
	Service(double, double); //not used

	//getters
	int getJobID();
	double getArrivalTime();
	double getServiceTime();

	//setters
	void setJobID(int);
	void setArrivalTime(double);
	void setServiceTime(double);

	//constructor 
	Service(); //using default constructor here

	//destructor
	~Service();
};


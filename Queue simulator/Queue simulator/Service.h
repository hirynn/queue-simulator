#pragma once
#include <iostream>
using namespace std;

class Service
{
private:
	string jobID;
	double arrivalTime;
	double serviceTime;

public:
	//non-default constructor
	Service(string, double, double);

	//getters
	string getJobID();
	double getArrivalTime();
	double getServiceTime();

	//setters
	void setJobID(string);
	void setArrivalTime(double);
	void setServiceTime(double);

	//constructor 
	Service();

	//destructor
	~Service();
};


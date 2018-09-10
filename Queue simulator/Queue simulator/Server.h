#pragma once
#include <iostream>
#include "Service.h"
#include "Queue.h"
using namespace std;

class Server
{
private:
	Service currentJob;
	double time;
	double idleTime;

public:
	Server(); //using default constructor here
	~Server();

	//validators
	bool isBusy;

	//getters
	double getTime();
	Service getCurrentJob();
	double getIdleTime();

	//setters
	void setCurrentJob(Service);
	void setBusy();
	void setFree();
	
	//functions to act on data
	void incrementIdleTime(double);
	void adjustTime(double);

};


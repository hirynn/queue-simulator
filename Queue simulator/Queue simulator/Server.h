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
	bool isBusy;

	double getTime();
	Service getCurrentJob();
	double getIdleTime();
	void setCurrentJob(Service);
	void setBusy();
	void setFree();
	
	void incrementIdleTime(double);
	void adjustTime(double);

	Server(); //using default constructor here
	~Server(); //TODO: double check implementation
};


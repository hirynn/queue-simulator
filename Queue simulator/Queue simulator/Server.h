#pragma once
#include <iostream>
#include "Service.h"
#include "Queue.h"
using namespace std;

class Server
{
private:
	string currentJobID;
	double time;

public:
	string getCurrentJobID();
	void setCurrentJobID(string);
	void setEmpty();

	bool isEmpty();
	
	void adjustTime(Service, Queue);

	Server(); //using default constructor here
	~Server();
};


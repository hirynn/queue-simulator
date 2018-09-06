#include "stdafx.h"
#include "Queue.h"
#include "Server.h"
#include "Service.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

int readNumOfService(); //reads number of jobs
void getJob(Server, Queue);
void getToWork(Server, Queue, Service);
double convertToDouble(string);

int main()
{
	Server server;
	Queue queue(readNumOfService());
	
	getJob(server, queue);

    return 0;
}

//reads number of jobs
int readNumOfService()
{
	ifstream file;
	string line;
	int lines = 0;

	file.open("service.txt", ifstream::in);

	if (file.good())
	{
		while (getline(file, line))
		{
			lines++;
		}
	}

	file.close();

	return lines;
}

void getJob(Server server, Queue queue)
{
	ifstream file;
	string line;
	int counter = 0;
	Service service;
	file.open("service.txt", ifstream::in);

	if (file.good())
	{
		while (getline(file, line))
		{
			int found = line.find(',');
			//getting arrival time
			service.setArrivalTime(convertToDouble(line.substr(0, found)));
			
			//getting service time
			service.setServiceTime(convertToDouble(line.substr(found + 1, line.find(','))));

			getToWork(server, queue, service);
		}
	}

	file.close();
}

void getToWork(Server server, Queue queue, Service service)
{
	if (server.isEmpty()) //if server is empty
	{
		server.setCurrentJobID(service.getJobID()); //set current job to server
		server.adjustTime(service, queue); //calculate time and serve
	}
	else if (!server.isEmpty()) //if server is not empty
		queue.enqueue(service); //enqueue the job
}

double convertToDouble(string input)
{
	double data;

	stringstream convert(input);

	convert >> data;

	return data;
}
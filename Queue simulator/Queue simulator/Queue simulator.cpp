#include "stdafx.h"
#include "Queue.h"
#include "Server.h"
#include "Service.h"
#include <iostream> //cin, cout
#include <fstream> //read/write to file
#include <sstream> //to convert from string to double
#include <regex> //for data validation
using namespace std;

int NUM_OF_SERVERS; //TODO: replace so it can be changed dynamically
int MAX_NUM_LINES;
double waitTime = 0;

int readNumOfServers();
int readNumOfService();
bool getJob(Service &, int);
void serveJobs(Queue);
void writeToFile();

//validation functions
int findEmptyServer();
int findEarliestServer();
bool isAllServerFree();

//utility functions
void validateData(string, int &);
double convertToDouble(string);

//Server server[NUM_OF_SERVERS];
Server *server;

int main()
{
	NUM_OF_SERVERS = readNumOfServers();
	MAX_NUM_LINES = readNumOfService();

	server = new Server[NUM_OF_SERVERS];
	Queue queue(MAX_NUM_LINES);
	
	serveJobs(queue);
	writeToFile();

	delete[] server;

    return 0;
}

//reads number of servers from the user
int readNumOfServers()
{
	string validator;
	int num;
	cout << "Please enter the number of servers: ";
	getline(cin, validator);
	validateData(validator, num);

	return num;
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

//get and return only one job from text file
bool getJob(Service &service, int lineNum)
{
	ifstream file;
	string line;
	file.open("service.txt", ifstream::in);

	if (file.good())
	{
		//skip N lines
		for (int i = 0; i < lineNum; i++)
		{
			getline(file, line);
		}

		//if read operation succeeds
		if (getline(file, line))
		{
			int found = line.find(',');
			//getting arrival time
			service.setArrivalTime(convertToDouble(line.substr(0, found)));

			//getting service time
			service.setServiceTime(convertToDouble(line.substr(found + 1, line.find(','))));

			file.close();
			return true;
		}

	}
	
	file.close();

	return false;
}

//returns index of ANY free/empty server, else return -1 if all servers are busy
int findEmptyServer()
{
	for (int i = 0; i < NUM_OF_SERVERS; i++)
	{
		if (!server[i].isBusy)
			return i;
	}
	
	//if there is no empty servers
	return -1; 
}

//returns index of the EARLIEST AVAILABLE server, else return -1 if all servers are available
int findEarliestServer()
{
	double minServiceTime = 999; //dummy value for the sake of comparison
	int index; 
	double compareTime;

	for (int i = 0; i < NUM_OF_SERVERS; i++)
	{
		if (server[i].isBusy) //if server has a job
		{
			compareTime = server[i].getTime() + server[i].getCurrentJob().getArrivalTime() + server[i].getCurrentJob().getServiceTime();
			if (compareTime < minServiceTime)
			{
				minServiceTime = compareTime;
				index = i;
			}
		}
	}

	if (isAllServerFree())
		index = -1;
	
	return index;
}

//returns true if all servers are free/not busy, else return false if one server is unavailable
bool isAllServerFree()
{
	for (int i = 0; i < NUM_OF_SERVERS; i++)
	{
		if (server[i].isBusy)
			return false;
	}

	return true;
}

void serveJobs(Queue queue)
{
	int hasWaitTime = -1, counter = 0;
	double tempWaitTime = 0;
	bool hasJob; //checks if there is a job
	int index; //index for server
	int lineNum = 0; //keeps track of which job to get
	double currentJobStartTime, currentJobEndTime, nextJobStartTime = 0, currentServerTime, idleTime;

	Service service;
	do
	{
		//read one job from file
		if (lineNum <= MAX_NUM_LINES) //while there is a job in the text file for us to read 
		{
			hasJob = getJob(service, lineNum);
			lineNum++;
		}

		if (hasJob && (findEmptyServer() == -1 || (!queue.isEmpty() && !isAllServerFree()))) //if there is a job from the text file
			queue.enqueue(service); //we enqueue the job

		if (findEmptyServer() == -1) //if all servers are busy
			index = findEarliestServer(); //find the earliest server
		else if (findEmptyServer() != -1 && !hasJob && queue.isEmpty()) //to finish up any remaining jobs
			index = findEarliestServer();
		else //if there is an empty server
			index = findEmptyServer(); //find ANY ONE server that isn't busy

		//calculating times
		currentServerTime = server[index].getTime();
		currentJobStartTime = server[index].getCurrentJob().getArrivalTime();
		currentJobEndTime = currentJobStartTime + server[index].getCurrentJob().getServiceTime();
		if (!queue.isEmpty())
		{
			nextJobStartTime = queue.peek().getArrivalTime();
			if (nextJobStartTime < currentJobEndTime) //if next job arrives before current job
			{
				nextJobStartTime = currentJobEndTime; //next job should start right after current one ends
			}
		}
		//append wait time (if there is) to the job end time, this block of code runs twice
		if (hasWaitTime == index)
		{
			counter++;
			currentJobEndTime += tempWaitTime;
			if (counter == 2)
				hasWaitTime = -1;
		}

		//if the server is busy, process the job and set status to free
		if (server[index].isBusy)
		{
			//if there is a job in queue
			if (!queue.isEmpty())
			{
				//if the next job arrived earlier than the current job finishing (job wait time)
				if (nextJobStartTime <= currentJobEndTime)
				{
					server[index].adjustTime(nextJobStartTime);
				} //if the next job arrived later than the current job (server idle time)
				else if (nextJobStartTime > currentJobEndTime) 
				{
					server[index].adjustTime(nextJobStartTime);
				}
			}
			else
				server[index].adjustTime(currentJobEndTime);


			server[index].setFree();
		}
		//if the server is free/not busy, assign it a job and set status to busy
		else if (!server[index].isBusy)
		{
			if (!queue.isEmpty())
			{
				//if the next job in the queue arrives earlier than when the server finishes the current job 
				if (queue.peek().getArrivalTime() <= currentJobEndTime)
				{
					//we calculate wait time
					waitTime += currentJobEndTime - queue.peek().getArrivalTime();
					tempWaitTime = currentJobEndTime - queue.peek().getArrivalTime();
					hasWaitTime = index;
				}
				else if (queue.peek().getArrivalTime() > currentJobEndTime)
				{
					//if the next job arrived later than the current job finishing (server idle time)
					//calculate idle time
					idleTime = queue.peek().getArrivalTime() - currentJobEndTime;// - tempWaitTime;
					server[index].incrementIdleTime(idleTime);
				}
			}


			if (!queue.isEmpty()) //if there is a job in queue 
				server[index].setCurrentJob(queue.dequeue()); //give it the job we read from the text file
			else if (queue.isEmpty()) //if there are no jobs in queue (e.g. when reading from file)
				server[index].setCurrentJob(service); //give the job we just read from the text file

			server[index].setBusy();
		}
	} while (!queue.isEmpty() || !isAllServerFree());
}

void writeToFile()
{
	ofstream file;

	file.open("results.txt", ofstream::out | ofstream::app);

	for (int i = 0; i < NUM_OF_SERVERS; i++)
	{
		file << "Server " << i + 1 << "'s idle time is: " << server[i].getIdleTime() << endl;
	}
	file << "Wait time is " << waitTime << endl;
	file << "Average wait time is " << (waitTime / (double)MAX_NUM_LINES) << endl;

	file.close();
}

//utility function to validate user input
void validateData(string input, int &data)
{
	while (!(regex_match(input, regex("[0-9]+"))))
	{
		cout << "You have entered an invalid number of servers.\nPlease reenter the number of servers: ";
		getline(cin, input);
	}

	stringstream convert(input);

	convert >> data;
}

//utility function to convert string and return double
double convertToDouble(string input)
{
	double data;

	stringstream convert(input);

	convert >> data;

	return data;
}
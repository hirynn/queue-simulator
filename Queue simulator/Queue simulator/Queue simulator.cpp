#include "stdafx.h"
#include "Queue.h"
#include "Server.h"
#include "Service.h"
#include <iostream> //cin, cout
#include <fstream> //read/write to file
#include <sstream> //to convert from string to double
#include <regex> //for data validation
#include <iomanip> //for data display

#define FORMAT_TITLE << left << setw(18) << setfill(' ') <<
using namespace std;

int NUM_OF_SERVERS;
int MAX_NUM_LINES;
double waitTime = 0;

//main driver functions
string getFilename();
int readNumOfServers();
int readNumOfService(string);
bool getJob(Service &, int, string);
void serveJobs(string, bool);
void writeToFile(bool);
void writeToLog(int, bool, double, double);
void printOutput(bool);

//validation functions
int findEmptyServer();
int findEarliestServer();
int findEarliestQueue();
bool isAllServerFree();
bool isAllQueueEmpty(bool);
bool fileExists(string);

//utility functions
void validateData(string &, string);
int convertToInt(string);
double convertToDouble(string);

Server *server;
Queue *queue;

int main()
{
	string filename, input;
	bool hasRunOnce = false;
	do
	{
		if (hasRunOnce)
		{
			cout << "Do you want to change the file? (y/n)\n->";
			getline(cin, input);
			validateData(input, "continue");
		}
		else
		{
			do
			{
				filename = getFilename();
			} while (!fileExists(filename));
		}

		if (input == "y")
		{
			filename = getFilename();
		} while (!fileExists(filename));

		cout << "\n1. One queue for a number of servers.\n" << "2. One queue for each server.\n" << "Please select the queueing strategy to use: ";
		getline(cin, input);
		validateData(input, "strategy");

		NUM_OF_SERVERS = readNumOfServers();
		MAX_NUM_LINES = readNumOfService(filename);

		if (convertToInt(input) == 1) //queueing strategy 1
		{
			server = new Server[NUM_OF_SERVERS];
			queue = new Queue[1];

			queue->initialization(MAX_NUM_LINES);

			serveJobs(filename, true);

			cout << "\nCalculation complete.\n";
			cout << "Do you want to print output on the console? (y/n): ";
			getline(cin, input);
			validateData(input, "continue");

			if (input == "y")
				printOutput(true);

			system("del tempLog.txt");

			writeToFile(true);
			cout << "\nThe summarized results have been added to a text file \"results.txt\".\nThe text file can be used to compare between two strategies." << endl;
		}
		else if (convertToInt(input) == 2) //queueing strategy 2
		{
			server = new Server[NUM_OF_SERVERS];
			queue = new Queue[NUM_OF_SERVERS]; //one queue for each server

			for (int i = 0; i < NUM_OF_SERVERS; i++)
			{
				(queue + i)->initialization(MAX_NUM_LINES);
			}

			serveJobs(filename, false);

			cout << "\nCalculation complete.";
			cout << "\nDo you want to print output on the console? (y/n): ";
			getline(cin, input);
			validateData(input, "continue");

			if (input == "y")
				printOutput(true);

			system("del tempLog.txt");

			writeToFile(false);
			cout << "\nThe summarized results have been added to a text file \"results.txt\".\nThe text file can be used to compare between two strategies." << endl;
		}

		delete[] server;
		delete[] queue;

		cout << "Write to file successful.\n" << endl;

		cout << "Do you want to continue? (y/n)\n->";
		getline(cin, input);
		validateData(input, "continue");

		hasRunOnce = true;
	} while (input == "y");

    return 0;
}

//get filename from user
string getFilename()
{
	string name;

	cout << "Please enter the file name of text file containing the jobs.\nOtherwise, enter nothing and the sample \"service.txt\" will be opened.\nFilename->";
	getline(cin, name);

	if (name == "")
	{
		name = "service.txt";
		cout << "Program will open file " << name << "." << endl;
	}


	//if user has not entered the .txt behind, append it
	if (name.length() >= 4) //4 characters for ".txt", = because it is possible for the filename to be ".txt"
	{
		if ((name.compare(name.length() - 4, 4, ".txt")) != 0) //if name does not end in ".txt"
			name += ".txt";
	}
	else //the filename has less than 4 characters, meaning it does not end in ".txt"
		name += ".txt";

	return name;
}

//reads number of servers from the user
int readNumOfServers()
{
	string validator;
	int num;
	cout << "Please enter the number of servers: ";
	getline(cin, validator);
	validateData(validator, "servers");
	num = convertToInt(validator);

	return num;
}

//reads number of jobs
int readNumOfService(string filename = "service.txt")
{
	ifstream file;
	string line;
	int lines = 0;

	file.open(filename, ifstream::in);

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
bool getJob(Service &service, int lineNum, string filename)
{
	ifstream file;
	string line;
	file.open(filename, ifstream::in);

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
			//setting job ID
			service.setJobID(++lineNum);

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

//returns index of the EARLIEST queue based on service time //TODO: change logic
int findEarliestQueue()
{
	int index = 0;
	int compare = queue[0].getNumberOfItems();

	for (int i = 0; i < NUM_OF_SERVERS; i++)
	{
		if (compare > queue[i].getNumberOfItems())
		{
			index = i;
			compare = queue[i].getNumberOfItems();
		}
	}

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

//returns true if all queues are empty, else return false if one server is unavailable
bool isAllQueueEmpty(bool isOnlyOneQueue)
{
	if (isOnlyOneQueue) //if queueing strategy 1
	{
		if (!queue[0].isEmpty())
			return false;
		else
			return true;
	}
	else //if more than one queue (queueing strategy 2)
	{
		for (int i = 0; i < NUM_OF_SERVERS; i++)
		{
			if (!queue[i].isEmpty()) //if not free
				return false;
		}
	}


	return true;
}

//returns true if file exists
bool fileExists(string filename)
{
	ifstream file; 

	file.open(filename, ifstream::in);

	if (!file.good())
	{
		cout << "The file is not found.\nPlease check if you have entered the correct filename.";
		file.close();
		return false;
	}

	file.close();
	return true;
}

void serveJobs(string filename, bool isOnlyOneQueue)
{	
	int queueIndex;
	double tempWaitTime = 0;
	bool hasJob; //checks if there is a job
	int index; //index for server
	int lineNum = 0; //keeps track of which job to get
	double currentJobStartTime, currentJobEndTime, currentServerTime, idleTime = 0;
	Service service;

	do
	{
		//read one job from file
		if (lineNum <= MAX_NUM_LINES) //while there is a job in the text file for us to read 
		{
			hasJob = getJob(service, lineNum, filename);
			lineNum++;
		}

		if (hasJob && (findEmptyServer() == -1 || (!isAllQueueEmpty(isOnlyOneQueue) && !isAllServerFree()))) //if there is a job from the text file
		{
			if (isOnlyOneQueue)
				queue[0].enqueue(service); //we enqueue the job
			else if (!isOnlyOneQueue)
				queue[findEarliestQueue()].enqueue(service);
		}

		if (findEmptyServer() == -1) //if all servers are busy
			index = findEarliestServer(); //find the earliest server
		else if (findEmptyServer() != -1 && !hasJob && isAllQueueEmpty(isOnlyOneQueue)) //to finish up any remaining jobs
			index = findEarliestServer();
		else //if there is an empty server
			index = findEmptyServer(); //find ANY ONE server that isn't busy

		if (isOnlyOneQueue)
			queueIndex = 0;
		else
			queueIndex = index;

		//if the server is free/not busy, assign it a job and set status to busy
		if (!server[index].isBusy)
		{
			if (!queue[queueIndex].isEmpty()) //if there is a job in queue 
				server[index].setCurrentJob(queue[queueIndex].dequeue()); //give it the job we read from the text file
			else if (queue[queueIndex].isEmpty()) //if there are no jobs in queue (e.g. when reading from file)
				server[index].setCurrentJob(service); //give the job we just read from the text file

			server[index].setBusy();
		}
		//if the server is busy, process the job and set status to free
		else if (server[index].isBusy)
		{
			//for logging purposes
			tempWaitTime = 0;
			idleTime = 0; 

			//calculating times
			currentServerTime = server[index].getTime();
			currentJobStartTime = server[index].getCurrentJob().getArrivalTime();
			if (currentJobStartTime <= currentServerTime) //if have job wait time
			{
				tempWaitTime = currentServerTime - currentJobStartTime;
				waitTime += tempWaitTime;
				currentJobStartTime = currentServerTime; //current job must start during or after current server time
			}
			else if (currentJobStartTime > currentServerTime) //if have server idle time
			{
				idleTime = currentJobStartTime - currentServerTime;
				server[index].incrementIdleTime(idleTime);
				currentServerTime = currentJobStartTime; 
			}
			currentJobEndTime = currentJobStartTime + server[index].getCurrentJob().getServiceTime();

			server[index].adjustTime(currentJobEndTime);

			server[index].setFree();

			writeToLog(index, isOnlyOneQueue, tempWaitTime, idleTime);
		}
		
	} while (!isAllQueueEmpty(isOnlyOneQueue) || !isAllServerFree());
}

void writeToLog(int index, bool isOnlyOneQueue, double tempWaitTime, double idleTime)
{
	ofstream file;

	file.open("tempLog.txt", ofstream::out | ofstream::app);
	int strategy;
	if (isOnlyOneQueue)
		strategy = 1;
	else
		strategy = 2;

	file << server[index].getCurrentJob().getJobID() << "," //job ID
		<< index + 1 << "," //server handling the job
		<< server[index].getCurrentJob().getArrivalTime() << "," //the arrival time of job
		<< server[index].getCurrentJob().getServiceTime() << "," //the service time of job
		<< server[index].getTime() << "," //current server time
		<< idleTime << "," //any server idle time 
		<< tempWaitTime << "," //any job wait time
		<< waitTime << "," //accumulated waiting time
		<< server[index].getIdleTime() << "," //accumulated idle time
		<< endl;


	file.close();
}

void printOutput(bool isOnlyOneQueue) 
{
	string line, data;
	int found = 0, start = 0, strategy;
	ifstream file;
	file.open("tempLog.txt", ifstream::in);

	if (isOnlyOneQueue)
		strategy = 1;
	else
		strategy = 2;

	cout << "\nQueueing strategy " << strategy << endl << "====================" << endl;
	cout 
		FORMAT_TITLE "Job ID"
		FORMAT_TITLE "Server"
		FORMAT_TITLE "Arrival time" 
		FORMAT_TITLE "Service time" 
		FORMAT_TITLE "Server time" 
		FORMAT_TITLE "Idle time" 
		FORMAT_TITLE "Wait time" 
		FORMAT_TITLE "Total wait time" 
		FORMAT_TITLE "Total idle time";
	cout << left << setw(162) << setfill('=') << "\n" << endl;

	if (file.good())
	{
		while (!file.eof())
		{
			for (int i = 0; i < 9; i++) //9 is the number of data
			{
				getline(file, line, ',');
				cout FORMAT_TITLE line;

				if (line == "\n")
					break;
			}
		}
	}

	file.close();
}

//function to write results to file
void writeToFile(bool isOnlyOneQueue)
{
	ofstream file;
	int strategy;
	if (isOnlyOneQueue)
		strategy = 1;
	else
		strategy = 2;

	file.open("results.txt", ofstream::out | ofstream::app);

	file << "Results for queueing strategy " << strategy << endl << "==================================" << endl;

	for (int i = 0; i < NUM_OF_SERVERS; i++)
	{
		file << "Server " << i + 1 << "'s idle time is: " << server[i].getIdleTime() << endl;
	}
	file << "Wait time is " << waitTime << endl;
	file << "Average wait time is " << (waitTime / (double)MAX_NUM_LINES) << endl << endl;

	file.close();

	//reseting wait time
	waitTime = 0;
}

//utility function to validate user input
void validateData(string &input, string type)
{
	if (type == "servers")
	{
		while (!(regex_match(input, regex("[0-9]+"))))
		{
			cout << "You have entered an invalid number of servers.\nPlease reenter the number of servers: ";
			getline(cin, input);
		}
	}
	else if (type == "continue")
	{
		while (!(regex_match(input, regex("(y)|(n)"))))
		{
			cout << "You have entered an invalid choice.\nPlease reenter your choice: ";
			getline(cin, input);
		}
	}
	else if (type == "strategy") //type of queueing strategy
	{
		while (!(regex_match(input, regex("(1)|(2)"))))
		{
			cout << "You have entered an invalid choice.\nPlease reenter your queueing strategy: ";
			getline(cin, input);
		}
	}
}

//utility function to convert string and return int
int convertToInt(string input)
{
	int data;

	stringstream convert(input);

	convert >> data;
	
	return data;
}

//utility function to convert string and return double
double convertToDouble(string input)
{
	double data;

	stringstream convert(input);

	convert >> data;

	return data;
}
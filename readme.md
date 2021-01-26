Your task for this assignment is to investigate some of the properties of queues and write programs
which simulate the queuing and service of a set of requests using TWO (2) different strategies.
Each simulation should start at time 0 and run until all customers have been served.

The 2 queueing strategies are as follows:
1. A single queue. Each server will take the next customer as soon as the server becomes available.
2. A queue for each server. Customers will choose the server with the shortest queue on arrival and not
allowed to jump queue thereafter.

Input consists of a set of service requests each consisting of an arrival time, service time and number of
servers. (Note: the arrival times are sorted in ascending order).

Your program should read the name of the data file from standard input and then read the data in the
named file into the simulation. Run the simulation for a 20 service requests.
The standard output for both the queuing process will consist of the following data:

- Average waiting time of a service request
Average waiting time = (Total time a service request waits in queue) / (Total number of service
requests)
- Total idle time for each server.
- A short write out to show comparison between both strategies.

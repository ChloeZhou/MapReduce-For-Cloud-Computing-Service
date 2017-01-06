# MapReduce-For-Cloud-Computing-Service
a. Name: Keying Zhou
b. Student ID: 1935-0418-72

c. About the project:

This project aims to simulate a simple model of a cloud computing service. Basically, there is a single client which will send numbers and the mode of operation through TCP to an aws server. Then, the aws server will divide the numbers and send to three back-end servers through UDP to calculate the result. The final result will be sent back to the client finally.
The project has three major phases. 
1.In phase one, all servers boot up and client will send the data and operating mode to aws server through TCP. 
2.In phase two aws server has received the integers from the client. It will divide the data into three parts with equal size and send them separately to three backend server A, B and C through UDP. Back-servers will perform the function and the results will be ready at the end of this phase.
3.In phase three, back-servers will send their answers to Frontend-server(aws) through UDP. Then aws server will calculate the final answer and send it back to client through TCP.

d. Code files:

1. client.cpp: The code for client. This program can read the integers which need to be calculated in certain and password from a file and sends the data over TCP.  After it reads the data, it can create a TCP socket. After the TCP socket is created, it will wait for the final result from the AWS server. When it receives the result, it will close the TCP socket and exit. If there are any problems of creating socket or establishing connection, it produces output messages as and when necessary.

2. aws.cpp: The code for frontend server AWS. This program can create a TCP socket and receive the function name and data from the client over TCP. Then it divides the data into three parts and send them to Back-end servers through UDP to calculate the result. After it receives the results from backend servers, it calculates the final result and send back to client.

3. serverA.cpp: The code for back-end server A. Back-end server A receive the data from aws (total n/3 numbers) and performs the reduction type.

4. serverB.cpp: The code for back-end server B. Back-end server B receive the data from aws (total n/3 numbers) and performs the reduction type.


5. serverC.cpp: The code for back-end server C. Back-end server C receive the data from aws (total n/3 numbers) and performs the reduction type.

e. To run the program:

To compile the code please type:

make all

After compiling the code, please type the following command to run

make serverA 
make serverB
make serverC
make aws
./client <function name> (MAX,SUM,SOS,MIN, please use all capital letter)

each one in a different terminal and please follow the order mentioned above.

f. The format of all the message exchanged
Message from Client to AWS consists of 1 packet which contains the function name, total numbers the Client read from the num.cvs file and the data in the file. They are separated by commas: "SUM, 300, 607…" and the type of the array is char. 

Message from AWS to Sever i consists of 1 packet which also contains the function name, total numbers of the data that AWS sends to Server i and the data which needs to be calculated. The array looks like: "SUM, 100, 607…" and the type of the array is char.

Message from Server i to AWS consists of 1 packet which contains the reduction result. The type of the array is char.

Message from AWS to Client consists of 1 packet contains the final result and the type of the array is char.

g. Idiosyncrasy
Any error occurs in the socket related sections is given by perror().

h. Reused Code
Code has used from the beej's guide to socket programming and the website list below.
http://c.biancheng.net/cpp/html/3030.html  (echo server and client).
http://www.cplusplus.com/reference/cstdio/FILE/  (read CSV file).
http://www.cplusplus.com/reference/sstream/stringstream/?kw=stringstream (change int to string)

//
//  main.cpp
//  client
//
//  Created by Keying Zhou on 11/10/16.
//  Copyright © 2016 Keying Zhou. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <iostream>
#include <sstream>

using namespace std;


//read the nums.cvs
string readfile()
{
    FILE* pfile;
    pfile = fopen("nums.csv", "r");
    char number[100];
    string number2 = "";
    string temp;
    int count = 0;
    if (pfile == NULL)
    {
        perror("Error opening file");
        exit(1);
    }
    else
    {
        while (fgets(number, 100, pfile) != NULL) //read every line of nums.cvs
        {
            number2 += number;
            count++;
        }
        for (int i = 0; i < number2.length(); i++)
        {
            if (number2[i] == '\n')
                number2[i] = ',';
        }
        stringstream stream;
        stream << count;
        stream >> temp;
        
        number2 = temp + "," + number2;
    }
    return number2;
}

int main(int argc, const char * argv[]){
    //create socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;  //use IPv4 address
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  //IP
    serv_addr.sin_port = htons(25872);  //Port
    printf("The client is up and running.\n");
    
    // get function name
    
    string functionname;
    functionname=argv[1];
    
    // connect to aws
    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
    {
        perror("connect");
        exit(1);
    }
    //send data to aws
    //read the nums.cvs and process the data
    
    string numfile = readfile ();
    numfile=functionname+","+numfile+"\0";  //add function name to numfile and send
    char CNumberFile[10000];
    strcpy (CNumberFile, numfile.c_str());
    int allnum=0;
    allnum=(CNumberFile[4]-48)*100+(CNumberFile[5]-48)*10+(CNumberFile[6]-48)*1;
    send(sock, CNumberFile, numfile.length() , 0);
    printf("The client has sent the reduction type %s to AWS\n", functionname.c_str());
    printf("The client has sent %d numbers to AWS\n", allnum);
    
    //receive result from aws
    char result[10000];
    memset(result,0,sizeof(result));
    
    recv(sock, result, sizeof(result), 0);

    printf("The client has received reduction %s:%s\n",functionname.c_str(),result);
    //close socket
    close(sock);
    return 0;
}

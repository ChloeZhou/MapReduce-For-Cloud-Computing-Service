//
//  main.cpp
//  SeverA
//
//  Created by Keying Zhou on 11/11/16.
//  Copyright © 2016 Keying Zhou. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <iostream>
#include <sstream>

int typechange(char *para)
{
    int integer=0;
    for (int i=0; para[i] !='\0'; i++)
    {
        integer= (para[i]-48)+integer*10;
    }
    return integer;
}


int main(int argc, const char * argv[]) {
    printf("The Server A is up and running using UDP on port 21872.\n");
    while(1)
    {
        //creat socket
        int serva_sock = socket(AF_INET, SOCK_DGRAM, 0);
        
        //bind socket IP and port
        struct sockaddr_in serva_addr;
        memset(&serva_addr, 0, sizeof(serva_addr));
        serva_addr.sin_family = AF_INET; //use IPv4 addr
        serva_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  //IP
        serva_addr.sin_port = htons(21872);  //Port
        
        bind(serva_sock, (struct sockaddr*)&serva_addr, sizeof(serva_addr));
        
        
        
        //receive data from aws
        
        struct sockaddr_in aws_addr;
        socklen_t aws_addr_size = sizeof(aws_addr);
        char recvfromaws[10000];
        memset(&recvfromaws, 0, sizeof(recvfromaws));
        
        recvfrom(serva_sock, recvfromaws, sizeof(recvfromaws), 0, (struct sockaddr *)&aws_addr, &aws_addr_size);
        //printf("%s\n",recvfromaws);   //for test
        
        
        //process the data
        
        //get functionname
        
        int nums=0;
        int i=0;
        char functionname[10];
        memset(functionname,0,sizeof(functionname));
        for( ;recvfromaws[i] != ','; i++)
        {
            functionname[i]=recvfromaws[i];
        }
        i++;
        
        //get data size
        
        char temp[40];
        memset(temp,0,sizeof(temp));
        for(int ia=0 ;recvfromaws[i] != ','; i++)
        {
            temp[ia++]=recvfromaws[i];
        }
        nums=typechange(temp);
        printf("Server A has receive %d numbers.\n",nums);
        
        //change char to int
        
        i++;
        int recv[10000];
        memset(recv,0,sizeof(recv));
        char temp2[10];
        for (int ir=0; ir<nums; ir++ )
        {
            memset(temp2,0,sizeof(temp2));
            for(int ip=0; recvfromaws[i] != ',';ip++)
            {
                temp2[ip]=recvfromaws[i];
                i++;
            }
            i++;
            recv[ir]= typechange(temp2);
        }
        // printf("%d\n%d\n",recv[0],recv[99]);    //for test
        //Calculate
        
        long long int result=0;
        if(recvfromaws[2]=='M')    //do the sum
        {
            for(int ia=0; ia<nums; ia++ )
            {
                result+=recv[ia];
            }
        }
        if(recvfromaws[2]=='X')
        {
            for(int ia=0; ia<nums; ia++)
            {
                if(recv[ia]>result)
                    result=recv[ia];
            }
        }
        if(recvfromaws[2]=='N')
        {
            result=recv[0];
            for(int ia=0; ia<nums; ia++)
            {
                if(recv[ia]<result)
                    result=recv[ia];
            }
        }
        if(recvfromaws[2]=='S')
        {
            for(int ia=0; ia<nums; ia++)
            {
                result+=recv[ia]*recv[ia];
            }
        }
        //printf("%lld\n",result);  //for test
        
        
        printf("The Server A has successfully finished the reduction %s: %lld\n",functionname,result);
        
        //change longlongint result to char
        
        std::string stresult;
        char chresult[10000];
        memset(chresult,0,sizeof(chresult));
        std::stringstream stream;
        stream << result;
        stream >> stresult;
        strcpy(chresult,stresult.c_str());
        
        //send result to aws
        
        int aws_sock = socket(AF_INET, SOCK_DGRAM, 0);
        struct sockaddr_in awsse_addr;
        memset(&awsse_addr,0,sizeof(aws_addr));
        awsse_addr.sin_family = AF_INET; //use IPv4 addr
        awsse_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  //IP
        awsse_addr.sin_port = htons(24872);  //Port
        char ACK;
        ACK='\0';
        recvfrom(serva_sock, &ACK, sizeof(ACK), 0, (struct sockaddr *)&aws_addr, &aws_addr_size);
        close(serva_sock);
        if (ACK=='r')
        {
            if(sendto(aws_sock, chresult, stresult.length(), 0, (struct sockaddr *) &awsse_addr, sizeof(awsse_addr))==-1)
            {
                perror("UDPsend");
                exit(1);
            }
            printf("The Server A has successfully finished sending the reduciton value to AWS server.\n");
            close(aws_sock);
        }
    }
    return 0;
}

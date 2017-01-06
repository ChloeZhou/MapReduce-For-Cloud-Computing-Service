//
//  main.cpp
//  aws
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

// function change char to int

 int typechange(char *para)
 {
 int integer=0;
 for (int i=0; para[i] !='\0'; i++)
 {
 integer= (para[i]-48)+integer*10;
 }
 return integer;
 }


int main(){
    
    //creat socket
    int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    //bind socket IP and port
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET; //use IPv4 addr
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  //IP
    serv_addr.sin_port = htons(25872);  //Port
    bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    printf("aws is up and running.\n");
    
    //listen
    listen(serv_sock, 20);
    
    while (1)
    {
        //accept
        
        struct sockaddr_in clnt_addr;
        socklen_t clnt_addr_size = sizeof(clnt_addr);
        int clnt_sock = accept (serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
        
        //receive data
        
        char buff[10000];
        memset(buff,0,sizeof(buff));
        recv ( clnt_sock, buff, sizeof(buff), 0);
        

        //change int allnum into char challnum
 
        char sendtoa[10000],sendtob[10000],sendtoc[10000],funcname[10];
        memset(funcname,0,sizeof(funcname));
        
        int i=0;
        
        //send the function name to three back end server
        
        int ia=0,ib=0,ic=0,ip=0;
        for (;buff[i] !=',';i++)
        {
            sendtoa[ia]=buff[i];
            ia++;
            sendtob[ib]=buff[i];
            ib++;
            sendtoc[ic]=buff[i];
            ic++;
            funcname[ip]=buff[i];
            ip++;
        }
        sendtoa[ia]=',';
        ia++;
        sendtob[ib]=',';
        ib++;
        sendtoc[ic]=',';
        ic++;
        i++;

        //send the total number of data to three back end server
        //printf("%s\n",challnum);          //for test
        ip=0;
        char challnum[10];
        memset(challnum,0,sizeof(challnum));
        for (; buff[i] != ','; i++)
        {
            challnum[ip]=buff[i];
            ip++;
        }
        printf("The AWS has receive %s numbers from the client using TCP over prot 25872\n",challnum);
        int allnum=typechange (challnum);
        allnum=allnum/3;
        memset(challnum,0,sizeof(challnum));
        std::string stallnum;
        std::stringstream stream;
        stream << allnum;
        stream >> stallnum;
        strcpy(challnum,stallnum.c_str());
        
        for (int chp=0; challnum[chp] != '\0'; chp++)
        {
            sendtoa[ia]=challnum[chp];
            ia++;
            sendtob[ib]=challnum[chp];
            ib++;
            sendtoc[ic]=challnum[chp];
            ic++;
        }
        
        sendtoa[ia]=',';
        ia++;
        sendtob[ib]=',';
        ib++;
        sendtoc[ic]=',';
        ic++;
        i++;
        
        //divide the numbers
        
       // i=i+5; //jump over the allnum
        
        for(int counta=0; counta<allnum; i++)
         {
           sendtoa[ia++]=buff[i];
             if (buff[i]==',')
                 counta++;
           
         }
        
         for(int countb=0; countb<allnum; i++)
         {
            sendtob[ib++]=buff[i];
             if(buff[i]==',')
                 countb++;
         }
        
         for(int countc=0; countc<allnum; i++)
         {
             
             sendtoc[ic++]=buff[i];
              if(buff[i]==',')
                  countc++;
         }
        //printf("%s\n%s\n%s\n",sendtoa,sendtob,sendtoc);     //for test
        
        
        // UDP connect to server A,B,C

        //server A
        int serva_sock = socket(AF_INET, SOCK_DGRAM, 0);
        struct sockaddr_in serva_addr;
        memset(&serva_addr,0,sizeof(serva_addr));
        serva_addr.sin_family = AF_INET; //use IPv4 addr
        serva_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  //IP
        serva_addr.sin_port = htons(21872);  //Port
        
        if(sendto(serva_sock, sendtoa, strlen(sendtoa), 0, (struct sockaddr *) &serva_addr, sizeof(serva_addr))==-1)
        {
            perror("UDPsend");
            exit(1);
        }
        printf("The AWS has sent %d numbers to Backend-Server A.\n", allnum);
       

        //server B
        
        int servb_sock = socket(AF_INET, SOCK_DGRAM, 0);
        struct sockaddr_in servb_addr;
        memset(&servb_addr,0,sizeof(servb_addr));
        servb_addr.sin_family = AF_INET; //use IPv4 addr
        servb_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  //IP
        servb_addr.sin_port = htons(22872);  //Port
        if(sendto(servb_sock, sendtob, strlen(sendtob), 0, (struct sockaddr *) &servb_addr, sizeof(servb_addr))==-1)
        {
            perror("UDPsend");
            exit(1);
        }

        printf("The AWS has sent %d numbers to Backend-Server B.\n", allnum);
 
        //server C

        int servc_sock = socket(AF_INET, SOCK_DGRAM, 0);
        struct sockaddr_in servc_addr;
        memset(&servc_addr,0,sizeof(servc_addr));
        servc_addr.sin_family = AF_INET; //use IPv4 addr
        servc_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  //IP
        servc_addr.sin_port = htons(23872);  //Port
        if(sendto(servc_sock, sendtoc, strlen(sendtoc), 0, (struct sockaddr *) &servc_addr, sizeof(servc_addr))==-1)
        {
            perror("UDPsend");
            exit(1);
        }
        
        printf("The AWS has sent %d numbers to Backend-Server C.\n", allnum);
 
        char ACK;
        ACK='r';
        //receive result from Server A,B,C
        //creat socket
        int awsUDPa_sock = socket(AF_INET, SOCK_DGRAM, 0);

        struct sockaddr_in awsUDPa_addr;
        memset(&awsUDPa_addr, 0, sizeof(awsUDPa_addr));
        awsUDPa_addr.sin_family = AF_INET; //use IPv4 addr
        awsUDPa_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  //IP
        awsUDPa_addr.sin_port = htons(24872);  //Port
        
        bind(awsUDPa_sock, (struct sockaddr*)&awsUDPa_addr, sizeof(awsUDPa_addr));
        
        //let server A know aws is ready to receive
        sendto(serva_sock, &ACK, sizeof(ACK), 0, (struct sockaddr *) &serva_addr, sizeof(serva_addr));
         close(serva_sock);
        
        //receive result from Server A
        struct sockaddr_in servAre_addr;
        socklen_t servAre_addr_size = sizeof(servAre_addr);
        char recvfromserva[10000];
        memset(&recvfromserva, 0, sizeof(recvfromserva));
        
        if(recvfrom(awsUDPa_sock, recvfromserva, sizeof(recvfromserva), 0, (struct sockaddr *)&servAre_addr, &servAre_addr_size)==-1)
        {
            perror("receive");
            exit(-1);
        }
        
        printf("The AWS received reduction result of %s from Backend-Server A using UDP over port 24872 and it is %s\n",funcname,recvfromserva);
        close(awsUDPa_sock);

        //creat socket
        int awsUDPb_sock = socket(AF_INET, SOCK_DGRAM, 0);
        
        struct sockaddr_in awsUDPb_addr;
        memset(&awsUDPb_addr, 0, sizeof(awsUDPb_addr));
        awsUDPb_addr.sin_family = AF_INET; //use IPv4 addr
        awsUDPb_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  //IP
        awsUDPb_addr.sin_port = htons(24872);  //Port
        
        bind(awsUDPb_sock, (struct sockaddr*)&awsUDPb_addr, sizeof(awsUDPb_addr));
        
        //receive result from Server B
        struct sockaddr_in servBre_addr;
        socklen_t servBre_addr_size = sizeof(servBre_addr);
        char recvfromservb[10000];
        memset(&recvfromservb, 0, sizeof(recvfromservb));
        
        //let b know aws is ready to receive
         ACK='r';
        sendto(servb_sock, &ACK, sizeof(ACK), 0, (struct sockaddr *) &servb_addr, sizeof(servb_addr));
        close(servb_sock);
        
        if(recvfrom(awsUDPb_sock, recvfromservb, sizeof(recvfromservb), 0, (struct sockaddr *)&servBre_addr, &servBre_addr_size)==-1)
        {
            perror("receive");
            exit(-1);
        }
        
        printf("The AWS received reduction result of %s from Backend-Server B using UDP over port 24872 and it is %s\n",funcname,recvfromservb);
        close(awsUDPb_sock);

 
        //creat socket
        int awsUDPc_sock = socket(AF_INET, SOCK_DGRAM, 0);
        
        struct sockaddr_in awsUDPc_addr;
        memset(&awsUDPc_addr, 0, sizeof(awsUDPc_addr));
        awsUDPc_addr.sin_family = AF_INET; //use IPv4 addr
        awsUDPc_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  //IP
        awsUDPc_addr.sin_port = htons(24872);  //Port
        
        bind(awsUDPc_sock, (struct sockaddr*)&awsUDPc_addr, sizeof(awsUDPc_addr));
        
        //receive result from Server C
        struct sockaddr_in servCre_addr;
        socklen_t servCre_addr_size = sizeof(servCre_addr);
        char recvfromservc[10000];
        memset(&recvfromservc, 0, sizeof(recvfromservc));
        //let c know aws is ready to receive
         ACK='r';
        sendto(servc_sock, &ACK, sizeof(ACK), 0,  (struct sockaddr *)&servc_addr, sizeof(servc_addr));
        close(servc_sock);
        
        if(recvfrom(awsUDPc_sock, recvfromservc, sizeof(recvfromservc), 0, (struct sockaddr *)&servCre_addr, &servCre_addr_size)==-1)
        {
            perror("receive");
            exit(-1);
        }
        
        printf("The AWS received reduction result of %s from Backend-Server C using UDP over port 24872 and it is %s\n",funcname,recvfromservc);
        close(awsUDPc_sock);
    
        
        //calculate the final result
        int change[3];
        change[0]=typechange(recvfromserva);
        change[1]=typechange(recvfromservb);
        change[2]=typechange(recvfromservc);
        //printf("%s\n%d\n%d\n%d\n",printonscreen,change[0],change[1],change[2]); for test
        long long int result=0;
        if(funcname[2] == 'M' || funcname[2] == 'S')    //do the sum and sos
        {
            for(int ia=0; ia<3; ia++ )
            {
                result+=change[ia];
            }
        }
        else if(funcname[2]=='X')      //do the max
        {
            for(int ia=0; ia<3; ia++)
            {
                if(change[ia]>result)
                    result=change[ia];
            }
        }
        else if(funcname[2]=='N')     //do the min
        {
            result=change[0];
            for(int ia=0; ia<3; ia++)
            {
                if(change[ia]<result)
                    result=change[ia];
            }
        }
        
        printf("The AWS has successfully finished the reduction %s :%lld\n",funcname,result);
        

        //change int result into char chresult
        std::string stresult;
        char chresult[10];
        memset(chresult,0,sizeof(chresult));
        std::stringstream sstream;
        sstream << result;
        sstream >> stresult;
        strcpy(chresult,stresult.c_str());
        
        //send result to client
        
        send(clnt_sock, chresult, stresult.length() , 0);
        
        close(clnt_sock);
        
        printf("The AWS has successfully finished sending the reduction value to client.\n");
        
        
        
    }

    return 0;
}

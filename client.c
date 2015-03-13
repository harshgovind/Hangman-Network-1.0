/*/#####################################################################################################
				HANGMAN(NETWORK VERSION 1.0)--A WORD GAME---------------
####################################################################################################*/




#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<stdio_ext.h>
#include <sys/types.h>
#include <signal.h>
#include<time.h>
#include "cli.h"


//*********************** MAIN START***************************************
void main(int argc, char **argv) 
{
	char mesg[1],*temp,ch;
	system("clear");
	FILE *fp;
	if (argc != 3)
	{
		printf("usage: udpcli <IPaddress> & port\n");
		exit (0);
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(atoi(argv[2]));
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
	
	len_se = sizeof(servaddr);
	len_pr=sizeof(proxyaddr);
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd==-1)
	{		
		perror("error in socket creation\n");
		exit(0);
	}

	fp=fopen("rules.txt","r");		//PRINTING GAME RULES
	if(fp==NULL)
		printf("failed to open rules.txt\n");	
	while(1)
	{
		ch=fgetc(fp);
		if(ch==EOF)
			break;
		else	
		printf("%c",ch);
	}
	
	printf("\n\t\tConnecting to server...\n");
	temp="\n\t\t1.Computerised Game \n\t\t2.Manual with your friend \n\t\t3.Exit";
	ret_sendto= sendto(sockfd,temp , strlen(temp), 0, (struct sockaddr *)&servaddr,len_se);//SENDING  OPTIONS
	if(ret_sendto<0)
		perror("error in send to\n");

	ret_recvfrom=recvfrom(sockfd,mesg,sizeof(mesg),0,(struct sockaddr *)&proxyaddr,&len_se);//RECIEVING CHOICE 
	if(ret_recvfrom<0)
		perror("error in recvfrom\n");
	printf("\t\tConnected !\n");
	switch(mesg[0])
	{
		case '1':		// CASE OF AUTO PLAY
			  while(1)
			  {	
				sendq();	
				recivq();
			  }
			  break;
		case '2':		//  CASE OF MANUAL PLAY
			  while(1)
			  {
				sendman();
				recivq();
	
			  }
		case '3':		//  CASE TO EXIT
	
			  printf("\t\toppnent want to terminate\n");
			  exit(1);
	}

}		
//************************* MAIN END***************************************************
		



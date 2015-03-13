/*/#####################################################################################################
				HANGMAN(NETWORK VERSION 1.0)--A WORD GUESSING GAME GAME---------------
####################################################################################################*/


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<time.h>
#include<stdio_ext.h>
#include "serv.h"



int main(int argc, char **argv)
{
	FILE *fp;
	char ch;	
	system("clear");
	if (argc != 2)
	{
		printf("usage: udp  port\n");
		exit (0);
	}
	/* create  an endpoint for communication */
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);	
	if(sockfd==-1)
	{		
		perror("error in socket creation\n");
		exit(0);
	}
	
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(atoi(argv[1]));
	/* bind to socket */
	ret=bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));	
	if(ret==-1)
	{		
		perror("error in socket binding\n");
		exit(0);
	}	
	len=sizeof(cliaddr);
	/* open a file rules to display rules */
	fp=fopen("rules.txt","r");		
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
	
	printf("\n\t\twaiting for client...\n");
	bzero(mesg,sizeof(mesg));
	ret_recvfrom=recvfrom(sockfd, mesg, MAXLINE, 0,(struct sockaddr *)&cliaddr, &len);
	if(ret_recvfrom<0)
        {
                   perror("error in recvfrom");
        }
		
	printf("\t\tconnected !");	
	printf("%s\n",mesg);
	printf("\t\tselect option :");
	__fpurge(stdin);
	gets(sendline);
	/* choice for mode */
	ret_sendto=sendto(sockfd, sendline, sizeof(sendline), 0,(struct sockaddr *)&cliaddr,len);
	
	if(ret_sendto<0)
	{
        		perror("error in sendto1");
        }
	/* switch modes accordingly */
	switch(sendline[0])	
	{
		case '1':while(1)
			 {
	
				recvq();
				sendq();
			 }
			 break;
		
		case '2':while(1)
			 {
				recvq();
				sendman();
			 }
			 break;
		case '3':
			 printf("\t\tThanks for using Hangaro\n");
			 exit(0);	
	}
				
}



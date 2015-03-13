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


int flg=0,won=1;
int score2=0;


/*---------------------------GENRATING RANDOM WORD AND HINT IN THE CASE OF AUTO MODE---------------------*/
void sendq()	
{
	int iseed,mod,mod1;
	/* WORD DICTIONARY */
	char *str[4][15]={"PARIS","MUMBAI","SHANGHAI","TOKYO","BERLIN","SYDNEY","DELHI","JOHANSBERG","LONDON","NEWYORK","CAPETOWN",
                          "MUNICH","CHICAGO","DALLAS","ATLANTA",
                          "INDIA","GREAT BRITAIN","RUSSIA","JAPAN","FRANCE","CHINA","GERMANY","NORWAY","HOLLAND","SPAIN","ANTARCTICA",
                          "UAE","ARGENTINA","AUSTRALIA","BRAZIL",
                          "LION","PANTHER","RABBIT","DEER","TIGER","ELEPHANT","SNAKE","MONKEY","HORSE","CAT","ZEBRA","HIPPOPOTAMUS",
                          "GIRAFF","WOLF","FOX",  
                          "MANGO","ORANGE","PINEAPPLE","APPLE","GRAPE","GUAVA","PEECH","LICHY","STRAWBERRY","CHERRY","BANANA","CARROT",
                          "COCONUT","BLACKBERRY","PAPAYA"};    
	char *string,*temp;
	temp="Press any key to continue...";
        ret_sendto= sendto(sockfd,temp,strlen(temp), 0, (struct sockaddr *)&servaddr,len_se);
	if(ret_sendto<0)
	{
		perror("error in sendto");
	}
	
	sleep(1);
	system("clear");
	bzero(mesg,sizeof(mesg));
	/* RECIEVE ANY KEY WHEN OPPONENT READY  */
	ret_recvfrom=recvfrom(sockfd,mesg,sizeof(mesg),0,(struct sockaddr *)&proxyaddr,&len_se);
	
	printf("\t\tNow given word is\n");
	
	won=1;
	/* RANDOM WORD GENERATION */
	iseed=(int)time(NULL);		
	srand(iseed);				
	randm=rand();			
	mod1=randm%4;			
	mod=randm%15;			
	string=str[mod1][mod];		
	
	/*  HINTS ACCORDING TO WORD */
	switch(mod1)	
	{
		case 0:
			temp="HINT:This is CITY";
			break;
		case 1:
			temp="HINT:This is country name";
			break;
		case 2:
			temp="HINT:This is Animal";
			break;
		case 3:
			temp="HINT:This is fruit";
			
	}
	sendword(string,temp);	
}
	
	
/*SEND WORD AND HINT TO OPPONENT, RECIEVE CHARECTER FORM OPPONENT AND CHECK*/
void sendword(char *string,char *temp)	
{
	char mes1[100],tmp[80];
	char temp1[15],*temp2;
	int chance=6,f1=0,f2=0,i,j=0,count=1; 
	won=1;
	
	printf("\t\tYou given -- %s --\n",string);	
	
	count=strlen(string);
	/* TRANSLATE WORD INTO '--' */
	for(i=0;i<count;i++)
	{
	if(string[i]==' ')
	temp1[i]=' ';
	else
	temp1[i]='-';
	}
	temp1[i]='\0';
	temp2=temp1;		

	/* SEND HINT */
	ret_sendto= sendto(sockfd,temp, strlen(temp), 0, (struct sockaddr *)&servaddr,len_se); 
        if(ret_sendto<0)
        {
        	perror("error in sendto at word1 send");
        }
	
	/* RECIEVE CHARECTER OR WORD FROM OPPONENT & CHECK WITH CORRECT WORD */
	while(chance && won)	
	{
		f1=0;
		f2=0;
		/* SEND UPDATES OF WORD AFTER GUESSING TO OPPONENT */
		ret_sendto= sendto(sockfd, temp2, strlen(temp2), 0, (struct sockaddr *)&servaddr,len_se); 
 		if(ret_sendto<0)
                {
                	perror("error in sendto at word1 send");
                }
	
		/* CHECK WHETHER SPECIAL REQUEST RECIEVE OR NOT */
                do		
		{	        
			bzero(mesg,sizeof(mesg));		
			ret_recvfrom=recvfrom(sockfd,mesg,sizeof(mesg),0,(struct sockaddr *)&proxyaddr,&len_se);
			/* IF OPPONENT SEND MASSAGE THEN GIVE REPLY */
			if(mesg[0]=='>')	
			{
				printf("\t\tOppnent says: %s\n",mesg);
				printf("\t\treply : ");			
				gets(tmp);
				ret_sendto= sendto(sockfd, tmp, sizeof(tmp), 0, (struct sockaddr *)&servaddr,len_se);
                		if(ret_sendto<0)
                		{
                			perror("error in sendto at word2 send");
                		}
			}

			/* IF OPPONENT GUESS WHOLE WORD */
			if(mesg[0]=='!')	
			{	
				strcpy(mes1,mesg);
				strcpy(mesg,&mes1[1]);				
				f1=1;
				if(strcasecmp(mesg,string)==0)
				{
					f2=1;
					score2+=10;		
			                won=0;
			                ret_sendto= sendto(sockfd,string, strlen(string), 0, (struct sockaddr *)&servaddr,len_se);
                        		if(ret_sendto<0)
                        		{
                               		 	perror("error in sendto at word1 send");
                        		}
				}

			}

			/* IF OPPONENT WANTS TO EXIT */
			if(mesg[0]=='#')	
			{	
				
				printf("\t\tterminating connection..\n");
				sprintf(tmp,"#%d",score2);
				ret_sendto= sendto(sockfd, tmp, sizeof(tmp), 0, (struct sockaddr *)&servaddr,len_se);
                		if(ret_sendto<0)
                		{
                			perror("error in sendto at word2 send");
                		}
				strcpy(mes1,mesg);
				strcpy(mesg,&mes1[1]);					
				printf("\n\t-------------SCORE--------------\n");				
				printf("\t\tYour score is %s\n\t\topponent score is %d\n",mesg,score2);
				printf("\t--------------------------------\n");
				printf("\n-----------------Thanks for playing---------------------\n");
				exit(0);
			}
				
					
		}while(mesg[0]=='>');	
		
		/* IF OPPONENT GUESS WHOLE WORD BREAK FROM GUESSING WHILE */
		if(f1==1)	
			break;
		   
		j=0;
		/* ADJUSTING CASE */
		if(mesg[0]>='a'&& mesg[0]<='z')	 
		mesg[0]-=32;
		// CHARECTER CHECKING START */
		while(string[j]!='\0')		
		{
			__fpurge(stdin);
			if(mesg[0]==string[j])
			{
								
				temp1[j]=mesg[0];
				temp2=temp1;	
			/* SET FLAG IF GUESS CORRECT */	
				flg=1;		
			}
			j++;
		}				
		/* CHECKING END */

		/* IF GUESS WRONG */				
		if(flg==0)	
		{	
			j=0;
			/* DECREASE CHANCES */ 
			chance--;	
			sprintf(tmp,"you are wrong chance left %d",chance);
			ret_sendto=sendto(sockfd,tmp,strlen(tmp), 0, (struct sockaddr *)&servaddr,len_se);
                	if(ret_sendto<0)
                	{
                		perror("error in sendto at word1 send");
                	}

		}
		
		/* IF GUESS CORRECT */
		else		
                {      
			
			flg=0;			
			j=0;			
			sprintf(tmp,"you are correct chance left %d",chance);                        
			ret_sendto= sendto(sockfd, tmp, strlen(tmp), 0, (struct sockaddr *)&servaddr,len_se);
                	if(ret_sendto<0)
                	{
                		perror("error in sendto at word1 send");
                	}
		}
		
		/* AT END OF GUESSING WHILE IT CHECKS FOR ENTIRE WORD */
		if((strcmp(temp2,string))==0)	
		{		
			won=0;
			sprintf(tmp,"Correct Word : %s\n",string);
			ret_sendto= sendto(sockfd, tmp, strlen(tmp), 0, (struct sockaddr *)&servaddr,len_se);
   			if(ret_sendto<0)
        		{
        			perror("error in sendto at word1 send");
      			}


		}

			
	}	
	/* GUESSING WHILE END */
	
	/* IF WORD MATCHED */	
	if(won==0)	
	{	
		printf("\t\tOpponent win!\n");		
		if(f2==0)			
		score2+=5;
		temp="Yuppy!you guess correct.. :-)";
		ret_sendto= sendto(sockfd, temp, strlen(temp), 0, (struct sockaddr *)&servaddr,len_se);
   		if(ret_sendto<0)
        	{
        		perror("error in sendto at word1 send");
      		}
	}
	/* IF WORD NOT MATCHED */
	else		
	{
		printf("\t\tOpponent loss\n");			
		temp="SORRY! you failed.. :-(";
		ret_sendto= sendto(sockfd, temp, strlen(temp), 0, (struct sockaddr *)&servaddr,len_se);
   		if(ret_sendto<0)
        	{
        		perror("error in sendto at word1 send");
      		}
		sprintf(tmp,"Correct Word : %s\n",string);		
		ret_sendto= sendto(sockfd, tmp, strlen(tmp), 0, (struct sockaddr *)&servaddr,len_se);
   		if(ret_sendto<0)
        	{
        		perror("error in sendto at word1 send");
      		}
	}
	/* TERMINATING CONDITION FOR ONE ROUND */
	temp="*";	
	ret_sendto= sendto(sockfd, temp, strlen(temp), 0, (struct sockaddr *)&servaddr,len_se);
                if(ret_sendto<0)
                {
                        perror("error in sendto at word1 send");

                }
	sleep(2);
	system("clear");

}


/* RECIEVE WORD AND HINT FROM OPPONENT AND GUESS CHARECTER OR WORD */
void recivq()	
{
	int ret,len,wt;
	struct sockaddr_in cliaddr;
	char msg[80],msg1[80],*tr="*";	
	len = sizeof(cliaddr);
	/* CHILD CREATED */
	ret=fork();	
	if(ret<0)
	{
		perror("error in fork");exit(1);
	}

	/*bPARENT RECIEVES */
	if(ret>0)	
	{
        	
		while(1)
		{
       			
			bzero(msg,sizeof(msg));
			
                	ret_recvfrom=recvfrom(sockfd, msg, sizeof(msg), 0,(struct sockaddr *)&proxyaddr, &len_pr);
		 	if(ret_recvfrom<0)
                	{
                		perror("error in recvfrom");
                	}
			/* RECIEVES SCORE AFTER SENDING EXIT */
			if(msg[0]=='#')		
			{
				printf("\t\tterminating...\n");
				strcpy(msg1,msg);
				strcpy(msg,&msg1[1]);
				printf("\n\t-------------SCORE--------------\n");					
				printf("\t\tYour score %s \n\t\topponent score %d\n",msg,score2);
				printf("\t--------------------------------\n");
				printf("\n-----------------Thanks for playing---------------------\n");
				kill(ret,9);					
				wt=waitpid(-1,NULL,0);
				if(wt<0)	
					perror("error in waitpid\n");
				exit(0);
			}
			
			if(strcmp(msg,tr))
			printf("	 %s\n",msg) ;
			/* CHECK WHETHER ROUND COMPLETE OR NOT */
			if(strcmp(msg,tr)==0)	
			{
				kill(ret,9);					
				wt=waitpid(-1,NULL,0);
				if(wt<0)	
					perror("error in waitpid\n");
				return;
			}
		}
	}
	
	/* CHILD IN SENDING MODE */
	if(ret==0)	
	{	
		len=len_se;
		while(1)
		{	__fpurge(stdin);
			gets(sendline);
		  	if(strcasecmp(sendline,"exit")==0)
			{
				sprintf(sendline,"#%d",score2);
			}
			ret_sendto=sendto(sockfd,sendline, strlen(sendline),0,(struct sockaddr *)&servaddr, len_se);
			if(ret_sendto<0)
		      	{
               			perror("error in sendto");
                	}
			if(sendline[0]!='>')
			printf("\t\tGuess charecter\n");
			ch[0]='\0';
			bzero(sendline,sizeof(sendline));
			
		}
     	}	
}


/* SEND WORD AND HINT MANUALY TO OPPONENT */
void sendman()	
{	
	int i,len;
	char mant[30],mans[30];
	printf("\t\tgive any word to opponent\n");
	/* GIVING WORD TO OPPONENT */
	gets(mans);				
	printf("\t\tenter the hint\n");
	/* GIVING HINT */
	gets(mant);				
	len=strlen(mans);
	/* CASE ADJUSTING */
	for(i=0;i<len;i++)			
	{
		if(mans[i]>='a'&& mans[i]<='z')
		mans[i]-=32;
	}
	sendword(mans,mant);
}



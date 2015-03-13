
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


int score1=0;
	
void recvq()
{
	int wt;		
	char mes1[80];
	tr="*";
		
	len = sizeof(cliaddr);
	/* forking for receiving and sending simultaneously */
	ret=fork();	
	if(ret<0)
	{
		perror("error in fork");exit(1);
	}
	/* parent process waits for receiving data */
	if(ret>0)	
	{
        	while(1)
		{
			bzero(mesg,sizeof(mesg));
			
			ret_recvfrom=recvfrom(sockfd, mesg, MAXLINE, 0,(struct sockaddr *)&cliaddr, &len);
					
		 	if(ret_recvfrom<0)
                	{
                		perror("error in recvfrom");
                	}
			/* if oppnent wants terminate then notify */
			if(mesg[0]=='#')
			{	
				printf("\t\tterminating...\n");
				strcpy(mes1,mesg);
				strcpy(mesg,&mes1[1]);
				printf("\n\t-------------SCORE--------------\n");					
				printf("\t\tYour score %s \n\t\topponent score %d\n",mesg,score1);
				printf("\t--------------------------------\n");
				printf("\n-----------------Thanks for playing---------------------\n");
				kill(ret,9);					
				wt=waitpid(-1,NULL,0);
				if(wt<0)	
					perror("error in waitpid\n");
				exit(0);
			}

			if(strcmp(mesg,tr))
				printf("\t\t%s\n",mesg);
			if((strcmp(mesg,tr))==0)
			{	
				kill(ret,9);					
				wt=waitpid(-1,NULL,0);
				if(wt<0)	
					perror("error in waitpid\n");
				return;
			}
		}	
	}	
	/* child process sends the data */
	if(ret==0)
	{	
		while(1)
		{
			__fpurge(stdin);
			gets(sendline);
            	    	if(strcasecmp(sendline,"exit")==0)
			{
				sprintf(sendline,"#%d",score1);
			
			}
			ret_sendto=sendto(sockfd, sendline, strlen(sendline), 0,(struct sockaddr *)&cliaddr, len);
			if(ret_sendto<0)
		       	{
               			perror("error in sendto");
                	}
			if(sendline[0]!='>')
			printf("\t\tGuess charecter\n");
			bzero(sendline,sizeof(sendline));
			
		}
	}
	
}




/* this function is used to send question */ 
void sendq()
{
	int mod1,iseed;
	char mesg1[1];
	/* database for computerised mode */
	char *str[3][15]={"AISHA","AAKROSH","CROOK","GUZAARISH","GOLMAAL3","SHQIYA","LAMHAA","ROBOT","RAAVAN","RAAJNEETI","13B",
                          "DELHI6","KAMINEY","PAA","WANTED",
                          "ADIDAS","ARMANI","BOSSINI","FUBU","FCUK","GUCCI","LEVIS","LACOSTE","PUMA","VERSACE","WRANGLER",
                          "ROLEX","REEBOK","PEPSI","OMEGA",
                          "SHAHRUKH","HRITHIK","JOHN ABRAHAM","AJAY DEVGAN","GOVINDA","SALMAN","AMIR KHAN","SANJAY DUTT","EMRAAN HASHMI",
                          "ABHISHEK BACHCHAN","AKSHAY KUMAR","ANIL KAPOOR","VIVEK OBEROI","RITESH DESHMUKH","PARESH RAWAL"};
	char *string,*temp,temp1[15],*temp2,*tr;
	int won,i,j,randm,mod,count=0,chance=6; 
	
	chance=6;
	won=1;
	iseed=(int)time(NULL);
	/* rand() function with timestamp for random data generation */
	srand(iseed);
	randm=rand();
	mod1=randm%3;
	mod=randm%15;
	string=str[mod1][mod];
	
	temp="Press any key to continue...";
	ret_sendto= sendto(sockfd, temp, strlen(temp), 0, (struct sockaddr *)&cliaddr,len);

        if(ret_sendto<0)
        {
                perror("error in sendto");
        }

	sleep(1);
	system("clear");
	bzero(mesg1,sizeof(mesg1));
	/* relpy of request */
	ret_recvfrom=recvfrom(sockfd,mesg1,sizeof(mesg1),0,(struct sockaddr *)&cliaddr,&len);
		
	if(ret_recvfrom<0)
        {
                perror("error in recv");
        }
	
	/* generates appropriate hint */
	switch(mod1)
	{
	case 0:
	temp="HINT:This is Bolywood movie";
	break;
	case 1:
	temp="HINT:This is wellknown brand";
	break;
	case 2:
	temp="HINT:This is an indian actor";
	break;
	}
	sendword(string,temp);

}


/* function to send word and checks whether opponent is right or wrong */
void sendword(char *string,char *temp)
{	
	int won=1,count =0;
	int i,j,f1=0,f2=0;
	int flg=0;
	
	char mesg1[80],mes1[80];
	char temp1[15],*temp2;
	int chance=6; 
	won=1;
	
	count=strlen(string);
	for(i=0;i<count;i++)
	{
		if(string[i]==' ')
			/* sends spaces as it is */
			temp1[i]=' ';	
		else
			/* send - for string initially */
			temp1[i]='-';	
	}
	temp1[i]='\0';
	temp2=temp1;
	printf("\t\tNow given word is\n");
	printf("\t\tYou given -- %s --\n",string);	
	ret_sendto= sendto(sockfd, temp, strlen(temp), 0, (struct sockaddr *)&cliaddr,len);
        if(ret_sendto<0)
        {
        	perror("error in sendto at word1 send");
        }
	
	/* to check the answers within 6 chances */
	while(chance && won)	
	{
		f1=0;
		f2=0;
		ret_sendto= sendto(sockfd, temp2, strlen(temp2), 0, (struct sockaddr *)&cliaddr,len);
                if(ret_sendto<0)
                {
        	        perror("error in sendto at word1 send");
                }
		ret_recvfrom=-1;
                
		do
		{	        
			bzero(mesg1,sizeof(mesg1));		
			
			ret_recvfrom=recvfrom(sockfd,mesg1,sizeof(mesg1),0,(struct sockaddr *)&cliaddr,&len);
			
			if(ret_recvfrom<0)
                	{
	                	perror("error in recvfrom");
        	        }

			/* IF OPPNENT WANTS HINT */			
			if(mesg1[0]=='>')	
			{
				printf("\t\tOpponent says: %s\n",mesg1);
				printf("\t\treply : ");			
				gets(tmp);	
				ret_sendto= sendto(sockfd, tmp, sizeof(tmp), 0, (struct sockaddr *)&cliaddr,len);
                		if(ret_sendto<0)
                		{
                			perror("error in sendto at word2 send");
                		}
			}

			/* IF OPPONENT SENDS COMPLETE WORD */
			if(mesg1[0]=='!')	
			{	
				strcpy(mes1,mesg1);
				strcpy(mesg1,&mes1[1]);	
				/* IF COMPLETE WORD GUESS THEN SET TO BREAK WHILE LOOP */			
				f1=1;	
				if(strcasecmp(mesg1,string)==0)
				{	
					/* SET FLAG TO ADD 10 PTS */
					f2=1;	
					score1+=10;
	
			                won=0;
			                ret_sendto= sendto(sockfd,string, strlen(string), 0, (struct sockaddr *)&cliaddr,len);
                        		if(ret_sendto<0)
                        		{
                               		 	perror("error in sendto at word1 send");
                        		}
				}

			}
			/* IF OPPONENT EXITS THEN RECEIVE SCORE AND PRINT */
			if(mesg1[0]=='#')	
				{	
					
					printf("\t\tterminating connection\n");
					sprintf(tmp,"#%d",score1);
					ret_sendto= sendto(sockfd, tmp, sizeof(tmp), 0, (struct sockaddr *)&cliaddr,len);
                			if(ret_sendto<0)
                			{
                				perror("error in sendto at word2 send");
                			}
					strcpy(mes1,mesg1);
					strcpy(mesg1,&mes1[1]);							
					printf("\n\t-------------SCORE--------------\n");	
					printf("\t\tYour score is %s\n\t\topponent score is %d\n",mesg1,score1);
					printf("\t--------------------------------\n");
					printf("\n-------------Thanks for playing-----------------\n");
					exit(0);
				}
		}while(mesg1[0]=='>');

		if(f1==1)
			break;							       
		  
		j=0;

		/* ADJUST CASE */		
		if(mesg1[0]>='a'&& mesg1[0]<='z')	
			mesg1[0]-=32;
			
		while(string[j]!='\0')
		{bzero(mesg,sizeof(mesg));
			
			if(mesg1[0]==string[j])
			{
		
				temp1[j]=mesg1[0];
				temp2=temp1;		
				flg=1;
			}
			j++;
		}
		
		/* IF WRONG GUESS DECREMENT COUNT */
		if(flg==0)	
		{	
			j=0;
			chance--;
			
			sprintf(tmp,"you are wrong chance left %d",chance);
			ret_sendto= sendto(sockfd, tmp, sizeof(tmp), 0, (struct sockaddr *)&cliaddr,len);
                	if(ret_sendto<0)
                	{
                		perror("error in sendto at word2 send");
                	}

		}
		
		/* FOR CORRECT GUESS */
		else	
                {      
		
			flg=0;			
			j=0;
                     
			sprintf(tmp,"you are correct chance left %d",chance);
			
                        ret_sendto= sendto(sockfd, tmp, sizeof(tmp), 0, (struct sockaddr *)&cliaddr,len);
                	if(ret_sendto<0)
                	{
                		perror("error in sendto at word3 send");
                	}
		}

		/* IF GUESSED COMPLETELY CORRECTLY THEN SEND CORRECT STRING */
		if((strcmp(temp2,string))==0)	
		{			
			won=0;
			sprintf(tmp,"Correct Word : %s\n",string);
			ret_sendto= sendto(sockfd, tmp, strlen(tmp), 0, (struct sockaddr *)&cliaddr,len);
   			if(ret_sendto<0)
        		{
        			perror("error in sendto at word1 send");
      			}


		}

			
	}	
	
	if(won==0)
	{	
		printf("\t\tOpponent win!\n");		
		if(f2==0)
		score1+=5;
		temp="Yuppy!you guess correct.. :-)";
		ret_sendto= sendto(sockfd, temp, strlen(temp), 0, (struct sockaddr *)&cliaddr,len);
   		if(ret_sendto<0)
        	{
        		perror("error in sendto at word1 send");
      		}
	}
	else
	{
		printf("\t\tOpponent loss\n");		
		temp="SORRY! you failed.. :-(";
		ret_sendto= sendto(sockfd, temp, strlen(temp), 0, (struct sockaddr *)&cliaddr,len);
   		if(ret_sendto<0)
        	{
        		perror("error in sendto at word1 send");
      		}
		sprintf(tmp,"Correct Word : %s\n",string);
		ret_sendto= sendto(sockfd, tmp, strlen(tmp), 0, (struct sockaddr *)&cliaddr,len);
   		if(ret_sendto<0)
        	{
        		perror("error in sendto at word1 send");
      		}
	}
	tr="*";
	ret_sendto=sendto(sockfd,tr,sizeof(tr),0,(struct sockaddr*)&cliaddr,len);
	if(ret_sendto<0)
		perror("error in sento\n");
	sleep(2);         
	system("clear");
}



/* function to send manually */
void sendman()
{	
	int i,len;
	char mant[30],mans[30];
	printf("\t\tgive any word to opponent\n");
	gets(mans);
	printf("\t\tenter the hint\n");
	gets(mant);
	len=strlen(mans);
	for(i=0;i<len;i++)
	{
	if(mans[i]>='a'&& mans[i]<='z')
	mans[i]-=32;
	}
	sendword(mans,mant);
}

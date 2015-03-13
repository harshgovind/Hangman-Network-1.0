
#define  MAXLINE	80

char mesg[80];
int sockfd,ret_sendto,ret_recvfrom,randm;
struct sockaddr_in servaddr,proxyaddr;
char sendline[50];
char ch[1];
socklen_t len_se,len_pr;
char *string1,*string2;
char *mant,*mans;



void sendword(char *string,char *temp);
void sendq();
void recivq();
void sendman();







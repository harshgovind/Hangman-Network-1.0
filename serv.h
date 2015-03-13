#define  MAXLINE	100


int sockfd,ret_recvfrom,ret_sendto,ret;
struct sockaddr_in servaddr, cliaddr,chkaddr;
char sendline[MAXLINE];
socklen_t len;
char mesg[MAXLINE];
char *tr,tmp[80];


void sendq();
void recvq();
void sendword(char *string,char *temp);
void sendman();


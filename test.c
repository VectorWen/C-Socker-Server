#include <stdio.h>
#include <errno.h>
#include "server.h"

void  task(int sockfd,void *arg)
{
	printf("sockfd--> %d\n",sockfd);
}

int main(int argc, char const *argv[])
{
	server serv;
	
	server_init(&serv,8080);
	//int server_accept(server *serv,void (* task)(int sockfd,void *arg),void *arg);
	server_accept(&serv,task,NULL);
	return 0;
}
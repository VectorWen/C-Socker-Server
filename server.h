#ifndef  __SERVER_H
#define  __SERVER_H
#include "thread_pool.h"
#include <sys/socket.h>
#include <netinet/in.h>

#define  DEFAULT_THREADS_NUMBER 5

typedef struct client
{
	int sockfd;
	struct sockaddr_in addr;
	
	void (* task)(int sockfd,void *arg);
	void *arg;
}client;


typedef struct server
{
	int sockfd;
	struct sockaddr_in addr;
	thread_pool tp;
	unsigned int threads_number;
}server;

int server_init(server *serv,int portnumber);
int server_accept(server *serv,void (* task)(int sockfd,void *arg),void *arg);
void destroy_server(server *serv);
#endif //__SERVER_H

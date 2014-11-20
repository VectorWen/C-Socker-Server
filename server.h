#ifndef  __SERVER_H
#define  __SERVER_H

#define  DEFAULT_THREADS_NUMBER 5

struct sock_task
{
	void (* callback_func)(int sockfd);
	int sockfd;
};

int server_init(int portnumber);
int server_accept(void (* callback)(int sockfd));
void destroy_server();
#endif //__SERVER_H
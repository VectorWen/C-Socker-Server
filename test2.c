#include <stdio.h>
#include <errno.h>
#include "server.h"
#include "message.h"

void  task(int sockfd,void *arg)
{
	message msg;
	message_parse_for_socket(&msg,sockfd);
    printf("code = %d, content = %s\n", msg.code,msg.content);

    msg.code = 200;
    char* json = message_to_json_string(&msg);

    write(sockfd,json,strlen(json));
}

int main(int argc, char const *argv[])
{
	server serv;
	
	server_init(&serv,8080);
	//int server_accept(server *serv,void (* task)(int sockfd,void *arg),void *arg);
	server_accept(&serv,task,NULL);
	return 0;
}
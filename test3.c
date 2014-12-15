#include <stdio.h>
#include <errno.h>
#include "server.h"
#include "message.h"

void  task(int sockfd,void *arg)
{
	message msg;
	message_parse_for_socket(&msg,sockfd);
	if(msg.cmd == 1)
	{
		if(strcmp(msg.username,"vector")&&strcmp(msg.passwd,"123456"))
		{
			msg.code = 200;
			msg.sessionid=7463297;
		}else{
			msg.code = 500;
		}
	}else{
		msg.code = 400;
	}
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
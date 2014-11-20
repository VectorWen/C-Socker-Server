#include <stdio.h>
#include <errno.h>
#include "server.h"
#include "message.h"

void  callback	(int sockfd)
{
	struct message msg = message_parse_for_socket(sockfd);
    printf("code = %d, content = %s\n", msg.code,msg.content);

    msg.code = 200;
    msg.content = "ok";
    char* json = message_to_json_string(msg);

    write(sockfd,json,sizeof json);

}

int main(int argc, char const *argv[])
{
	server_init(8080);
	server_accept(callback);
	return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

#include "json.h"
#include "message.h"


void message_parse_for_json(message *msg,char * json)
{
	msg->json_obj = json_tokener_parse(json);

	json_object_object_foreach(msg->json_obj, key, val)
	{
		printf("%s\n", json_object_get_string(val));
		if(strcmp((char*)key,"code") == 0)
		{
			msg->code = json_object_get_int(val);
		}

		if(strcmp((char*)key,"content") == 0)
		{	
			msg->content = (char *)json_object_get_string(val);
		}
	}
}

void message_parse_for_socket(message *msg,int sockfd)
{
	msg->code = 400;
	msg->content = "error";
	json_tokener * tok = json_tokener_new();
	char buf[1024];
	bzero(buf,1024);
	int len = -1;
	while((len = read(sockfd,buf,1024))>0)
	{
		printf("%s\n", buf);
		msg->json_obj = json_tokener_parse_ex(tok,buf,len);
		if(!is_error(msg->json_obj))
		{
			printf("%s\n", "read json ok");
			break;
		}
		 bzero(buf,sizeof buf);
	}
	if(!is_error(msg->json_obj))
	{
		json_object_object_foreach(msg->json_obj, key, val)
		{
			printf("%s\n", json_object_get_string(val));
			if(strcmp(key,"code") == 0)
			{
				msg->code = json_object_get_int(val);
			}

			if(strcmp(key,"content") == 0)
			{
				msg->content = (char *)json_object_get_string(val);
			}
		}
	}
	printf("%s\n", "read msg ok");
}

char * message_to_json_string(message* msg)
{

	return (char *)json_object_to_json_string(msg->json_obj);
}
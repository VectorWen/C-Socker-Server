#ifndef __MESSAGE_H_
#define __MESSAGE_H_


#include "json.h"

typedef struct message
{
	json_object *json_obj;
	int code;
	char * content;
}message;

void message_parse_for_json(message *msg,char * json);
void message_parse_for_socket(message *msg,int sockfd);
char * message_to_json_string(message* msg);

#endif//__MESSAGE_H_
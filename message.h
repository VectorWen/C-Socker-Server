#ifndef __MESSAGE_H_
#define __MESSAGE_H_

struct message
{
	int code;
	char * content;
};

struct message message_parse_for_json(char * json);
char * message_to_json_string(struct message msg);
struct message message_parse_for_socket(int sockfd);

#endif//__MESSAGE_H_
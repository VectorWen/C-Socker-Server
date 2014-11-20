#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include "json.h"
#include "message.h"


json_object *new_obj;

struct message message_parse_for_json(char * json)
{
	printf("%s\n", json);
	struct message msg;
	new_obj = json_tokener_parse(json);

	json_object_object_foreach(new_obj, key, val)
	{
		printf("%s\n", json_object_get_string(val));
		if(strcmp(key,"code") == 0)
		{
			msg.code = json_object_get_int(val);
		}

		if(strcmp(key,"content") == 0)
		{
			msg.content = json_object_get_string(val);
		}
	}
	return msg;
}

struct message message_parse_for_socket(int sockfd)
{
	struct message msg = {.code=400,.content="error"};
	json_tokener * tok = json_tokener_new();
	char buf[1024];
	bzero(buf,1024);
	int len = -1;
	while((len = read(sockfd,buf,1024))>0)
	{
		printf("%s\n", buf);
		new_obj = json_tokener_parse_ex(tok,buf,len);
		if(!is_error(new_obj))
		{
			printf("%s\n", "read json ok");
			break;
		}
		 bzero(buf,sizeof buf);
	}
	if(!is_error(new_obj))
	{
		json_object_object_foreach(new_obj, key, val)
		{
			printf("%s\n", json_object_get_string(val));
			if(strcmp(key,"code") == 0)
			{
				msg.code = json_object_get_int(val);
			}

			if(strcmp(key,"content") == 0)
			{
				msg.content = json_object_get_string(val);
			}
		}
	}
	printf("%s\n", "read msg ok");
	return msg;
}

char * message_to_json_string(struct message msg)
{
	json_object * obj = json_object_new_object();
	json_object_object_add(obj, "code", json_object_new_int(msg.code));
	json_object_object_add(obj, "content", json_object_new_string(msg.content));

	return json_object_to_json_string(obj);
}
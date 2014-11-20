#include <stdio.h>
#include "message.h"

int main(int argc, char const *argv[])
{
	char json[] = "/* more difficult test case */ { \"code\":88,\"content\":\"I'm jf\"}";
	struct message msg = message_parse_for_json(json);
	printf("code = %d, content = %s\n", msg.code,msg.content);

	printf("%s\n", message_to_json_string(msg));

	return 0;
}

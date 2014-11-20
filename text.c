#include <stdio.h>
#include <errno.h>
#include "server.h"

void  callback	(int sockfd)
{
	char buf[1024];
	while(1)
    {
        int len;
        if((len = read(sockfd,buf,1024)) == -1)
        {
            fprintf(stderr,"Read Error:%s\n",strerror(errno));
            return;
        }else if(len == 0)
        {
         	printf("%s\n", "end");
         	return;
         }

         printf("Receive the client's data --> %s\n", buf);
         printf("Sent to the client's data --> %s\n", buf);
         if(write(sockfd,buf,sizeof(buf))==-1)//写和普通文件一样
         {
            fprintf(stderr,"Write Error:%s\n",strerror(errno));
            return;
         }
         bzero(buf,sizeof buf);
        
	}
}

int main(int argc, char const *argv[])
{
	server_init(8080);
	server_accept(callback);
	return 0;
}
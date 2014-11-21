#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include "thread_pool.h"
#include "server.h"


void destroy_server(server *serv)
{
         close(serv->sockfd);
         destroy_pool(&(serv->tp));
}

void * tp_task(void *arg)
{
        client * cli = (client *)arg;
        cli->task(cli->sockfd,cli->arg);
        close(cli->sockfd);
        free(cli);
}

int server_accept(server *serv,void (* task)(int sockfd,void *arg),void *arg)
{
        while(1)
        {
        		 client * cli = (client *)malloc(sizeof (client));
  
                /* 服务器阻塞,直到客户程序建立连接 */
                int sin_size=sizeof(struct sockaddr_in);
                if((cli->sockfd=accept(serv->sockfd,(struct sockaddr *)(&cli->addr),&sin_size))==-1)//等待连接，拿到连接client 的描述符和sockaddr
                {
                        fprintf(stderr,"Accept error:%s\n\a",strerror(errno));
                        free(cli);
                        return -1;
                }

                fprintf(stderr,"Server get connection from %s\n",
                inet_ntoa(cli->addr.sin_addr));

               
               cli->arg = arg;
               cli->task = task;

                add_task(&(serv->tp),tp_task,(void *)cli);
        }
}

int server_init(server *serv,int portnumber)
{
         /* 服务器端开始建立socket描述符 */
        if((serv->sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)  //打开socket通信文件
        {
                fprintf(stderr,"Socket error:%s\n\a",strerror(errno));
                return -1;
        }

        /* 服务器端填充 sockaddr结构 */ 
        bzero(&serv->addr,sizeof(struct sockaddr_in));
        serv->addr.sin_family=AF_INET;
        serv->addr.sin_addr.s_addr=htonl(INADDR_ANY);//将主机的无符号长整形数转换成网络字节顺序。INADDR_ANY 为0 的，不明天这个函数
//这里不应该是htonl, 应该是htons -- 不是long 是short
        serv->addr.sin_port=htons(portnumber);

        /* 捆绑sockfd描述符 */ 
        if(bind(serv->sockfd,(struct sockaddr *)(&serv->addr),sizeof(struct sockaddr))==-1)// 为socket描述符绑定端口
        {
                fprintf(stderr,"Bind error:%s\n\a",strerror(errno));
                return -1;
        }

        /* 监听sockfd描述符 */
        if(listen(serv->sockfd,5)==-1) // 监听
        {
                fprintf(stderr,"Listen error:%s\n\a",strerror(errno));
                return -1;
        }
        
		if(serv->threads_number<=0||serv->threads_number>MAX_ACTIVE_THREADS)
		{
			serv->threads_number = DEFAULT_THREADS_NUMBER;
		}
        
        init_pool(&serv->tp,serv->threads_number);
}

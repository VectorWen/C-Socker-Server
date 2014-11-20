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

thread_pool tp;

int sockfd,new_fd;
struct sockaddr_in server_addr;
struct sockaddr_in client_addr;
unsigned int threads_number = DEFAULT_THREADS_NUMBER;

void destroy_server()
{
         close(sockfd);
         destroy_pool(&tp);
}

void * tp_task(void *arg)
{
        struct sock_task * callback_struct = (struct sock_task *)arg;
        callback_struct->callback_func(callback_struct->sockfd);

        close(callback_struct->sockfd);
        free(callback_struct);
}

int server_accept(void (* callback)(int sockfd))
{
        while(1)
        {
                /* 服务器阻塞,直到客户程序建立连接 */
                int sin_size=sizeof(struct sockaddr_in);
                if((new_fd=accept(sockfd,(struct sockaddr *)(&client_addr),&sin_size))==-1)//等待连接，拿到连接client 的描述符和sockaddr
                {
                        fprintf(stderr,"Accept error:%s\n\a",strerror(errno));
                        return -1;
                }

                fprintf(stderr,"Server get connection from %s\n",
                inet_ntoa(client_addr.sin_addr));

                struct sock_task * callback_struct = (struct sock_task *)malloc(sizeof (struct sock_task));
                callback_struct->callback_func = callback;
                callback_struct->sockfd = new_fd;

                add_task(&tp,tp_task,(void *)callback_struct);
        }
}

int server_init(int portnumber)
{
         /* 服务器端开始建立socket描述符 */
        if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)  //打开socket通信文件
        {
                fprintf(stderr,"Socket error:%s\n\a",strerror(errno));
                return -1;
        }

        /* 服务器端填充 sockaddr结构 */ 
        bzero(&server_addr,sizeof(struct sockaddr_in));
        server_addr.sin_family=AF_INET;
        server_addr.sin_addr.s_addr=htonl(INADDR_ANY);//将主机的无符号长整形数转换成网络字节顺序。INADDR_ANY 为0 的，不明天这个函数
//这里不应该是htonl, 应该是htons -- 不是long 是short
        server_addr.sin_port=htons(portnumber);

        /* 捆绑sockfd描述符 */ 
        if(bind(sockfd,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr))==-1)// 为socket描述符绑定端口
        {
                fprintf(stderr,"Bind error:%s\n\a",strerror(errno));
                return -1;
        }

        /* 监听sockfd描述符 */
        if(listen(sockfd,5)==-1) // 监听
        {
                fprintf(stderr,"Listen error:%s\n\a",strerror(errno));
                return -1;
        }
        init_pool(&tp,threads_number);
}

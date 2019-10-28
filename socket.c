 #include <sys/socket.h>
 #include "socket.h"
 
int socketfd;

void open_socket()
{
	 int ret;
	 socketfd = socket(AF_INET, SOCK_DGRAM, 0);	
	 
	struct sockaddr_in bindaddr;
	bindaddr.sin_family=AF_INET;
	bindaddr.sin_addr.s_addr=inet_addr("192.168.1.15"); 
	bindaddr.sin_port=htons(2234);	
	
	ret = bind(socketfd,(struct sockaddr *)&bindaddr,sizeof(bindaddr));
	
	if(ret == -1)
	{
		printf("create socket failed!\n");
		return -1;
	}
}

void close_socket()
{
	close(socketfd);
}

void recmessage()
{
	char buf[30];
	struct sockaddr_in boyaddr;
	int addrsize=sizeof(boyaddr);
	while(1)
	{
		bzero(buf, 30); 
		recvfrom(socketfd,buf,30,0,(struct sockaddr *)&boyaddr,&addrsize);
		printf("接受的数据是:%s\n",buf);
	}	
}

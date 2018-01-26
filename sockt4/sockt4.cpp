#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <error.h>
#include "sockt4.h"

sockt4::sockt4() 
{
	sockFd = -1;
};

int  sockt4::create(sockt4::domain d, sockt4::type t)
{
	int type=0;
	switch(t)
	{
		case TCP: type=SOCK_STREAM; break;
		case UDP: type=SOCK_DGRAM; break;
	}
	sockFd = ::socket(AF_INET, type, 0);
	if(-1 == sockFd)
		return -1;
	else
		return 0;
}

int  sockt4::bind(string ip4, unsigned short port)
{
	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	if (0 == inet_aton(ip4.c_str(), &(saddr.sin_addr)))  //converts ip4 into network-byte-order sin_addr
		return -1;

	int enable = 1;
	if (setsockopt(sockFd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
		printf("sotsockopt failed\n");
	
	int result = ::bind(sockFd, (struct sockaddr *)&saddr, sizeof(saddr));

	if(-1==result)
		return -1;


	return 0;
}

int sockt4::listen(int qLen)
{
	int result = ::listen(sockFd, qLen);
	if(-1==result)
		return -1;

	return 0;
}

int sockt4::accept(sockt4 &rsock) 
{
	struct sockaddr_in raddr;
	socklen_t rlen = sizeof(raddr);
	printf("waiting for new con on sockFd=%d\n", sockFd);
	int result = ::accept(sockFd, (struct sockaddr *)&raddr, (socklen_t *)&rlen);
	if(-1==result)
		return -1;

	rsock.sockFd = result;
	printf("Accepted new connection. sockFd=%d, rsockFd=%d\n", sockFd, result);
	printAddr();

	return 0;
}

int sockt4::getsockipport(string &ip, unsigned short &port)
{
	struct sockaddr_in saddr;

	int result=0, len;
	len = sizeof(saddr);

	result = getsockname(sockFd, (struct sockaddr *)&saddr, (socklen_t *)&len);

	if(result == -1)
	{
		printf("sockFd=%d : no local address bound\n", sockFd);
		return -1;
	}

	ip.assign(inet_ntoa(saddr.sin_addr));
	port =  ntohs(saddr.sin_port);

	return 0;
}

int sockt4::getpeeripport(string &ip, unsigned short &port)
{
	struct sockaddr_in raddr;

	int result=0, len;
	len = sizeof(raddr);

	result = getsockname(sockFd, (struct sockaddr *)&raddr, (socklen_t *)&len);

	if(result == -1)
	{
		printf("sockFd=%d : no remote address bound\n", sockFd);
		return -1;
	}

	ip.assign(inet_ntoa(raddr.sin_addr));
	port =  ntohs(raddr.sin_port);

	return 0;
}


int sockt4::connect(string rip, unsigned short port)
{
	struct sockaddr_in raddr;
	raddr.sin_family = AF_INET;
	raddr.sin_port = htons(port);
	if (0 == inet_aton(rip.c_str(), &(raddr.sin_addr)))  //converts ip4 into network-byte-order sin_addr
		return -1;

	int result = ::connect(sockFd, (const sockaddr *)&raddr, (socklen_t)sizeof(raddr) );
	if(-1==result)
		return -1;

	return 0;
}

int sockt4::send(void * buf, int bufLen)
{
	int result = ::send(sockFd, buf, bufLen, 0);
	if(-1 == result)
		return -1;

	return result;
}

int sockt4::recv(void *buf, int *bufLen)
{
	int result = ::recv(sockFd, buf, *bufLen, MSG_PEEK);
	if(result > *bufLen)
	{
		*bufLen = result;
		return -2;
	}

	result = ::recv(sockFd, buf, *bufLen, 0);
	*bufLen = result;
	return result;
}

void sockt4::printAddr()
{
	struct sockaddr_in saddr, raddr;

	int result=0, len;
	len = sizeof(saddr);

	result = getsockname(sockFd, (struct sockaddr *)&saddr, (socklen_t *)&len);

	if(result == -1)
	{
		printf("sockFd=%d : no local address bound\n", sockFd);
		return;
	}

	printf("laddr: %15s | lport=%d\n", inet_ntoa(saddr.sin_addr), ntohs(saddr.sin_port) );

	result = getpeername(sockFd, (struct sockaddr *)&raddr, (socklen_t *)&len);
	if(result == -1)
	{
		printf("raddr:              NA | rport=NA\n");
		return;
	}
	printf("raddr: %15s | rport=%d\n", inet_ntoa(raddr.sin_addr), ntohs(raddr.sin_port) );

	return;
}

int sockt4::close()
{
	int result = ::close(sockFd);

	if(-1 == result)
		return -1;
	sockFd = -1;
	return 0;
}

sockt4::~sockt4()
{
/* TO DO */
}

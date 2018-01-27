
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

#include "threadpool.h"

void get_file(sockt4 &sock, char *fname)
{
	unsigned char buf[1024];
	int bufLen;
	ofstream opf;
	int result=0;

	opf.open(fname, ios::out | ios::binary);

	if(!opf.is_open())
	{
		printf("couldn't open %s for writing. stopping now.\n", fname);
		return;
	}

	result = sock.send(fname, strlen(fname)+1);
	if(-1 == result)
	{
		printf("sending file name %s to the server failed\n", fname);
		return;
	}

	bufLen = 1023;
	result = sock.recv(buf, &bufLen);

	if(-2 == result)
	{/* TO DO: Handled increasing buffer size */}
	else if (-1 == result)
	{/* TO DO: Handle error */}

	buf[bufLen] = '\0';
	printf("buf=%s, bufLen=%d\n", buf, bufLen);
	if(2 == bufLen && buf[0] == 'n' && buf[1] == 'a')
	{
		printf("file %s is not available at the server. stopping\n", fname);
		opf.close();
		return;
	}

	if(bufLen>13)
	{
		printf("file size returned by server is more than 12 digits (%s).. something's wrong. stopping\n", buf);
	}

	int flen = atoi((const char *)buf);
	printf("file size = %d\n", flen);

		result = sock.send(buf, 1);
	int dwled=0;
	while(1)
	{
		bufLen = 1023;
		int result = sock.recv(buf, &bufLen);

		if(-2 == result)
		{
			opf.close();
			printf("sock.recv result=%d\n", result);
			break;
			/* TO DO: Handled increasing buffer size */}
		else if (-1 == result)
		{/* TO DO: Handle error */
			opf.close();
			printf("sock.recv result=%d\n", result);
			break;
		}

		dwled += bufLen;
		buf[bufLen] = '\0';
		opf.write((const char *)buf, bufLen);

		printf("bufLen=%d\t\tdwled=%d\t\tflen=%d\n", bufLen, dwled, flen);
		if(dwled == flen)
		{
			opf.close();
			printf("file %s successfully downloaded\n", fname);
			break;
		}
		if(dwled > flen)
		{
			printf("dwled > flen. something wrong\n");
			exit(-1);
		}
		//TO DO: Handle err cases. flen is negative etcc
		result = sock.send(buf, 1);
	}

}



static int count=1;
int main(int argc, char *argv[]) 
{
	if(2 != argc)
	{
		printf("give file name for download\n");
		return -1;
	}

	char cmd[128];
	ThreadPool tp(1);

	int result = 0;
	sockt4 sock;
	result = sock.create(sockt4::IPV4, sockt4::TCP);
	if(-1==result)
	{	printf("socket create failed\n"); 
		return -1;
	} 
	else
		printf("socket created\n");



	result = sock.connect("127.0.0.1", 9876);
	if(-1 == result)
	{
		printf("connect failed. probably server is not up\n");
		return -1;
	}

	get_file(sock, argv[1]);

	sock.close();
	printf("main exiting\n");
}

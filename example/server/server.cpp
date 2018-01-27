
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <fstream>

#include "threadpool.h"

int getfilesize(ifstream &ipf)
{
    streampos begin,end;
    begin = ipf.tellg();
    ipf.seekg (0, ios::end);
    end = ipf.tellg();
    ipf.seekg (0, ios::beg);
    return end-begin;
}


void send_file(sockt4 &sock)
{
	unsigned char buf[1024];
	int bufLen=1023;
	ifstream ipf;

	int result = sock.recv(buf, &bufLen);

	if(-2 == result)
	{/* TO DO: Handled increasing buffer size */}
	else if (-1 == result)
	{/* TO DO: Handle error */}

	buf[bufLen] = '\0';

	printf("filename=%s\n", buf);
	ipf.open((const char *)buf, ios::in | ios::binary);

	if(! ipf.is_open())
	{	
		printf("file %s not available. returning 'na'\n", buf);
		buf[0] = 'n'; buf[1] = 'a';

		bufLen = 2;
		sock.send(buf, bufLen);
		sock.close();

		return;
	}

    // get length of file:
	int flen = getfilesize(ipf);
	printf("file size=%d\n", flen);
	sprintf((char *)buf,"%d", flen);
	bufLen = strlen((const char *)buf);
	result = sock.send(buf, bufLen);

	result = sock.recv(buf, &bufLen);
	int bytesSent = 0;
	while(1)
	{
		bufLen = 1023;
		ipf.read((char *)(buf), bufLen);

		bufLen=ipf.gcount();
		//printf("read=%d\t\t", bufLen);
		int result = sock.send(buf, bufLen);
		//printf("sent=%d\n", result);

		if(-2 == result)
		{/* TO DO: Handled increasing buffer size */}
		else if (-1 == result)
		{/* TO DO: Handle error */}

		bytesSent += bufLen;
		//printf("bytesSent=%d, totalSent=%d\n", bufLen, bytesSent);
		//printf("Got %s, size received=%d\n", buf, bufLen);
		if(bytesSent == flen ||  ipf.eof())
		{
			printf("eof reached. bytes sent=%d, withseekg=%d\n", bytesSent, ipf.tellg());
			sock.close();
			ipf.close();
			break;
		}
		result = sock.recv(buf, &bufLen);
	}

	printf("sendfile done\n");
}



void just_sleep(sockt4 &sock)
{
	printf("just sleep called\n");
	sleep(10);
	printf("just sleep done\n");
}

static int count=1;
main() {

	char cmd[128];
	ThreadPool tp(3);

	int result = 0;
	sockt4 sock;
	result = sock.create(sockt4::IPV4, sockt4::TCP);
	if(-1==result)
	{	printf("socket create failed\n"); 
		return -1;
	} 
	else
		printf("socket created\n");

	result = sock.bind("127.0.0.1", 9876);
	if(-1 == result)
	{
		printf("socket bind failed\n"); 
		return -1;
	} 
	else 
	{
		printf("socket bounded\n"); 
		sock.printAddr();
	}

	result = sock.listen(2);
	if(-1 == result)
	{
		printf("socket listen failed\n"); 
		return -1;
	} 
	else 
		printf("socket listen success\n"); 


	while(fscanf(stdin, "%s", cmd) != EOF) 
	{
		if(strcmp(cmd, "q") == 0) 
		{
			break;
		}
		else if(strcmp(cmd, "a") == 0) 
		{
			sockt4 csock;
			result = sock.accept(csock);
			if(-1 == result)
			{
				printf("socket accept failed\n"); 
				return -1;
			} 
			else 
			{
				printf("socket accept success\n"); 
				csock.printAddr();
			}

			object o;
			o.x = count*11;
			o.sock = csock;
			o.runme = send_file;
			tp.executeJob(o);
			printf("executeJob done\n");
		}
		else 
			break;
	}

	printf("main exiting\n");
}

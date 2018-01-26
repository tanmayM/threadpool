#ifndef _sockt4___
#define _sockt4___

#include <iostream>
using namespace std;

class sockt4
{
	public:
	enum domain
	{
		IPV4,
	};

	enum type{
	TCP, UDP
	};
	
	sockt4();
	~sockt4();
	int create(sockt4::domain d, sockt4::type t);

	int bind(string ip4, unsigned short port);
	int listen(int qLen);

	int accept(sockt4 &rsock);

	int getsockipport(string &ip, unsigned short &port);

	int getpeeripport(string &ip, unsigned short &port);


	int connect(string rip, unsigned short port);

	int send(void * buf, int bufLen);

	int recv(void *buf, int *bufLen);

	void printAddr();

	int close();
	private:
		int sockFd;
};

#endif

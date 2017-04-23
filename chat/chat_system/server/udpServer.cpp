#include"udpServer.h"

Server::Server(int _cap)
	:cap(_cap)
	 ,size(0)
{
	pool.reserve(cap);
}

int Server::initServer(const string& ip,int port)
{
	sock = socket(AF_INET,SOCK_DGRAM,0);
	if(sock < 0)
	{
		print_log("socket is error!",FATAL);
		return sock;
	}
	int opt = 1;
	int retset = setsockopt(sock,SOL_SOCKET,SO_REUSEADDDR,&opt,sizeof(opt));
	if(retset < 0)
	{
		print_log("setsockopt is error!",WARNING);
	}
	
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip.c_str());

	if(bind(sock,(sockaddr*)&addr,sizeof(addr)) < 0)
	{
		print_log("bind is error!",FATAL);
		return -1;
	}
	return sock;
}
void adduser(const sockaddr_in& remote)
{
	user_list.insert(std::pair(remote.sin_addr.s_addr,remote));
}
const int& Server::recvData(string &outstr)
{
	char buf[SIZE];
	sockaddr_in remote;
	socklen_t len = sizeof(remote);
	
	int s = recvfrom(sock,buf,sizeof(buf),0,(sockaddr*)&remote,&len);
	if(s < 0)
	{
		print_log("recvfrom is error!",FATAL);
		return s;
	}
	adduser(remote);
	outstr=buf;
	return s;
}
const int& Server::sendData(const string% instr)
{
	
}
Server::~Server()
{
	close(sock);
}

#include<iostream>
#include<vector>
#include<string>
#include<map>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
using namespace std;

#define SIZE 1024

class Server
{
public:
	Server();
    int initServer(const string& ip,int port);
	const int &recvData();
	const int &sendData();
	~Server();
private:
	Server(const Server&s;
private:
	int sock;
	int cap;
	int size;
	vector<string> pools;
	map<in_addr_t,sockaddr_in> user_list;
	
	
}

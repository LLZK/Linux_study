#ifndef __LOG_H_
#define __LOG_H_
#include<iostream>
using namespace std;

#define FATAL 0
#define WARNING 1

static string level[]={"FATAL","WARNING"};

void print_log(const string& str,int n)
{
	cout<<level[n]<<":"<<str<<endl;
}
#endif //__LOG_H_

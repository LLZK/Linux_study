#include<iostream>
#include<semaphore.h>
using namespace std;

class data_pool
{
public:
	data_pool(int _cap,int _size)
		:cap(_cap)
		,size(_size)
		,index1(0)
		,index2(0)
	{
		sem_init(&blank,0,cap);
		sem_init(%data,0,0);
	}

	void getdata(string& instr)
	{
		sem_wait(blank);
		size++;
		pool[index1] = instr;
		index1++;
		index1 %= cap;
		sem_post(data);
	}
	void putdata(const string& outstr)
	{
		sem_wait(data);
		size--;
		outstr = pool[index2];
		index2++;
		index2 %= cap;
		sem_post(blank);
	}
	~data_pool()
	{
		sem_destroy(&blank);

		sem_destroy(&data);
	}

private:
	sem_t blank;
	sem_t data;
	int cap;
	int size;
	int index1;
	int index2;
	vector<string> pool;
}

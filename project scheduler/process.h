#include"LinkedList.h"
#include"Node.h"
#include"QueueADT.h"
#include"StackADT.h"
#include<string>
class process
{
private:
	int PID;
	int AT;
	int RT;
	int TT;
	int TRT;
	int WT;
	string state;
	int IO_R;
	int IO_D;
	int num_of_IO;
public:
	int CpuTime;
	process()
	{
		state = "NEW";
	}
	process(int at,int pid,int ct,int IO_num, int io_R,int io_D)
	{
		AT = at;
		PID = pid;
		num_of_IO = IO_num;
		IO_R = io_R;
		IO_D = io_D;
	}
	void setstate()
	{

	}
	void print()
	{

	}

};


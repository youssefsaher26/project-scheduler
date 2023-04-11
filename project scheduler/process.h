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

public:
	int CpuTime;
	process()
	{
		state = "NEW";
	}
	void setstate()
	{

	}
	void print()
	{

	}

};


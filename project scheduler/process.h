#include"LinkedList.h"
#include"Node.h"
#include"QueueADT.h"
#include<string>
#pragma once

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
	int kill_time;
	int RemTime;
public:
	int CpuTime;
	process(int at, int pid, int ct, int IO_num, int io_R, int io_D)
	{
		RemTime = ct;
		AT = at;
		PID = pid;
		num_of_IO = IO_num;
		IO_R = io_R;
		IO_D = io_D;
		state = "NEW";
		kill_time = -1;
	}
	process()
	{
		state = "NEW";
		kill_time = -1;
	}
	int GetArrTime()
	{
		return AT;
	}
	int GetRemTime()
	{
		return RemTime;
	}
	int getID()
	{
		return PID;
	}
	void SetRemTime(int y)
	{
		RemTime = y;
	}
	
	void setstate(string s)
	{
		state = s;
	}

	void setkiltime(int t)
	{
		kill_time = t;
	}
	void finishTimes(int t)
	{
		TT = t;
		TRT = TT - AT;
		WT = TRT - CpuTime;
	}
	bool operator > (process* p)
	{
		if (RemTime > p->GetRemTime())
		{
			return true;
		}
		return false;
	}
};


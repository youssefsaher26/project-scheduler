#include"LinkedList.h"
#include"Node.h"
#include"QueueADT.h"
#include<string>
#include"IO_R_D.h"
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
	QueueADT<IO_R_D*>* inputsigs;//if null then no input iutout sigs
	int num_of_IO;
	int kill_time;
	int RemTime;
public:
	int CpuTime;
	process(int at, int pid, int ct, int IO_num, QueueADT<IO_R_D*>* input_sigs)
	{
		RemTime = ct;
		CpuTime = ct;
		AT = at;
		PID = pid;
		num_of_IO = IO_num;
		inputsigs = input_sigs;
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
	void add_inputsigs(int IO_r,int IO_d)
	{

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


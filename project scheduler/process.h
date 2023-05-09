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
	QueueADT<IO_R_D*> inputsigs;//if null then no input output sigs
	int num_of_IO;
	int kill_time;
	int RemTime;
	int Rem_IO;
	int RR_TIME;
public:
	int CpuTime;
	process(int at, int pid, int ct, int IO_num)
	{
		RR_TIME = 0;
		RemTime = ct;
		CpuTime = ct;
		AT = at;
		PID = pid;
		num_of_IO = IO_num;
		state = "NEW";
		kill_time = -1;
		Rem_IO = -1;
	}
	void add_inputs_sigs(IO_R_D* ptr)
	{
		inputsigs.enqueue(ptr);
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
	int GetRRTime()
	{
		return RR_TIME;
	}
	int getID()
	{
		return PID;
	}
	int get_ior()
	{
		return inputsigs.getfront()->getItem()->get_ior();
	}
	int get_iod()
	{
		return inputsigs.getfront()->getItem()->get_iod();
	}
	int get_rem_io()
	{
		return Rem_IO;
	}
	QueueADT<IO_R_D*>* get_inputsigs()
	{
		return &inputsigs;
	}
	void RR_INC()
	{
		RR_TIME++;
	}
	void RR_RESET()
	{
		RR_TIME = 0;
	}
	void SetRemTime(int y)
	{
		RemTime = y;
	}
	
	void setstate(string s)
	{
		state = s;
	}
	void decremtime()
	{
		RemTime--;
	}
	void setkiltime(int t)
	{
		kill_time = t;
	}
	void set_remIO(int t)
	{
		Rem_IO = t;
	}
	void dec_IO()
	{
		Rem_IO--;
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


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
	int TT;//time when process is finished
	int TRT;
	int WT;
	QueueADT<IO_R_D*> inputsigs;//if null then no input output sigs
	int num_of_IO;
	int kill_time;
	int RemTime;
	int Rem_IO;
	int RR_TIME;
	bool forked;
	bool pure;
	process* child;
	
public:
	
	int  CpuTime;
	process(int at, int pid, int ct, int IO_num)
	{
		RR_TIME = 0;
		RemTime = ct;
		CpuTime = ct;
		AT = at;
		PID = pid;
		num_of_IO = IO_num;
		kill_time = -1;
		Rem_IO = -1;
		forked = false;
		pure = true;
		child = nullptr;
	}
	process(int at, int pid, int ct, int IO_num, bool x)
	{
		RR_TIME = 0;
		RemTime = ct;
		CpuTime = ct;
		AT = at;
		PID = pid;
		num_of_IO = IO_num;
		kill_time = -1;
		Rem_IO = -1;
		forked = false;
		pure = x;
		child = nullptr;
	}
	void add_inputs_sigs(IO_R_D* ptr)
	{
		inputsigs.enqueue(ptr);
	}
	process()
	{
		kill_time = -1;
	}
	bool get_forked()
	{
		return forked;
	}
	void set_forked()
	{
		forked = true;
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
		if (inputsigs.getfront() != nullptr)
			return inputsigs.getfront()->getItem()->get_iod();
		else return 0;
	}
	int get_rem_io()
	{
		return Rem_IO;
	}
	int get_TT()
	{
		return TT;
	}
	int get_CT()
	{
		return CpuTime;
	}
	int get_WT()
	{
		return WT;
	}
	int get_RT()
	{
		return RT;
	}
	int get_TRT()
	{
		return TRT;
	}
	bool get_pure()
	{
		return pure;
	}
	process* get_child()
	{
		return child;
	}
	QueueADT<IO_R_D*>* get_inputsigs()
	{
		return &inputsigs;
	}
	void RR_INC()
	{
		RR_TIME++;
	}
	void setchild(process* p)
	{
		child = p;
	}
	void RR_RESET()
	{
		RR_TIME = 0;
	}
	void SetRemTime(int y)
	{
		RemTime = y;
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


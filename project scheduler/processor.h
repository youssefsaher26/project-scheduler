#include"LinkedList.h"
#include"QueueADT.h"
#include "process.h"
#pragma once


class processor
{
protected:
	process* RUN;
	int processornumber;
	int type; //1 is FCFS, 2 is RR, 3 is SJF
	int IdleTime;
	int BusyTime; //phase 2
public:
	bool Block;
	bool Terminate;
	bool State; //  0-IDLE 1-BUSY
	int TotalTRT;
	virtual void RDY_TO_RUN() = 0;
	virtual void RUN_TO_RDY() = 0;
	virtual int queuetime() = 0;
	virtual void AddProcess(process* p) = 0;
	virtual bool Done() = 0;
	virtual void SchedAlgo() = 0;
	virtual void NeedBlock();
	virtual void NeedTrm();
	virtual void STATE();

	processor()
	{
		IdleTime = 0;
		BusyTime = 0;
		RUN = nullptr;
		State = 0;
		Block = 0;
		Terminate = 0;
	}
	virtual void STATE()
	{
		if(RUN==nullptr)
		{
			State = 0;
		}
		else
		{
			State = 1;
		}
	}
	bool getstate()
	{
		return State;
	}
	int get_type()
	{
		return type;
	}
	int getpnumber()
	{
		return processornumber;
	}
	process* GetRun()
	{
		return RUN;

	}
	virtual void NeedBlock()
	{
		int c_iors = RUN->get_inputsigs()->getcount();//count of iors
		if (c_iors == 0)
		{
			Block = 0;
			return;
		}
		int ct = RUN->CpuTime;
		int rem = RUN->GetRemTime();
		int io_r = RUN->get_ior();
		if ((ct - rem) == io_r)
		{
			Block = 1;
		}
	
	}
	virtual void NeedTrm()
	{
		int c = RUN->GetRemTime();
		if (c == 0)
		{
			Terminate = 1;
		}
	}

	virtual void setstate(bool s)
	{
		State = s;
	}
	void SetRun()//sets run=null
	{
		RUN = nullptr;
	}
	bool operator > (processor* p)
	{
		if (this->queuetime() > p->queuetime())
			return true;
		return false;
	}
	bool operator < (processor* p)
	{
		if (this->queuetime() < p->queuetime())
			return true;
		return false;
	}
	virtual int pLoad()
	{
		int x = BusyTime / TotalTRT;
		return x;
		
	}
	virtual int pUtil()
	{
		int x = BusyTime / (BusyTime + IdleTime);
		return x;
	}
	
};
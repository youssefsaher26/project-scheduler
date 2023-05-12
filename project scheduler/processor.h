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
	process* mig;
	process* block;
	process* trm;
	int TIME;
public:
	bool fork_it;
	bool State; //  0-IDLE 1-BUSY
	int TotalTRT;
	virtual void RDY_TO_RUN() = 0;
	virtual void RUN_TO_RDY() = 0;
	virtual int queuetime() = 0;
	virtual void AddProcess(process* p) = 0;
	virtual bool Done() = 0;
	virtual void SchedAlgo() = 0;
	virtual process* KILL(int y) = 0;

	processor()
	{
		TIME = 0;
		IdleTime = 0;
		BusyTime = 0;
		RUN = nullptr;
		State = 0;
		mig = nullptr;
		block = nullptr;
		trm = nullptr;
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
	void resetFork()
	{
		fork_it = false;
	}
	bool getstate()
	{
		return State;
	}
	process* getmigrate()
	{
		return mig;
	}
	process* gettrm()
	{
		return trm;
	}
	process* getblock()
	{
		return block;
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
		int c_iors = RUN->get_inputsigs()->getcount();
		if (c_iors == 0)
		{
			block = nullptr;
			return;
		}
		else
		{
			IO_R_D* io;
			RUN->get_inputsigs()->peek(io);
			int ct = RUN->CpuTime;
			int rem = RUN->GetRemTime();
			int io_r = io->get_ior();
			if ((ct - rem) == io_r)
			{
				block = RUN;
				block->set_remIO(io->get_iod());
				RUN = nullptr;
			}
			else
			{
				block = nullptr;
			}
		}
	
	}
	virtual void NeedTrm()
	{
		int c = RUN->GetRemTime();
		if (c == 0)
		{
			trm = RUN;
			RUN = nullptr;
		}
	}

	virtual void setstate(bool s)
	{
		State = s;
	}
	void resetmigrate()
	{
		mig = nullptr;
	}
	void resetblock()
	{
		block = nullptr;
	}
	void resettrm()
	{
		trm = nullptr;
	}
	void SetRun()//sets run=null
	{
		RUN = nullptr;
	}
	void time(int x)
	{
		TIME = x;
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
	bool get_forkit()
	{
		return fork_it;
	}
	
};
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
	
	bool State; //  0-IDLE 1-BUSY
	int TotalTRT;
	virtual void RDY_TO_RUN() = 0;
	virtual void RUN_TO_RDY() = 0;
	virtual int queuetime() = 0;
	virtual void AddProcess(process* p) = 0;
	virtual bool Done() = 0;

	processor()
	{
		IdleTime = 0;
		BusyTime = 0;
		RUN = nullptr;
		State = 0;

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
	virtual void SchduleAlgo()
	{
		if (State == 0)
		{
			int random = 1 + (rand() % 100);
			if (random >= 1 && random <= 15)
			{
				//RUNtoBLK();
			}if (random >= 20 && random <= 30)
			{
				//RUNtoRDY();
			}
			if (random>=50 && random<= 60)
			{
				//RUNtoTRM();
			}
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

	//virtual int pLoad()
	//{
	//	int x = BusyTime / TotalTRT;
	//	return x;
	//	
	//}
	//virtual int pUtil()
	//{
	//	int x = BusyTime / (BusyTime + IdleTime);
	//	return x;
	//}
	
};
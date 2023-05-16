#include"LinkedList.h"
#include"QueueADT.h"
#include "process.h"
#pragma once


class processor
{
protected:
	process* RUN;
	int processornumber;
	int type; //1 is FCFS, 2 is RR, 3 is SJF, 4 is EDF
	int IdleTime;
	int BusyTime; //phase 2
	process* mig;
	process* block;
	process* trm;
	int TIME;
	bool frozen;
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
	virtual process* donate() = 0;
	virtual int stealqueuetime() = 0;

	processor();
	virtual void STATE();
	void resetFork();
	bool getstate();
	process* getmigrate();
	process* gettrm();
	process* getblock();
	int get_type();
	int getpnumber();
	bool get_forkit();
	int get_busy_time();
	int get_idle_time();
	bool getfrozen();
	void setfrozen(bool f);

	process* GetRun();
	virtual void NeedBlock();
	virtual void NeedTrm();
	virtual void setstate(bool s);
	void resetmigrate();
	void resetblock();
	void resettrm();
	void SetRun();
	void time(int x);
	bool operator > (processor* p);
	bool operator < (processor* p);
	virtual int pLoad(int TotalTRT);
	virtual int pUtil();
};
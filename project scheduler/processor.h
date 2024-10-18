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
	int BusyTime; 
	process* mig; //pointer to the process that needs to be migrated. usually null
	process* block;
	process* trm;
	int TIME; 
	int rr_no;
	int sjf_no;
	bool frozen; // 1 if processor is overheated, 0 if processor is operating normally
public:
	bool fork_it; //checked each time step in the scheduler to know if a process needs to be forked or not
	bool State; //  0-IDLE 1-BUSY
	int TotalTRT;
	virtual process* donate_steal() = 0;
	virtual void RDY_TO_RUN() = 0; 
	virtual void RUN_TO_RDY() = 0;
	virtual int queuetime() = 0; //caclulates the total queue time (including RUN process)
	virtual int stealqueuetime() = 0; //calculates the total queue time but without adding the RUN process (corner case, sth in RUN, nth in RDY)
	virtual void AddProcess(process* p) = 0; //depends on the processor's ADT and how something is added to its rdy list
	virtual bool Done() = 0; //called in scheduler to know when simulation needs to be ended
	virtual void SchedAlgo() = 0; //serves running process, and checks if it needs to terminated, blocked, forked(fcfs), or killed(fcfs)
	virtual process* KILL(int y) = 0; //returns nullptr in rr, sjf, edf but looks for it in fcfs.
	virtual process* donate() = 0; //return a process from the top of the rdy list (needed in stealing and overheating)
	virtual void destruct() = 0; //function called in processor destructor
	processor();
	virtual void STATE(); //called in the end of each time step to set the state of the processor
	//GETTERS
	bool getstate();
	process* getmigrate(); //pointer to process that needs to be migrated
	process* gettrm(); //pointer to process that needs to be terminated
	process* getblock(); //pointer to process that needs to be blocked
	int get_type(); 
	int getpnumber(); //processor number
	bool get_forkit(); //getter for whether the RUN needs to be forked or not
	int get_busy_time();
	int get_idle_time();
	bool getfrozen(); //whether this processor has overheated or not
	process* GetRun(); //returns pointer to the running process
	void setrr_no(int y);
	void setsjf_no(int y);
	virtual void NeedBlock();// checks the running process and sees if it needs to blocked or not
	virtual void NeedTrm();
	//SETTERS
	void setfrozen(bool f);
	virtual void setstate(bool s); 
	void resetmigrate(); //makes mig pointer null 
	void resetblock(); //makes the BLK pointer null
	void resettrm(); //makes the TRM pointer null
	void resetFork(); //resets the boolean of whether the run process needs to be forked or not
	void SetRun();
	void Time(int x);
	virtual double pLoad(int TotalTRT);
	virtual int pUtil();
	bool operator > (processor* p);
	bool operator < (processor* p);
};
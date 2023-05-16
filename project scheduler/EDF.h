#pragma once
#include"processor.h"
#include"PriorityQueue.h"
class EDF:public processor 
{
private:
	PriorityQueue<process*>* EDF_RDY;

public:
	EDF(int x);
	virtual void AddProcess(process* p);
	virtual int queuetime();
	virtual int stealqueuetime();
	void RDY_TO_RUN();
	bool Replace_RUN();
	virtual void SchedAlgo();
	virtual process* donate();
	virtual process* KILL(int id);
	void RUN_TO_RDY();
	virtual bool Done();

};


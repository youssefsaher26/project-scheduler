#include"LinkedList.h"
#include"Node.h"
#include"QueueADT.h"
#include "processor.h"
#include "process.h"
#include <iostream>
#pragma once
class FCFS : public processor
{
private:
	int forkprob;
	LinkedList<process*>* FCFS_RDY;
public:
	static int MaxW;
	FCFS(int x, int y);
	virtual void AddProcess(process* p);
	virtual int queuetime();
	virtual int stealqueuetime();
	void RDY_TO_RUN();
	void migration(process* p);
	void fork();
	virtual void SchedAlgo();
	void RUN_TO_RDY();
	virtual bool Done();
	virtual process* donate();
	LinkedList<process*>* get_FCFS_RDY();
	static void set_Maxw(int x);
	virtual process* KILL(int ID);
	friend ostream& operator<< (ostream& out, const FCFS& p)
	{
		out << "[FCFS] : " << p.FCFS_RDY->get_count() << " RDY: ";
		if (p.FCFS_RDY->get_count() == 0)
		{
			return out;
		}
		Node <process*>* ptr = p.FCFS_RDY->GetHead();
		while (ptr)
		{
			out << ptr->getItem()->getID();
			if (ptr->getNext() != nullptr)
			{
				out << " , ";
			}
			ptr = ptr->getNext();
		}
		return out;
	}
};

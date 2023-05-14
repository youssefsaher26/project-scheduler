#include"LinkedList.h"
#include"Node.h"
#include"QueueADT.h"
#include "processor.h"
#include "process.h"
#pragma once
class RoundRobin : public processor
{
private:
	QueueADT<process*>* RR_RDY;
public:
	static int RTF;
	static int TimeSlice;
	RoundRobin(int x);
	virtual process* KILL(int id);
	virtual void AddProcess(process* p);
	virtual int queuetime();
	virtual int stealqueuetime();
	void RDY_TO_RUN();
	void MIGRATION(process* p);
	virtual void SchedAlgo();
	void RUN_TO_RDY();
	virtual bool Done();
	static void set_Timeslice(int x)
	{
		TimeSlice = x;
	}
	static void set_RTF(int x)
	{
		RTF = x;
	}
	virtual process* donate();
	friend ostream& operator<< (ostream& out, const RoundRobin& p)
	{
		out << "[RR  ] : "<< p.RR_RDY->getcount()<<" RDY: ";
		if (p.RR_RDY->getcount()==0)
		{
			return out;
		}
		Node <process*>* ptr = p.RR_RDY->getfront();
		while (ptr)
		{
			out << ptr->getItem()->getID();
			if (ptr == p.RR_RDY->getrear())
			{
				break;
			}
			if (ptr->getNext() != nullptr)
			{
				out << " , ";
			}
			ptr = ptr->getNext();
		}
		return out;
	}
};



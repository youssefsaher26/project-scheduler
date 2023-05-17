#include"LinkedList.h"
#include"Node.h"
#include"PriorityQueue.h"
#include "processor.h"
#include"process.h"
#pragma once
class SJF : public processor
{
private:
	PriorityQueue<process*>* SJF_RDY;

public:
	SJF(int x);
	virtual int queuetime();
	virtual int stealqueuetime();
	virtual void AddProcess(process* p);
	void RDY_TO_RUN();
	virtual process* donate_steal();
	virtual void SchedAlgo();
	virtual process* donate();
	virtual process* KILL(int id);
	void RUN_TO_RDY();
	virtual bool Done();
	virtual void destruct();
	friend ostream& operator<< (ostream& out, const SJF& p)
	{
		out << "[SJF ] : " << p.SJF_RDY->getcount() << " RDY: ";
		if (p.SJF_RDY->getcount() == 0)
		{
			return out;
		}
		PNode <process*>* ptr = p.SJF_RDY->getfront();
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

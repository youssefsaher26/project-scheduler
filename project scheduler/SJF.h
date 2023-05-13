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
	SJF(int x)
	{
		SJF_RDY = new PriorityQueue<process*>;
		type = 3;
		processornumber = x;
	}

	virtual int queuetime()
	{
		int sum = 0;
		Node<process*>* temp= SJF_RDY->getfront();
		while (temp)
		{
			process* C = temp->getItem();
			sum = C->GetRemTime() + sum;
			temp = temp->getNext();
		}
		if (RUN)
			sum = RUN->GetRemTime() + sum;
		return sum;
	}
	virtual void AddProcess(process* p)
	{
		SJF_RDY->enqueue(p);
	}
	void RDY_TO_RUN()	
	{ 
		if (RUN == nullptr && !SJF_RDY->isEmpty())
		{
			SJF_RDY->dequeue(RUN);
			if (RUN->GetRemTime() == RUN->get_CT())
				RUN->setRT(TIME);
		}
	}
	virtual void SchedAlgo()
	{
		if (!RUN)
		{
			RDY_TO_RUN();
		}
		if (RUN)
		{
			NeedBlock();
			if (RUN)
			{
				NeedTrm();
			}
			if (block == nullptr && trm == nullptr)
			{
				RUN->decremtime();
			}
		}
	}
	virtual process* donate()
	{
		process* p;
		SJF_RDY->dequeue(p);
		if (p)
			return p;
		else
			return nullptr;
	}
	virtual process* KILL(int id)
	{
		return nullptr;
	}
	void RUN_TO_RDY()
	{
		SJF_RDY->enqueue(RUN);
		RUN = NULL;
		State = 0;
	}
	virtual bool Done()
	{
		if (RUN == nullptr && SJF_RDY->isEmpty() == true)
			return true;
		return false;
	}
	friend ostream& operator<< (ostream& out, const SJF& p)
	{
		out << "[SJF ] : " << p.SJF_RDY->getcount() << " RDY: ";
		if (p.SJF_RDY->getcount() == 0)
		{
			return out;
		}
		Node <process*>* ptr = p.SJF_RDY->getfront();
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

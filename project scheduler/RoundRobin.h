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
	static int TimeSlice;
	static int RTF;
	RoundRobin(int x)
	{
		RR_RDY = new QueueADT<process*>;
		type = 2;
		processornumber = x;
	}
	virtual process* KILL(int id)
	{
		return nullptr;
	}
	virtual void AddProcess(process* p)
	{
		RR_RDY->enqueue(p);
	}
	virtual int queuetime()
	{
		// add run time too
		int sum = 0;
		Node <process*>* ptr = RR_RDY->getfront();
		while (ptr)
		{
			sum = sum + ptr->getItem()->GetRemTime();
			if (ptr == RR_RDY->getrear())
			{
				break;
			}
			ptr = ptr->getNext();
		}
		return sum;
	}
	void RDY_TO_RUN()
	{
		if (RUN == nullptr)
		{
			if (RR_RDY->isEmpty() == false)
			{
				process* p;
				RR_RDY->dequeue(p);
				MIGRATION(p);
				if (mig == nullptr)
				{
					if (p->GetRemTime() == p->get_CT())
						p->setRT(TIME);
					RUN = p;
				}
			}
		}
	}
	void MIGRATION(process* p)
	{
		if (p->GetRemTime() < RTF)
		{
			mig = p;
		}
		else
		{
			mig = nullptr;
		}
	}
	virtual void SchedAlgo()
	{
		if (!RUN)
		{
			RDY_TO_RUN();
		}
		if (mig == nullptr)
		{
			if (RUN)
			{
				NeedBlock();
				if (RUN)
				{
					NeedTrm();
				}
				if (!RUN)
				{
					if (trm)
						trm->RR_RESET();
					else if (block)
						block->RR_RESET();
				}
				if (block == nullptr && trm==nullptr)
				{
					RUN->decremtime();
					RUN->RR_INC();
					if (RUN->GetRemTime() == 0)
					{
						trm = RUN;
						RUN->RR_RESET();
						RUN = nullptr;
					}
					else if (RUN->GetRRTime() == TimeSlice)
					{
						RUN->RR_RESET();
						RR_RDY->enqueue(RUN);
						RR_RDY->dequeue(RUN);
					}
				}
			}
		}
	}
	void RUN_TO_RDY()
	{
		RR_RDY->enqueue(RUN);
		RUN = NULL;
		State = 0;
	}
	virtual bool Done()
	{
		if (RUN == nullptr && RR_RDY->isEmpty() == true)
			return true;
		return false;
	}
	static void set_Timeslice(int x)
	{
		TimeSlice = x;
	}
	static void set_RTF(int x)
	{
		RTF = x;
	}
	virtual process* donate()
	{
		process* p=nullptr;
		RR_RDY->dequeue(p);
		if (p)
			return p;
		else
			return nullptr;
	}
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
int RoundRobin::RTF = 0;
int RoundRobin::TimeSlice=0;


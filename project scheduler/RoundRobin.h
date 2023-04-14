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
	RoundRobin()
	{
		RR_RDY = new QueueADT<process*>;
		type = 2;
	}

	virtual void AddProcess(process* p)
	{
		RR_RDY->CircEnqueue(p);
	}
	virtual int queuetime()
	{
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
	}
	void RDY_TO_RUN()
	{
		RR_RDY->CircDequeue(RUN);
		State = 1;
	}
	void RUN_TO_RDY()
	{
		RR_RDY->CircEnqueue(RUN);
		RUN = NULL;
		State = 0;
	}
	static void set_Timeslice(int x)
	{
		TimeSlice = x;
	}
	static void set_RTF(int x)
	{
		RTF = x;
	}
	
	friend ostream& operator<< (ostream& out, const RoundRobin& p)
	{
		out << "[RR  ] : " << p.RUN->getID() << " RDY: ";
		Node <process*>* ptr = p.RR_RDY->getfront();
		while (ptr)
		{
			out << ptr->getItem()->getID() <<" , ";
			if (ptr == p.RR_RDY->getrear())
			{
				break;
			}
			ptr = ptr->getNext();
		}
	}
};
int RoundRobin::RTF = 0;
int RoundRobin::TimeSlice=0;


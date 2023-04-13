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
	}
	static void set_Timeslice(int x)
	{
		TimeSlice = x;
	}
	static void set_RTF(int x)
	{
		RTF = x;
	}
	virtual int queuetime()
	{

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


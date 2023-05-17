#pragma once
#include"processor.h"
#include"PriorityQueue.h"
class EDF:public processor 
{
private:
	PriorityQueue<process*>* EDF_RDY; //EDF RDY list

public:
	EDF(int x);
	virtual process* donate_steal();
	virtual void AddProcess(process* p);
	virtual void destruct();
	virtual int queuetime();
	virtual int stealqueuetime();
	void RDY_TO_RUN(); 
	bool Replace_RUN(); //replaces the run with the earliest deadline in RDY list
	virtual void SchedAlgo();
	virtual process* donate();
	virtual process* KILL(int id);
	void RUN_TO_RDY();
	virtual bool Done(); 
	friend ostream& operator<< (ostream& out, const EDF& p)
	{
		out << "[EDF ] : " << p.EDF_RDY->getcount() << " RDY: ";
		if (p.EDF_RDY->getcount() == 0)
		{
			return out;
		}
		PNode <process*>* ptr = p.EDF_RDY->getfront();
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
	} //operator overloading
};


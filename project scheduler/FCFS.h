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
	
	LinkedList<process*>* FCFS_RDY;
public:
	FCFS()
	{
		FCFS_RDY= new LinkedList<process*>;
	}
	virtual void AddProcess(process* p)
	{
		FCFS_RDY->InsertEnd(p);
	}
	static int MaxW;
	virtual int queuetime()
	{
		int sum = 0;
		Node<process*>* temp = FCFS_RDY->GetHead();
		while (temp)
		{
			process* C = temp->getItem();
			sum = C->CpuTime + sum;
			temp = temp->getNext();
		}
			return sum;
	}
	void FCFS_RDY_TO_RUN()
	{
		Node<process*>* temp = FCFS_RDY->GetHead();
		FCFS_RDY->SetHead(temp->getNext());
		RUN = temp->getItem();
		delete temp;
	}
	void RUN_TO_FCFS_RDY()
	{
		FCFS_RDY->InsertEnd(RUN);
		RUN = NULL;
	}
	LinkedList<process*>* get_FCFS_RDY()
	{
		
		return FCFS_RDY;
	}
	static void set_Maxw(int x)
	{
		MaxW = x;
	}

	friend ostream& operator<< (ostream& out, const FCFS& p)
	{
		out << "[FCFS] : " << p.RUN->getID() << " RDY: ";
		Node <process*>* ptr = p.FCFS_RDY->GetHead();
		while (ptr)
		{
			out << ptr->getItem()->getID() <<" , ";
			ptr = ptr->getNext();
		}
	}
};
int FCFS::MaxW = 0;

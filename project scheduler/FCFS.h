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
	static int MaxW;

	FCFS()
	{
		FCFS_RDY= new LinkedList<process*>;
		type = 1;
	}
	virtual void AddProcess(process* p)
	{
		FCFS_RDY->InsertEnd(p);
	}
	virtual int queuetime()
	{
		int sum = 0;
		Node<process*>* temp = FCFS_RDY->GetHead();
		while (temp)
		{
			process* C = temp->getItem();
			sum = C->GetRemTime() + sum;
			temp = temp->getNext();
		}
			return sum;
	}
	void RDY_TO_RUN()
	{
		Node<process*>* temp = FCFS_RDY->GetHead();
		if (temp == nullptr)
		{
			return;
		}
		else
		{
			FCFS_RDY->SetHead(temp->getNext());
			RUN = temp->getItem();
			State = 1;
		}
	}
	void RUN_TO_RDY()
	{
		FCFS_RDY->InsertEnd(RUN);
		RUN = NULL;
		State = 0;
	}
	LinkedList<process*>* get_FCFS_RDY()
	{
		return FCFS_RDY;
	}
	static void set_Maxw(int x)
	{
		MaxW = x;
	}

	process* ForcedTRM(int ID)
	{
		Node <process*>* p = FCFS_RDY->GetHead();
		while (p)
		{
			int id = p->getItem()->getID();
			if (id == ID)
			{
				process* temp = p->getItem();
				FCFS_RDY->DeleteNode(temp);
				return temp;

			}
			p = p->getNext();
		}
		return nullptr;
	}

	friend ostream& operator<< (ostream& out, const FCFS& p)
	{
		out << "[FCFS] : " << p.RUN->getID() << " RDY: ";
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
	}
};
int FCFS::MaxW = 0;

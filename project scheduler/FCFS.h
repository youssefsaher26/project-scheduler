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
	FCFS(int x, int y)
	{
		FCFS_RDY= new LinkedList<process*>;
		type = 1;
		processornumber = x;
		forkprob = y;
		fork_it = false;
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
		if (RUN == nullptr)
		{
			Node<process*>* temp = FCFS_RDY->GetHead();
			if (temp == nullptr)
			{
				return;
			}
			else
			{
				RUN = temp->getItem();
				FCFS_RDY->DeleteFirst();
				State = 1;
			}
		}
	}
	void fork()
	{
		int forking_no = rand() % 100 + 1;
		if(forking_no<forkprob)
		{
			if (!(RUN->get_forked()))
			{
				RUN->set_forked();
				fork_it = true;
			}
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
			NeedTrm();
			if (Block == 0 && Terminate == 0)
			{
				RUN->decremtime();
			}
		}
	}
	void RUN_TO_RDY()
	{
		FCFS_RDY->InsertEnd(RUN);
		RUN = NULL;
		State = 0;
	}
	virtual bool Done()
	{
		if (RUN == nullptr && FCFS_RDY->isEmpty() == true)
			return true;
		return false;
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

		if (RUN != nullptr)
		{
			if (RUN->getID() == ID)
			{
				process* t = RUN;
				RUN = nullptr;
				State = 0;
				return t;

			}
		}
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
int FCFS::MaxW = 0;

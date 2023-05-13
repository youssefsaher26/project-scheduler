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
			sum = sum + C->GetRemTime();
			temp = temp->getNext();
		}
		return sum;
	}
	void RDY_TO_RUN()
	{
		if (RUN == nullptr)
		{
			if (FCFS_RDY->isEmpty()==false)
			{
				process* temp = FCFS_RDY->GetHead()->getItem();
				FCFS_RDY->DeleteFirst();
				if (temp->get_pure())
				{
					migration(temp);
				}
				if (mig == nullptr)
				{
					if (temp->GetRemTime() == temp->get_CT())
						temp->setRT(TIME);
					RUN = temp;
				}

			}
		}
	}
	void migration(process* p)
	{
		int arrtime = p->GetArrTime();
		int cpu = p->get_CT();
		int rem = p->GetRemTime();
		if (TIME-arrtime-cpu+rem > MaxW)
		{
			mig = p;
		}
		else
		{
			mig = nullptr;
		}
	}
	void fork()
	{
		if (RUN != nullptr)
		{
			int forking_no = rand() % 100 + 1;
			if (forking_no < forkprob)
			{
				if (!(RUN->get_forked()))
				{
					RUN->set_forked();
					fork_it = true;
				}
			}
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
				if (block == nullptr && trm == nullptr)
				{
					RUN->decremtime();
					fork();
				}
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
	virtual process* donate()
	{
		if (FCFS_RDY->isEmpty() == false)
		{
			process* p=nullptr;
			p = FCFS_RDY->GetHead()->getItem();
			FCFS_RDY->DeleteFirst();
		}
		return nullptr;
	}
	LinkedList<process*>* get_FCFS_RDY()
	{
		return FCFS_RDY;
	}
	static void set_Maxw(int x)
	{
		MaxW = x;
	}
	virtual process* KILL(int ID)
	{
		if (RUN)
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
				FCFS_RDY->DeleteNode(p->getItem());
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

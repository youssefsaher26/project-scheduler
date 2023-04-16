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
		return sum;
	}
	virtual void AddProcess(process* p)
	{
		SJF_RDY->enqueue(p);
	}
	void RDY_TO_RUN()
	{
		SJF_RDY->dequeue(RUN);
		State = 1;
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
		out << "[SJF ] : " << p.RUN->getID() <<" RDY: ";
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
	}
};

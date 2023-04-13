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
	SJF()
	{
		SJF_RDY = new PriorityQueue<process*>;
	}
	//Node<process*>* findmin()
	//{
	//	Node<process*>* ptr = SJF_RDY->getfront();
	//	Node<process*>* min = ptr;
	//	if (ptr != nullptr)
	//	{
	//		int x = ptr->getItem()->CpuTime;
	//		while (ptr)
	//		{
	//			ptr = ptr->getNext();
	//			if (ptr->getItem()->CpuTime < x)
	//			{
	//				min = ptr;
	//				x = min->getItem()->CpuTime;
	//			}
	//		}
	//		return min;
	//	}
	//	return nullptr;
	//}
	void Add_To_SJF_RDY(process* p)
	{
		SJF_RDY->enqueue(p);
	}
	virtual int queuetime()
	{
		int sum = 0;
		Node<process*>* temp= SJF_RDY->getfront();
		while (temp)
		{
			process* C = temp->getItem();
			sum = C->CpuTime + sum;
			temp = temp->getNext();
		}
		return sum;
	}
	virtual void AddProcess(process* p)
	{
		SJF_RDY->enqueue(p);
	}
	void SJF_RDY_TO_RUN()
	{
		SJF_RDY->dequeue(RUN);
	}
	void RUN_TO_SJF_RDY()
	{
		SJF_RDY->enqueue(RUN);
		RUN = NULL;
	}
	friend ostream& operator<< (ostream& out, const SJF& p)
	{
		out << "[SJF] : " << p.RUN->getID() <<" RDY: ";
		Node <process*>* ptr = p.SJF_RDY->getfront();
		while (ptr)
		{
			out << ptr->getItem()->getID() << " , ";
			ptr = ptr->getNext();
		}
	}
};

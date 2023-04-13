#include"LinkedList.h"
#include"Node.h"
#include"QueueADT.h"
#include"StackADT.h"
#include "processor.h"
#include"process.h"
#pragma once
class SJF : public processor
{
private:
	QueueADT<process*>* SJF_RDY;
	Node<process*>* min;

public:
	SJF()
	{
		SJF_RDY = new QueueADT<process*>;
	}
	Node<process*>* findmin()
	{
		Node<process*>* ptr = SJF_RDY->getfront();
		Node<process*>* min = ptr;
		if (ptr != nullptr)
		{
			int x = ptr->getItem()->CpuTime;
			while (ptr)
			{
				ptr = ptr->getNext();
				if (ptr->getItem()->CpuTime < x)
				{
					min = ptr;
					x = min->getItem()->CpuTime;
				}
			}
			return min;
		}
		return nullptr;
	}

	//void insert(process* p)
	//{
	//	Node<process*>* spprv = SJF_RDY->getfront();
	//	if (spprv == nullptr)
	//	{
	//		SJF_RDY->enqueue(p);
	//		return;
	//	}
	//	else
	//	{
	//		Node<process*>* ptr = new Node<process*>(p);
	//		Node<process*>* sp = SJF_RDY->getfront()->getNext();
	//		while (sp)
	//		{
	//			int z = spprv->getItem()->CpuTime;
	//			if (z> p->CpuTime)
	//			{
	//				ptr->setNext(spprv);
	//				ptr = SJF_RDY->getfront();
	//			}
	//			int y = sp->getItem()->CpuTime;
	//			if (y > p->CpuTime)
	//			{
	//				sp = sp->getNext();
	//			}
	//		}
	//	}
	//	//last step
	//	int x = SJF_RDY->getcount() + 1;
	//	SJF_RDY->setcount(x);
	//}


	virtual int queuetime()
	{
		int sum = 0;
		Node<process*>* temp;
		while (temp)
		{
			temp = SJF_RDY->getfront();
			process* C = temp->getItem();
			sum = C->CpuTime + sum;
			temp = temp->getNext();
		}
		return sum;
	}
	
};

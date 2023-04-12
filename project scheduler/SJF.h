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
	QueueADT<process*> SJF_RDY;

public:
public:
	virtual int queuetime()
	{
		int sum = 0;
		Node<process*>* temp;
		while (temp)
		{
			temp = SJF_RDY.getfront();
			process* C = temp->getItem();
			sum = C->CpuTime + sum;
			temp = temp->getNext();
		}
		return sum;
	}
	
};

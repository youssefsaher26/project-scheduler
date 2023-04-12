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


public:
	LinkedList<process>* FCFS_RDY;
	virtual int queuetime()
	{

	}
	
};


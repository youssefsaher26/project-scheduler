#include "SJF.h"

SJF::SJF(int x)
{
	SJF_RDY = new PriorityQueue<process*>;
	type = 3;
	processornumber = x;
}
int SJF:: queuetime()
{
	int sum = 0;
	Node<process*>* temp = SJF_RDY->getfront();
	while (temp)
	{
		process* C = temp->getItem();
		sum = C->GetRemTime() + sum;
		temp = temp->getNext();
	}
	if (RUN)
		sum = RUN->GetRemTime() + sum;
	return sum;
}
int SJF:: stealqueuetime()
{
	int sum = 0;
	Node<process*>* temp = SJF_RDY->getfront();
	while (temp)
	{
		process* C = temp->getItem();
		sum = C->GetRemTime() + sum;
		temp = temp->getNext();
	}
	return sum;
}
void SJF::AddProcess(process* p)
{
	SJF_RDY->enqueue(p);
}
void SJF::RDY_TO_RUN()
{
	if (RUN == nullptr && !SJF_RDY->isEmpty())
	{
		SJF_RDY->dequeue(RUN);
		if (RUN->GetRemTime() == RUN->get_CT())
			RUN->setRT(TIME);
	}
}
void SJF::SchedAlgo()
{
	if (!RUN)
	{
		RDY_TO_RUN();
	}
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
		}
	}
}
process* SJF:: donate()
{
	process* p;
	SJF_RDY->dequeue(p);
	if (p)
		return p;
	else
		return nullptr;
}
process* SJF:: KILL(int id)
{
	return nullptr;
}
void SJF:: RUN_TO_RDY()
{
	SJF_RDY->enqueue(RUN);
	RUN = NULL;
	State = 0;
}
bool SJF:: Done()
{
	if (RUN == nullptr && SJF_RDY->isEmpty() == true)
		return true;
	return false;
}
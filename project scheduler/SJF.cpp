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
	PNode<process*>* temp = SJF_RDY->getfront();
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
process* SJF:: donate_steal()
{
	return this->donate();
}
int SJF:: stealqueuetime()
{
	int sum = 0;
	PNode<process*>* temp = SJF_RDY->getfront();
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
	SJF_RDY->enqueue(p,p->GetRemTime());
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
	process* p = nullptr;
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
	SJF_RDY->enqueue(RUN,RUN->GetRemTime());
	RUN = NULL;
	State = 0;
}
bool SJF:: Done()
{
	if (RUN == nullptr && SJF_RDY->isEmpty() == true)
		return true;
	return false;
}
void SJF::destruct()
{
	SJF_RDY->~PriorityQueue();
	delete RUN;
	delete mig;
	delete block;
	delete trm;
}
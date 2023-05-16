#include "EDF.h"
EDF::EDF(int x)
{
	EDF_RDY = new PriorityQueue<process*>;
	type = 4;
	processornumber = x;
}
void EDF::AddProcess(process* p)
{
	EDF_RDY->enqueue(p, p->get_EDF());
}
int EDF::queuetime()
{
	int sum = 0;
	PNode<process*>* temp = EDF_RDY->getfront();
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

int EDF::stealqueuetime()
{
	int sum = 0;
	PNode<process*>* temp = EDF_RDY->getfront();
	while (temp)
	{
		process* C = temp->getItem();
		sum = C->GetRemTime() + sum;
		temp = temp->getNext();
	}
	return sum;
}

void EDF::RDY_TO_RUN()
{
	if (RUN == nullptr && !EDF_RDY->isEmpty())
	{
		EDF_RDY->dequeue(RUN);
		if (RUN->GetRemTime() == RUN->get_CT())
			RUN->setRT(TIME);
	}

}

bool EDF::Replace_RUN()
{
	process* ptr;
	if (!EDF_RDY)
		return false;
	EDF_RDY->peek(ptr);
	if (ptr->get_EDF() < RUN->get_EDF())
	{
		EDF_RDY->enqueue(RUN,RUN->get_EDF());
		EDF_RDY->dequeue(ptr);
		RUN = ptr;
		return true;
	}
	else
	{
		return false;
	}
}

void EDF::SchedAlgo()
{

	if (!RUN)
	{
		RDY_TO_RUN();
	}
	Replace_RUN();
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

process* EDF::donate()
{
	process* p = nullptr;
	EDF_RDY->dequeue(p);
	if (p)
		return p;
	else
		return nullptr;
}

process* EDF::KILL(int id)
{
	return nullptr;
}

void EDF::RUN_TO_RDY()
{
	EDF_RDY->enqueue(RUN, RUN->get_EDF());
	RUN = NULL;
	State = 0;
}

bool EDF::Done()
{
	if (RUN == nullptr && EDF_RDY->isEmpty() == true)
		return true;
	return false;
}

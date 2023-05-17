#include"RoundRobin.h"
int RoundRobin::RTF = 0;
int RoundRobin::TimeSlice = 0;
RoundRobin:: RoundRobin(int x)
{
	RR_RDY = new QueueADT<process*>;
	type = 2;
	processornumber = x;
}
process* RoundRobin::KILL(int id)
{
	return nullptr;
}
void RoundRobin:: AddProcess(process* p)
{
	RR_RDY->enqueue(p);
}
int RoundRobin:: queuetime()
{
	// add run time too
	int sum = 0;
	Node <process*>* ptr = RR_RDY->getfront();
	while (ptr)
	{
		sum = sum + ptr->getItem()->GetRemTime();
		if (ptr == RR_RDY->getrear())
		{
			break;
		}
		ptr = ptr->getNext();
	}
	if (RUN)
		sum = RUN->GetRemTime() + sum;
	return sum;
}
int RoundRobin:: stealqueuetime()
{
	// add run time too
	int sum = 0;
	Node <process*>* ptr = RR_RDY->getfront();
	while (ptr)
	{
		sum = sum + ptr->getItem()->GetRemTime();
		if (ptr == RR_RDY->getrear())
		{
			break;
		}
		ptr = ptr->getNext();
	}
	return sum;
}
void RoundRobin:: RDY_TO_RUN()
{
	if (RUN == nullptr)
	{
		if (RR_RDY->isEmpty() == false)
		{
			process* p;
			RR_RDY->dequeue(p);
			MIGRATION(p);
			if (mig == nullptr)
			{
				if (p->GetRemTime() == p->get_CT())
					p->setRT(TIME);
				RUN = p;
			}
		}
	}
}
void RoundRobin:: MIGRATION(process* p)
{
	if (pno == 1)
	{
		mig = nullptr;
		return;
	}
	if (p->GetRemTime() < RTF && pno>1)
	{
		mig = p;
	}
	else
	{
		mig = nullptr;
	}
}
void RoundRobin:: SchedAlgo()
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
			if (!RUN)
			{
				if (trm)
					trm->RR_RESET();
				else if (block)
					block->RR_RESET();
			}
			if (block == nullptr && trm == nullptr)
			{
				RUN->decremtime();
				RUN->RR_INC();
				if (RUN->GetRRTime() == TimeSlice && RUN->GetRemTime() != 0)
				{
					RUN->RR_RESET();
					RR_RDY->enqueue(RUN);
					RR_RDY->dequeue(RUN);
				}

			}
		}
	}
}
void RoundRobin:: RUN_TO_RDY()
{
	RR_RDY->enqueue(RUN);
	RUN = NULL;
	State = 0;
}
bool RoundRobin:: Done()
{
	if (RUN == nullptr && RR_RDY->isEmpty() == true)
		return true;
	return false;
}
process* RoundRobin:: donate()
{
	process* p = nullptr;
	RR_RDY->dequeue(p);
	if (p)
		return p;
	else
		return nullptr;
}
process* RoundRobin::donate_steal()
{
	return this->donate();
}
void RoundRobin::destruct()
{
	RR_RDY->~QueueADT();
	delete RUN;
	delete mig;
	delete block;
	delete trm;
}
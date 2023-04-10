#include"LinkedList.h"
#include"Node.h"
#include"QueueADT.h"
#include "process.h"
class scheduler;

class processor
{
protected:
	LinkedList<process>* RDY;
	process* RUN;
	scheduler* Sptr;
public:
	bool State; //  0-IDLE 1-BUSY
	int IdleTime;
	int BusyTime;
	int TotalTRT;
	virtual void SchduleAlgo()
	{
		if (State == 0)
		{
			int random = 1 + (rand() % 100);
			if (random >= 1 && random <= 15)
			{
				RUNtoBLK();
			}if (random >= 20 && random <= 30)
			{
				RUNtoRDY();
			}
			if (random>=50 && random<= 60)
			{
				RUNtoTRM();
			}
		}
	}
	virtual int queuetime() = 0;
	virtual void setstate(bool s)=0;
	virtual int pLoad()
	{
		int x = BusyTime / TotalTRT;
		return x;
		
	}
	virtual int pUtil()
	{
		int x = BusyTime / (BusyTime + IdleTime);
		return x;
	}
	virtual void AddProcess(process* p) = 0;
	virtual void RUNtoRDY()
	{
		RDY->InsertEnd(*RUN);
		RUN = NULL;
	}
	virtual void RUNtoTRM()
	{
		Sptr->getTRM()->enqueue(*RUN);
		RUN = NULL;
	}
	virtual void RUNtoBLK()
	{
		Sptr->getBLK()->enqueue(*RUN);
		RUN = NULL;
	}
};


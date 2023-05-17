#include "processor.h"
processor:: processor()
{
	TIME = 0;
	IdleTime = 0;
	BusyTime = 0;
	frozen = 0;
	RUN = nullptr;
	State = 0;
	mig = nullptr;
	block = nullptr;
	trm = nullptr;
}
void processor:: STATE()
{
	if (RUN == nullptr)
	{
		State = 0;
		IdleTime++;
	}
	else
	{
		State = 1;
		BusyTime++;
	}
}
void processor:: resetFork()
{
	fork_it = false;
}
bool processor:: getstate()
{
	return State;
}
process* processor:: getmigrate()
{
	return mig;
}
process* processor:: gettrm()
{
	return trm;
}
process* processor:: getblock()
{
	return block;
}
int processor:: get_type()
{
	return type;
}
int processor:: getpnumber()
{
	return processornumber;
}
bool processor:: get_forkit()
{
	return fork_it;
}
int processor:: get_busy_time()
{
	return BusyTime;
}
int processor:: get_idle_time()
{
	return IdleTime;
}
process* processor:: GetRun()
{
	return RUN;

}
void processor::setfrozen(bool f)
{
	frozen = f;
}
bool processor::getfrozen()
{
	return frozen;
}
void processor:: NeedBlock()
{
	int c_iors = RUN->get_inputsigs()->getcount();
	if (c_iors == 0)
	{
		block = nullptr;
		return;
	}
	else
	{
		IO_R_D* io;
		RUN->get_inputsigs()->peek(io);
		int ct = RUN->get_CT();
		int rem = RUN->GetRemTime();
		int io_r = io->get_ior();
		if ((ct - rem) == io_r)
		{
			block = RUN;
			block->set_remIO(io->get_iod());
			RUN = nullptr;
			RDY_TO_RUN();
		}
		else
		{
			block = nullptr;
		}
	}

}
void processor:: NeedTrm()
{
	int c = RUN->GetRemTime();
	if (c == 0)
	{
		trm = RUN;
		RUN = nullptr;
		RDY_TO_RUN();
	}
}
void processor:: setstate(bool s)
{
	State = s;
}
void processor:: resetmigrate()
{
	mig = nullptr;
}
void processor:: resetblock()
{
	block = nullptr;
}
void processor:: resettrm()
{
	trm = nullptr;
}
void processor:: SetRun()//sets run=null
{
	RUN = nullptr;
}
void processor:: time(int x)
{
	TIME = x;
}
int processor::pLoad(int TotalTRT)
{
	if (TotalTRT == 0)
		return 0;
	int x = ((BusyTime * 100) / TotalTRT);
	return x;

}
int processor::pUtil()
{
	if (BusyTime + IdleTime == 0)
		return 0;
	int z = (BusyTime + IdleTime);
	int x = ((BusyTime * 100) / z);
	return x;
}
bool processor:: operator > (processor* p)
{
	if (this->queuetime() > p->queuetime())
		return true;
	return false;
}
bool processor:: operator < (processor* p)
{
	if (this->queuetime() < p->queuetime())
		return true;
	return false;
}
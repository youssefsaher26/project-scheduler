#include "process.h"
process:: process(int at, int pid, int ct, int IO_num)
{
	RR_TIME = 0;
	RemTime = ct;
	CpuTime = ct;
	AT = at;
	PID = pid;
	num_of_IO = IO_num;
	kill_time = -1;
	Rem_IO = -1;
	forked = false;
	pure = true;
	child1 = nullptr;
	child2 = nullptr;
	total_io_D = 0;
	childrenno = 0;
}
process:: process(int at, int pid, int ct, int IO_num, int edf, bool x)
{
	EDF = edf;
	RR_TIME = 0;
	RemTime = ct;
	CpuTime = ct;
	AT = at;
	PID = pid;
	num_of_IO = IO_num;
	kill_time = -1;
	Rem_IO = -1;
	forked = false;
	pure = x;
	child1 = nullptr;
	child2 = nullptr;
	total_io_D = 0;
	childrenno = 0;
}
process::process(int at, int pid, int ct, int IO_num, int edf)
{
	RR_TIME = 0;
	RemTime = ct;
	CpuTime = ct;
	AT = at;
	PID = pid;
	num_of_IO = IO_num;
	kill_time = -1;
	Rem_IO = -1;
	forked = false;
	pure = true;
	child1 = nullptr;
	child2 = nullptr;
	total_io_D = 0;
	childrenno = 0;
	EDF = edf;
}
process:: process()
{
	kill_time = -1;
}
void process::inc_children()
{
	childrenno++;
}
int process:: get_children_no()
{
	return childrenno;
}
void process:: add_inputs_sigs(IO_R_D* ptr)
{
	inputsigs.enqueue(ptr);
}
void process:: set_total_io_D(int io_d)
{
	total_io_D = io_d + total_io_D;
}
int process:: GetArrTime()
{
	return AT;
}
int process:: GetRemTime()
{
	return RemTime;
}
int process:: GetRRTime()
{
	return RR_TIME;
}
int process:: getID()
{
	return PID;
}
int process:: get_ior()
{
	return inputsigs.getfront()->getItem()->get_ior();
}
int process:: get_iod()
{
	if (inputsigs.getfront() != nullptr)
		return inputsigs.getfront()->getItem()->get_iod();
	else return 0;
}
int process:: get_rem_io()
{
	return Rem_IO;
}
int process:: get_total_io_D()
{
	return total_io_D;
}
int process:: get_TT()
{
	return TT;
}
int process:: get_CT()
{
	return CpuTime;
}
int process:: get_WT()
{
	return WT;
}
int process:: get_RT()
{
	return RT;
}
int process:: get_TRT()
{
	return TRT;
}
bool process:: get_pure()
{
	return pure;
}
int process::get_EDF()
{
	return EDF;
}
void process:: setRT(int x)
{
	RT = x - AT;
}
process* process:: get_child1()
{
	return child1;
}
process* process::get_child2()
{
	return child2;
}
QueueADT<IO_R_D*>* process:: get_inputsigs()
{
	return &inputsigs;
}
void process:: RR_INC()
{
	RR_TIME++;
}
void process:: setchild1(process* p)
{
	child1 = p;
}
void process::setchild2(process* p)
{
	child2 = p;
}
void process:: RR_RESET()
{
	RR_TIME = 0;
}
void process:: SetRemTime(int y)
{
	RemTime = y;
}
void process:: decremtime()
{
	RemTime--;
}
void process:: setkiltime(int t)
{
	kill_time = t;
}
void process:: set_remIO(int t)
{
	Rem_IO = t;
}
void process:: dec_IO()
{
	Rem_IO--;
}
void process:: finishTimes(int t)
{
	TT = t;
	TRT = TT - AT;
	WT = TRT - CpuTime;
}
void process:: finish_Kill_Times(int t)
{
	TT = t;
	TRT = TT - AT;
	WT = TRT - (CpuTime - RemTime);

}
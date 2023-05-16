#include"LinkedList.h"
#include"Node.h"
#include"QueueADT.h"
#include<string>
#include"IO_R_D.h"
#pragma once

class process
{
private:
	int PID;
	int AT;
	int RT;
	int TT;//time when process is finished
	int TRT;
	int WT;
	QueueADT<IO_R_D*> inputsigs;//if null then no input output sigs
	int total_io_D;
	int num_of_IO;
	int kill_time;
	int RemTime;
	int Rem_IO;
	int RR_TIME;
	bool forked;
	bool pure;
	process* child1;
	process* child2;
	int  CpuTime;
	int childrenno;
	
public:
	process(int at, int pid, int ct, int IO_num);
	process(int at, int pid, int ct, int IO_num, bool x);
	process();
	void inc_children();
	void add_inputs_sigs(IO_R_D* ptr);
	void set_total_io_D(int io_d);
	void set_forked();
	int GetArrTime();
	int GetRemTime();
	int GetRRTime();
	int getID();
	int get_ior();
	int get_iod();
	int get_rem_io();
	int get_total_io_D();
	int get_TT();
	int get_CT();
	int get_WT();
	int get_RT();
	int get_TRT();
	bool get_pure();
	void setRT(int x);
	process* get_child1();
	process* get_child2();
	int get_children_no();
	QueueADT<IO_R_D*>* get_inputsigs();
	void RR_INC();
	void setchild1(process* p);
	void setchild2(process* p);
	void RR_RESET();
	void SetRemTime(int y);
	void decremtime();
	void setkiltime(int t);
	void set_remIO(int t);
	void dec_IO();
	void finishTimes(int t);
	void finish_Kill_Times(int t);
	bool operator > (process* p);
};


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
	bool pure; //if process is a child, pure=0. if process is not, pure=1.
	process* child1;
	process* child2;
	int  CpuTime;
	int childrenno; //0-2
	int EDF;

public:
	//constructor that is called while forking to inour pure as 0 to know that this process was forked
	process(int at, int pid, int ct, int IO_num,int edf, bool x); 
	//constructor that is called in load function
	process(int at, int pid, int ct, int IO_num,int edf);
	process();
	void inc_children(); //function that increases count of children when a process is forked
	void add_inputs_sigs(IO_R_D* ptr); //function used in load file to assign IOs to the process in the form of an ior object
	void set_total_io_D(int io_d); //adds the total iod to print it in the output file
	//GETTERS:
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
	int get_EDF();
	process* get_child1();
	process* get_child2();
	int get_children_no();
	QueueADT<IO_R_D*>* get_inputsigs();
	void setRT(int x); //setter for the RT (time where process reached CPU for first time)
	void RR_INC(); //function used in RR_SCHEDALGO to increment the time spent in RR processor until timeslice is reached
	void setchild1(process* p);
	void setchild2(process* p);
	void RR_RESET(); //resets the time spent in RR when the timeslice is over
	void SetRemTime(int y); //setter for remaining time
	void decremtime(); //decrements remaining cpu time 
	void setkiltime(int t);
	void set_remIO(int t);
	void dec_IO(); //decrements remaining IO_D to know when BLKtoRDY needs to be called
	void finishTimes(int t); //calculates the times needed after a process is terminated(TT, WT, TRT)
	void finish_Kill_Times(int t);//calculates the times needed when a process is killed (TT, WT, TRT)
	bool operator > (process* p)
	{
		if (RemTime > p->GetRemTime())
		{
			return true;
		}
		return false;
	}
};


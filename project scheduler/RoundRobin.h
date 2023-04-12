#include"LinkedList.h"
#include"Node.h"
#include"QueueADT.h"
#include "processor.h"
#include "process.h"
#pragma once
class RoundRobin : public processor
{
private:
public:
	static int TimeSlice;
	static int RTF;
	static void set_Timeslice(int x)
	{
		TimeSlice = x;
	}
	static void set_RTF(int x)
	{
		RTF = x;
	}
	virtual int queuetime()
	{

	}
};
int RoundRobin::RTF = 0;
int RoundRobin::TimeSlice=0;


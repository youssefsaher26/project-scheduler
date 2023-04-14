#pragma once
class IO_R_D
{
private:
	int io_R;
	int io_D;
	int pid;
public:
	IO_R_D(int io_r,int io_d,int piD)
	{
		io_R = io_r;
		io_D = io_d;
		pid = piD; 
	}
};


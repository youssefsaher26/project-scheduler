#pragma once
class IO_R_D
{
private:
	int io_R=0;
	int io_D=0;
	int pid;
public:
	IO_R_D(int io_r,int io_d,int piD)
	{
		io_R = io_r;
		io_D = io_d;
		pid = piD; 
	}
	int get_ior()
	{
		return io_R;
	}
	int get_iod()
	{
		return io_D;
	}

};


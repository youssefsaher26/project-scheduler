#pragma once
class IO_R_D
{
private:
	int io_R=0;
	int io_D=0;
	int pid;
public:
	IO_R_D(int io_r, int io_d, int piD);
	int get_ior();
	int get_iod();
};


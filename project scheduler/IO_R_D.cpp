#include"IO_R_D.h"
IO_R_D:: IO_R_D(int io_r, int io_d, int piD)
{
	io_R = io_r;
	io_D = io_d;
	pid = piD;
}
int IO_R_D:: get_ior()
{
	return io_R;
}
int IO_R_D:: get_iod()
{
	return io_D;
}
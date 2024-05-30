#include "AllocManager.h"

int main(void)
{
	int* p1 = new int[10];
	int* p2 = new int;
	int* p3 = (int*)0x00000000;
	int* p4 = (int*)0x0000000a;

	delete[] p1;
	delete p2;
	delete p3;
	delete p4;

	return 0;
}

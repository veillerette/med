#include "Step.h"
#include "Staff.h"
#include "Test.h"
#include "System.h"

int main(int argc, char *argv[])
{
	if(2 == argc && (!strcmp(argv[1], "-t") || !strcmp(argv[1], "-test")))
		Test_Main();
	
	exit(EXIT_SUCCESS);
}

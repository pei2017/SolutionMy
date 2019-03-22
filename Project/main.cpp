#include "crtp.h"
#include "cast.h"
#include "invoke.h"
#include "vartemplates.h"


int main()
{
	vartemplates::test();
	crtp::test();
	cast::test();
	enable::test();
	invoke::test();
	return 0;
}
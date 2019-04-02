#include "crtp.h"
#include "cast.h"
#include "invoke.h"
#include "vartemplates.h"
#include "issame.h"
#include "funcargs.h"


int main()
{
	vartemplates::test();
	crtp::test();
	cast::test();
	enable::test();
	invoke::test();
	myisame::Test();
	funcargs::Test();
	return 0;
}
#include "crtp.h"
#include "cast.h"
#include "invoke.h"
#include "vartemplates.h"
#include "issame.h"

int main()
{
	vartemplates::test();
	crtp::test();
	cast::test();
	enable::test();
	invoke::test();
	myisame::Test();
	return 0;
}
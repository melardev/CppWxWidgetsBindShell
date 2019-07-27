#include "Application.h"

#if defined(_WIN32) || defined(_WIN64)
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "Rpcrt4.lib")
#pragma comment(lib, "ws2_32.lib")
#endif

int main(int argc, char** argv)
{
	// Before running make sure to have run in your terminal a listener on port 3002, for example:
	// ncat -v localhost 3002
	Application app;
	app.Run();
	return 0;
}

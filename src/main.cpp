#include "SampleApplication.h"

using namespace ZeroEngine;

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    
	SampleApplication app(800, 600, 24);
	app.Run();
	return 0;
}

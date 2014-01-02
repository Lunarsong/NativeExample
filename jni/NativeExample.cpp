#include <AndroidLog.h>
#include <Android.h>
#include <unistd.h>

#include "ExampleAndroidHandler.h"

extern "C"
{
	void android_main();
}

void android_main()
{
	LOGV( "[Example]: android_main() called!" );

	// Create the handler
	ExampleAndroidHandler* pHandler = new ExampleAndroidHandler();

	// Set the event handler
	Android::SetEventHandler( pHandler );

	// Run!
	pHandler->Run();

	// Application over
	// Note the handler is destroy by Android
}

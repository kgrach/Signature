#include "Settings.h"
#include "WorkManager.h"

void main(int argc, char** argv) {

	auto settings = std::make_shared<Settings>();

	if (!settings->ParseConsoleArgs(argc, argv))
		return;


	WorkManager mngr(settings->GetInterface());
	mngr.StartWork();
	

	while (true) {
		Sleep(0);
	}
	
	mngr.StopWork();
}
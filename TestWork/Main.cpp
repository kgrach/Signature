#include "Settings.h"
#include "WorkManager.h"

#include <iostream>
#include <conio.h>

void main(int argc, char** argv) {

	auto settings = std::make_shared<Settings>();

	if (!settings->ParseConsoleArgs(argc, argv))
		return;


	WorkManager mngr(settings->GetInterface());
	mngr.StartWork();

	std::cout << "Press any key to stop operation" << std::endl;
	
	getch();

	mngr.StopWork();

	while (1) {
		Sleep(100);
	}
	
	//mngr.StopWork();
}
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

	std::cout << "Press Esc key to stop operation" << std::endl;
	
	while (mngr.IsWorkDone() == false && _kbhit() == 0) {
		Sleep(1000);
	}

 	mngr.StopWork();

	getch();

}
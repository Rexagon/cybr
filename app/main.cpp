#include "Core.h"

#include "Intro.h"

#include "MainMenu.h"

int main()
{
    setlocale(LC_ALL, "Russian");

	Core::init();

	try {
		Core::addState<Intro>();

		Core::run();
	}
	catch (const std::exception& e) {
		Log::write("ERROR:", e.what());
	}

	Core::close();

	return 0;
}

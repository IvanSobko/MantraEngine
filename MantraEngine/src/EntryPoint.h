#pragma once
#include <iostream>

#ifdef ME_PLATFORM_WINDOWS

extern Mantra::Application* Mantra::CreateApplication();

int main(int argc, char** argv) {
	auto app = Mantra::CreateApplication();
	app->Run();
	delete app;
}

#endif
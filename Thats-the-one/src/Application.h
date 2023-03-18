#pragma once

#include <iostream>
#include "Window.h"

class Application
{
public:
	Application();
	~Application();

	void Run();
private:
	std::unique_ptr<Window> m_Window;
	bool m_Running = true;
};

#pragma once
#include <string>
#include <iostream>

namespace NConsole
{
	bool Initiate();

	void Terminate();

	template<class ... Args>
	void Print(Args&& ... args);

	template<class ... Args>
	void Println(Args&& ... args);
}

template<class ... Args>
void NConsole::Print(Args&& ... args)
{
	(std::cout << ... << std::forward<Args>(args));
}

template<class ... Args>
void NConsole::Println(Args&& ... args)
{
	(std::cout << ...  << std::forward<Args>(args)) << "\n";
}
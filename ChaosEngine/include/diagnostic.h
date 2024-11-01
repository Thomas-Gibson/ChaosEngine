#pragma once
#include "chaos_api.h"
#include <iostream>
#include <typeinfo>

template <class T>
class CHAOS_API Diagnostic
{
private:
public:
	static unsigned int inits;
	Diagnostic()
	{
		std::cout << typeid(T).name() << "\t\t\t: instanced." << std::endl;
	}

	~Diagnostic()
	{
		std::cout << typeid(T).name() << "\t\t\t: destroyed." << std::endl;
	}
};
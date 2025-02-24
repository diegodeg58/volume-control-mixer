#pragma once
#include <Windows.h>

class Subscriber
{
public:
	virtual void Update(UINT value) = 0;
};
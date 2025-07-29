#pragma once

#include <stdlib.h>

namespace RNG 
{
	int Int(int min, int max);
	unsigned int UInt(unsigned int min, unsigned int max);
	float Float(float min = 0.f, float max = 1.f);
	bool FlipCoin(int n = 1);
}



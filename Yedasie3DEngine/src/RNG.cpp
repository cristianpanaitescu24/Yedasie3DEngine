#include "RNG.h"


namespace RNG
{
	int Int(int min, int max)
	{
		return rand() % (max - min + 1) + min;
	}

	unsigned int UInt(unsigned int min, unsigned int max)
	{
		return (unsigned int)rand() % (max - min + 1) + min;
	}

	float Float(float min, float max)
	{
		return min + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (max - min));
	}

	bool FlipCoin(int n)
	{
		return RNG::Int(0, n) == 0;
	}
}

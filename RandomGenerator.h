#ifndef __RANDOMGENERATOR_H__
#define __RANDOMGENERATOR_H__

#include "MathGeoLib\include\Algorithm\Random\LCG.h"


class RandomGenerator
{
public:
	RandomGenerator();
	
	~RandomGenerator();

	int RandomInt(int, int);

private:
	LCG* random_gen;
};
#endif //Random Generator
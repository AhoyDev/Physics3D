#include "RandomGenerator.h"
#include "MathGeoLib\include\Algorithm\Random\LCG.h"

int RandomGenerator::RandomInt(int _min, int _max)
{
	
	return random_gen->Int(_min, _max);
}

RandomGenerator::RandomGenerator()
{
	random_gen = new math::LCG();
}

RandomGenerator::~RandomGenerator()
{
	delete random_gen;
}
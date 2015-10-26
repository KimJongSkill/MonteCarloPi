#pragma once

#include <cstdint>
#include <random>

class Simulation
{
	std::mt19937_64 Engine;
	std::uniform_real_distribution<double> Distribution;

public:
	Simulation();

	double operator()(std::intmax_t Rounds);
};

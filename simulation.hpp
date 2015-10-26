#pragma once

#include <cstdint>
#include <random>
#include <utility>

class Simulation
{
	std::mt19937_64 Engine;
	std::uniform_real_distribution<double> Distribution;

public:
	Simulation();

	std::pair<std::intmax_t, double> operator()(std::intmax_t Rounds);
};

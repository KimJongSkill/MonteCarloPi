#include "simulation.hpp"

#include <iostream>
#include <chrono>
#include <cmath>

Simulation::Simulation()
{
	std::mt19937_64::result_type Seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();

	Engine.seed(Seed);
}

std::pair<std::intmax_t, double> Simulation::operator()(std::intmax_t Rounds)
{
	std::intmax_t Hits = 0;
	const std::intmax_t Total = Rounds;

	while (Rounds--)
	{
		const double x = Distribution(Engine);
		const double y = Distribution(Engine);

		if (std::sqrt(x * x + y * y) <= 1.0)
			++Hits;
	}

	return { Hits, 4.0 * Hits / Total };
};

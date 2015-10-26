#include "simulation.hpp"

#include <iostream>
#include <chrono>
#include <cmath>
#include <vector>
#include <future>

Simulation::Simulation()
{
	std::mt19937_64::result_type Seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();

	MasterEngine.seed(Seed);
}

std::intmax_t Simulation::Task(std::intmax_t Rounds, std::mt19937_64::result_type Seed)
{
	std::mt19937_64 Engine;
	Engine.seed(Seed);
	std::intmax_t Hits = 0;

	while (Rounds--)
	{
		const double x = Distribution(Engine);
		const double y = Distribution(Engine);

		if (std::sqrt(x * x + y * y) <= 1.0)
			++Hits;
	}

	return Hits;
};

std::pair<std::intmax_t, double> Simulation::operator()(std::intmax_t Rounds, std::size_t Threads)
{
	std::uniform_int_distribution<std::mt19937_64::result_type> IntDistribution;
	std::intmax_t TotalHits = 0;

	if (Threads > 1)
	{
		const std::intmax_t RoundsPerThread = Rounds / Threads;
		const std::intmax_t RoundsLeft = Rounds - RoundsPerThread * Threads;

		std::vector<std::future<std::intmax_t>> Futures;
		while (Threads--)
			Futures.push_back(std::async(&Simulation::Task, this, RoundsPerThread, IntDistribution(MasterEngine)));
		
		TotalHits += Task(RoundsLeft, IntDistribution(MasterEngine));

		for (auto& Future : Futures)
			TotalHits += Future.get();
	}
	else
	{
		TotalHits = Task(Rounds, IntDistribution(MasterEngine));
	}

	return { TotalHits, 4.0 * TotalHits / Rounds };
}

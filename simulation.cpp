#include "simulation.hpp"

#include <iostream>
#include <chrono>
#include <cmath>
#include <vector>
#include <future>
#include <functional>

Simulation::Simulation() : IntDistribution(std::numeric_limits<uint16_t>::min(), std::numeric_limits<uint16_t>::max())
{
	seed_type Seed = static_cast<seed_type>(std::chrono::high_resolution_clock::now().time_since_epoch().count());

	MasterEngine.seed(Seed);
}

std::uint16_t Simulation::GetInt(prng_type& Fallback)
{
	if (TrueEngine.SupportsRDRAND())
	{
		uint16_t Value;
		TrueEngine(Value);

		return Value;
	}
	else
	{
		return IntDistribution(Fallback);
	}
}

Simulation::seed_type Simulation::GetSeed()
{
	return TrueEngine.SupportsRDRAND() ? 0 : MasterEngine();
}

std::intmax_t Simulation::Task(std::intmax_t Rounds, seed_type Seed)
{
	std::unique_ptr<prng_type> Engine;
	if (!TrueEngine.SupportsRDRAND())
		Engine = std::make_unique<prng_type>(Seed);
	std::intmax_t Hits = 0;

	while (Rounds--)
	{
		const std::uint_fast64_t x = GetInt(*Engine);
		const std::uint_fast64_t y = GetInt(*Engine);

		if (std::sqrt(x * x + y * y) <= std::numeric_limits<uint16_t>::max())
			++Hits;
	}

	return Hits;
};

std::pair<std::intmax_t, double> Simulation::operator()(std::intmax_t Rounds, std::size_t Threads)
{
	std::intmax_t TotalHits = 0;

	if (Threads > 1)
	{
		const std::intmax_t RoundsPerThread = Rounds / Threads;
		const std::intmax_t RoundsLeft = Rounds - RoundsPerThread * Threads;

		std::vector<std::future<std::intmax_t>> Futures;
		while (Threads--)
			Futures.push_back(std::async(&Simulation::Task, this, RoundsPerThread, GetSeed()));
		
		TotalHits += Task(RoundsLeft, GetSeed());

		for (auto& Future : Futures)
			TotalHits += Future.get();
	}
	else
	{
		TotalHits = Task(Rounds, GetSeed());
	}

	return { TotalHits, 4.0 * TotalHits / Rounds };
}

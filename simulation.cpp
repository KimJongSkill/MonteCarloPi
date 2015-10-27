#include "simulation.hpp"

#include <chrono>
#include <cmath>
#include <vector>
#include <future>

Simulation::Simulation(ProgressBar& Progress) : Bar(Progress), IntDistribution(std::numeric_limits<uint16_t>::min(), std::numeric_limits<uint16_t>::max())
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

std::intmax_t Simulation::Task(std::intmax_t Rounds, seed_type Seed, int PercentageAllocated)
{
	std::unique_ptr<prng_type> Engine;
	if (!TrueEngine.SupportsRDRAND())
		Engine = std::make_unique<prng_type>(Seed);
	std::intmax_t Hits = 0;

	for (int i = 0; i < PercentageAllocated; ++i)
	{
		for (int j = 0; j < Rounds / PercentageAllocated; ++j)
		{
			const std::uint_fast64_t x = GetInt(*Engine);
			const std::uint_fast64_t y = GetInt(*Engine);

			if (std::sqrt(x * x + y * y) <= std::numeric_limits<uint16_t>::max())
				++Hits;
		}
		
		Bar.Update(1);
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
		const int PercentagePerThread = 100 / Threads;

		std::vector<std::future<std::intmax_t>> Futures;
		while (Threads--)
			Futures.push_back(std::async(&Simulation::Task, this, RoundsPerThread, GetSeed(), PercentagePerThread));
		
		TotalHits += Task(RoundsLeft, GetSeed(), RoundsLeft / Rounds * 100);

		for (auto& Future : Futures)
			TotalHits += Future.get();
	}
	else
	{
		TotalHits = Task(Rounds, GetSeed(), 100 / Threads);
	}

	return { TotalHits, 4.0 * TotalHits / Rounds };
}

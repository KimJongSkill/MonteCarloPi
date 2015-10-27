#include "simulation.hpp"

#include <iostream>
#include <chrono>
#include <cmath>
#include <vector>
#include <future>
#include <functional>

Simulation::Simulation() : Wrapper(TrueEngine)
{
	seed_type Seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();

	MasterEngine.seed(Seed);
}

Simulation::TrueEngineWrapper::TrueEngineWrapper(bpl::crypt::IvyRNG Object) : Engine(Object) {};

std::uint64_t Simulation::TrueEngineWrapper::operator()()
{
	std::uint64_t Value;
	Engine(Value);

	return Value;
}

constexpr std::uint64_t Simulation::TrueEngineWrapper::min()
{
	return std::numeric_limits<std::uint64_t>::min();
}

constexpr std::uint64_t Simulation::TrueEngineWrapper::max()
{
	return std::numeric_limits<std::uint64_t>::max();
}

Simulation::seed_type Simulation::GetSeed()
{
	return TrueEngine.SupportsRDRAND() ? 0 : IntDistribution(MasterEngine);
}

double Simulation::GetReal(prng_type& Fallback)
{
	if (TrueEngine.SupportsRDRAND())
		return RealDistribution(Wrapper);
	else
		return RealDistribution(Fallback);
}

std::intmax_t Simulation::Task(std::intmax_t Rounds, seed_type Seed)
{
	std::unique_ptr<prng_type> Engine;
	if (!TrueEngine.SupportsRDRAND())
		Engine = std::make_unique<prng_type>(GetSeed());
	std::intmax_t Hits = 0;

	while (Rounds--)
	{
		const double x = GetReal(*Engine);
		const double y = GetReal(*Engine);

		if (std::sqrt(x * x + y * y) <= 1.0)
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

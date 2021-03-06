#include "simulation.hpp"

#include <cmath>
#include <vector>
#include <future>
#include <utility>

static inline double Pythagoras(std::uint_fast64_t a, std::uint_fast64_t b)
{
	return std::sqrt(a * a + b * b);
}

Simulation::Simulation(ProgressBar& Progress, bool NoHardware) : Bar(Progress)
{
	if (NoHardware)
		Engine = BaseRNG::New(rng::force_software);
	else
		Engine = BaseRNG::New(rng::auto_select);
};

std::intmax_t Simulation::Task(std::intmax_t Rounds, int PercentageAllocated)
{
	const auto LocalEngine = Engine->NewLocalInstance();
	std::intmax_t Hits = 0;

	for (int i = 0; i < PercentageAllocated; ++i)
	{
		for (int j = 0; j < Rounds / PercentageAllocated; ++j)
		{
			const auto Values = LocalEngine->GetPair();

			if (Pythagoras(Values.first, Values.second) <= std::numeric_limits<uint16_t>::max())
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
			Futures.push_back(std::async(&Simulation::Task, this, RoundsPerThread, PercentagePerThread));
		
		TotalHits += Task(RoundsLeft, RoundsLeft / Rounds * 100);

		for (auto& Future : Futures)
			TotalHits += Future.get();
	}
	else
	{
		TotalHits = Task(Rounds, 100);
	}

	Bar.Set(100);

	return { TotalHits, 4.0 * TotalHits / Rounds };
}

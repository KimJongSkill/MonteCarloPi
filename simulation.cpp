#include "simulation.hpp"

#include <cmath>
#include <vector>
#include <future>

Simulation::Simulation(ProgressBar& Progress) : Bar(Progress) {};

std::intmax_t Simulation::Task(std::intmax_t Rounds, int PercentageAllocated)
{
	auto LocalEngine = Engine->NewLocalInstance();
	std::intmax_t Hits = 0;

	for (int i = 0; i < PercentageAllocated; ++i)
	{
		for (int j = 0; j < Rounds / PercentageAllocated; ++j)
		{
			const std::uint_fast64_t x = LocalEngine->GetInt();
			const std::uint_fast64_t y = LocalEngine->GetInt();

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
			Futures.push_back(std::async(&Simulation::Task, this, RoundsPerThread, PercentagePerThread));
		
		TotalHits += Task(RoundsLeft, RoundsLeft / Rounds * 100);

		for (auto& Future : Futures)
			TotalHits += Future.get();
	}
	else
	{
		TotalHits = Task(Rounds, 100);
	}

	return { TotalHits, 4.0 * TotalHits / Rounds };
}

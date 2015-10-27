#pragma once

#include "progress_bar.hpp"
#include "rng.hpp"

#include <cstdint>
#include <utility>
#include <memory>
#include <Baked Potatoes\Crypt.hpp>

class Simulation
{
	std::unique_ptr<BaseRNG> Engine = BaseRNG::New();
	ProgressBar& Bar;

	std::intmax_t Task(std::intmax_t Rounds, int PercentageAllocated);

public:
	explicit Simulation(ProgressBar& Progress);

	std::pair<std::intmax_t, double> operator()(std::intmax_t Rounds, std::size_t Threads);
};

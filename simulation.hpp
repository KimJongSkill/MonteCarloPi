#pragma once

#include "progress_bar.hpp"
#include "rng.hpp"

#include <cstdint>
#include <utility>
#include <memory>
#include <Baked Potatoes\Crypt.hpp>

class Simulation
{
	std::unique_ptr<BaseRNG> Engine;
	ProgressBar& Bar;

	std::intmax_t Task(std::intmax_t Rounds, int PercentageAllocated);

public:
	Simulation(ProgressBar& Progress, bool NoHardware);

	std::pair<std::intmax_t, double> operator()(std::intmax_t Rounds, std::size_t Threads);
};

#pragma once

#include <cstdint>
#include <random>
#include <utility>

class Simulation
{
	std::mt19937_64 MasterEngine;
	const std::uniform_real_distribution<double> Distribution;

	std::intmax_t Task(std::intmax_t Rounds, std::mt19937_64::result_type Seed);

public:
	Simulation();

	std::pair<std::intmax_t, double> operator()(std::intmax_t Rounds, std::size_t Threads);
};
